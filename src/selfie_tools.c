// Copyright (C) 2015-2017 CEA/DAM
// Copyright (C) 2015-2017 Laurent Nguyen <laurent.nguyen@cea.fr>
//
// This file is part of SelFIe.
//
// This software is governed by the CeCILL-C license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-C
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".

///
/// @file   selfie_tools.c
/// @author NGUYEN Laurent <laurent.nguyen@cea.fr>
/// @date   Mon Sep 21 12:00:00 2015
///
/// @brief  Main file for selfie_tools
///
///
///
#include "config.h"
#include "selfie_tools.h"
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <regex.h>
#include <unistd.h>

// Timer

/// \brief   Get time in seconds
/// \return  Time in seconds
///
/// \details A gettimeofday routine to give access to the wall
///          clock timer on most UNIX-like systems.
///
double selfie_mysecond(void)
{
  struct timeval tp;
  struct timezone tzp;
  int i = 0;

  i = gettimeofday(&tp, &tzp);
  if (i < 0)
	  return 0;
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
};

/// \def   MAX_CMDLINE
/// \brief Maximum size of string to store command line
#define MAX_CMDLINE 2048

/// \brief        Get command line of program
///
/// \param[out]   cmdline Command line
/// \return       exit code
///
/// \details      Read /proc/self/cmdline to retrieve command line
///
int selfie_getcmdline(char *cmdline)
{
  // ssize_t bytes = -1;
  // int fd = -1;

  // do
  //   {
  //     fd = open("/proc/self/cmdline", O_RDONLY | O_NOCTTY);
  //   } while (fd == -1 && errno == EINTR);
  // if(fd == -1)
  //   return EXIT_SUCCESS;
  // do
  //   {
  //     bytes = read(fd, cmdline, MAX_CMDLINE);
  //   } while (bytes == (ssize_t)-1 && errno == EINTR);

  // if (close(fd))
  //   {
  //     errno = EIO;
  //   }

  ssize_t len = 0;
  len = readlink("/proc/self/exe", cmdline, MAX_CMDLINE);
  if (len < 0)
	  return errno;
  if (len >= MAX_CMDLINE)
	  return ENAMETOOLONG;

  return EXIT_SUCCESS;
};

// Managing structures

/// \details
int selfie_alloc_params_in(params_in *in)
{
  char cmdline[MAX_CMDLINE] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  in->time_tol = 0.0;
  in->nb_env_vars = 0;
  in->env_vars = NULL;
  in->env_vars_values = NULL;
  in->nb_exclude_commands = 0;
  in->exclude_commands = NULL;
  in->log_print = 0;
  in->outputfile = NULL;

  selfie_getcmdline(cmdline);
  in->cmdline = strdup(cmdline);

  in->enable = 1;

  return EXIT_SUCCESS;
};

/// \details
int selfie_free_params_in(params_in *in)
{
  int i = 0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  in->time_tol = 0.0;
  if (in->nb_env_vars > 0)
  {
    for (i = 0; i < in->nb_env_vars; i++)
    {
      free(in->env_vars[i]);
      free(in->env_vars_values[i]);
    }
    free(in->env_vars);
    free(in->env_vars_values);
  }
  in->nb_env_vars = 0;
  in->env_vars = NULL;
  in->env_vars_values = NULL;

  if (in->nb_exclude_commands > 0)
  {
    for (i = 0; i < in->nb_exclude_commands; i++)
    {
      free(in->exclude_commands[i]);
    }
    free(in->exclude_commands);
  }
  in->nb_exclude_commands = 0;
  in->exclude_commands = NULL;

  if (in->cmdline != NULL)
    free(in->cmdline);

  if (in->outputfile != NULL)
    free(in->outputfile);

  in->enable = 0;

  return EXIT_SUCCESS;
};

/// \details
int selfie_alloc_params_out(params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  out->log_string = (char *)malloc(STRING_BLOCK * sizeof(char));
  strcpy(out->log_string, "");
  out->log_index = 0;
  out->log_size = STRING_BLOCK;
  out->output_string = (char *)malloc(STRING_BLOCK * sizeof(char));
  strcpy(out->output_string, "");
  out->output_index = 0;
  out->output_size = STRING_BLOCK;

  out->wtime = 0.0;

  return EXIT_SUCCESS;
};

/// \details
int selfie_free_params_out(params_out *out)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  free(out->log_string);
  out->log_index = 0;
  out->log_size = 0;
  free(out->output_string);
  out->output_index = 0;
  out->output_size = 0;

  out->wtime = 0.0;

  return EXIT_SUCCESS;
};

// Managing output

/// \details
int selfie_strcat_log_to_params_out(params_out *out, char *s)
{
  int len = 0;
  int total_size = 0;
  int tmp_block_size = 0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  tmp_block_size = out->log_size;
  len = strlen(s);
  total_size = strlen(out->log_string);
  while (len + total_size > tmp_block_size)
  {
    tmp_block_size += STRING_BLOCK;
  }
  if (tmp_block_size > out->log_size)
  {
    out->log_size = tmp_block_size;
    out->log_string = (char *)realloc(out->log_string, out->log_size);
  }
  strcat(out->log_string, s);
  return EXIT_SUCCESS;
};

/// \details
int selfie_strcat_output_to_params_out(params_out *out, char *s)
{
  int len = 0;
  int total_size = 0;
  int tmp_block_size = 0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  tmp_block_size = out->output_size;
  len = strlen(s);
  total_size = strlen(out->output_string);
  while (len + total_size > tmp_block_size)
  {
    tmp_block_size += STRING_BLOCK;
  }
  if (tmp_block_size > out->output_size)
  {
    out->output_size = tmp_block_size;
    out->output_string = (char *)realloc(out->output_string, out->output_size);
  }
  strcat(out->output_string, s);
  return EXIT_SUCCESS;
};

/// \details
int selfie_json_int_to_log(params_out *out, char const *key, int value)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  char s[1024] = "";
  sprintf(s, "\"%s\": %d, ", key, value);
  selfie_strcat_log_to_params_out(out, s);
  return EXIT_SUCCESS;
};

/// \details
int selfie_json_llu_to_log(params_out *out, char const *key,
			   unsigned long long int value)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  char s[1024] = "";
  sprintf(s, "\"%s\": %llu, ", key, value);
  selfie_strcat_log_to_params_out(out, s);
  return EXIT_SUCCESS;
};

/// \details
int selfie_json_double_to_log(params_out *out, char const *key, double value)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  char s[1024] = "";
  sprintf(s, "\"%s\": %0.2lf, ", key, value);
  selfie_strcat_log_to_params_out(out, s);
  return EXIT_SUCCESS;
};

/// \details
int selfie_json_string_to_log(params_out *out, char const *key,
			      char const *value)
{
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  char s[1024] = "";
  sprintf(s, "\"%s\": \"%s\", ", key, value);
  selfie_strcat_log_to_params_out(out, s);
  return EXIT_SUCCESS;
};

// Managing configuration file

// Chained List for unkown list size
// (local for selfie_tools)

/// \typedef element
/// \brief   Element of struct element
///
/// \details (local for selfie_tools)
///
typedef struct element element;

/// \struct element
/// \brief   Chained list of strings
///
/// \details (local for selfie_tools)
///
struct element
{
  char *chaine;		///< String
  struct element *next; ///< Pointer to next element
};

/// \brief   First element / Chained list
///
/// \details (local for selfie_tools)
///
typedef element *selfie_llist;

/// \brief     Add an element to the end of chained list
///
/// \param[in] list Chained list
/// \param[in] val Value of the element to add
/// \return    New chained list
///
/// \details   (local for selfie_tools)
///
selfie_llist selfie_llist_add_to_end(selfie_llist list, char *val)
{
  element *new_elem = malloc(sizeof(element));
  new_elem->chaine = val;
  new_elem->next = NULL;
  if (list == NULL)
  {
    return new_elem;
  }
  else
  {
    element *temp = list;
    while (temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = new_elem;
    return list;
  }
};

/// \brief     Print to stdout the list
///
/// \param[in] liste Chained list
///
/// \details   (local for selfie_tools)
///
void selfie_llist_print(selfie_llist liste)
{
  element *tmp = liste;
  while (tmp != NULL)
  {
    printf("%s ", tmp->chaine);
    tmp = tmp->next;
  }
};

/// \brief     Get number of element of a chained list
///
/// \param[in] liste Chained list
/// \return    Number of element of the chained list
///
/// \details   (local for selfie_tools)
///
int selfie_llist_count(selfie_llist liste)
{
  if (liste == NULL)
    return 0;
  return selfie_llist_count(liste->next) + 1;
};

/// \brief         Release memory resources of a chained list
///
/// \param[in,out] list Chained list
/// \return        exit code
///
/// \details       (local for selfie_tools)
///
int selfie_llist_free(selfie_llist list)
{
  if (list == NULL)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    element *temp = list;
    element *temp_ = NULL;
    while (temp->next != NULL)
    {
      temp_ = temp;
      temp = temp->next;
      free(temp_->chaine);
      free(temp_);
    }
    free(temp->chaine);
    free(temp);
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
};

  // YAML parsing configuration file

#include <yaml.h>

// Read configuration file

/// \brief       Parse yaml configuration file
///
/// \param[in]   filename String containing the name of configuration file
/// \param[out]  in params_in
/// \return      exit code
///
/// \details
int selfie_yaml_read(char *filename, params_in *in)
{
  yaml_parser_t parser;
  yaml_event_t event;
  FILE *fh = NULL;
  int done = 0;
  int error = 0;
  int i = 0;
  char *tmp_string = NULL;
  // Flags for yaml triggers
  int time_limit_flag = 0;
  int env_vars_flag = 0;
  int exclude_commands_flag = 0;
  // Chained lists for reading list in configuration file
  selfie_llist l_env_vars = NULL;
  selfie_llist l_exclude_commands = NULL;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  fh = fopen(filename, "r");
  if (fh == NULL)
  {
#ifdef HAVE_DEBUG
    PINFO("Error in opening configuration file");
#endif
    return EXIT_FAILURE;
  }

  // Initialize parser
  if (!yaml_parser_initialize(&parser))
  {
#ifdef HAVE_DEBUG
    PINFO("Error libyaml");
#endif
    fclose(fh);
    return EXIT_FAILURE;
  }

  // Set input file
  yaml_parser_set_input_file(&parser, fh);

  // Parsing
  while (!done)
  {
    if (!yaml_parser_parse(&parser, &event))
    {
#ifdef HAVE_DEBUG
      PINFO("Error libyaml");
#endif
      error = 1;
      break;
    }

    switch (event.type)
    {
    case YAML_STREAM_END_EVENT:
      done = 1;
      break;
    case YAML_SCALAR_EVENT:
      if (time_limit_flag == 1)
      {
	in->time_tol = atof(event.data.scalar.value);
	time_limit_flag = 0;
      }
      else if (env_vars_flag == 1)
      {
	l_env_vars = selfie_llist_add_to_end(l_env_vars,
					     strdup(event.data.scalar.value));
      }
      else if (exclude_commands_flag == 1)
      {
	l_exclude_commands = selfie_llist_add_to_end(
	    l_exclude_commands, strdup(event.data.scalar.value));
      }
      else if (strcmp(event.data.scalar.value, "time_limit") == 0)
      {
	time_limit_flag = 1;
      }
      else if (strcmp(event.data.scalar.value, "env_vars") == 0)
      {
	env_vars_flag = 1;
      }
      else if (strcmp(event.data.scalar.value, "exclude_commands") == 0)
      {
	exclude_commands_flag = 1;
      }
      break;
    case YAML_SEQUENCE_END_EVENT:
      if (env_vars_flag == 1)
      {
	env_vars_flag = 0;
      }
      else if (exclude_commands_flag == 1)
      {
	exclude_commands_flag = 0;
      }
      break;
    default:
      break;
    }
    yaml_event_delete(&event);
  }

  // Cleanup
  yaml_parser_delete(&parser);

  fclose(fh);

  // Convert chained list to arrays
  if (selfie_llist_count(l_env_vars) > 0)
  {
    in->nb_env_vars = selfie_llist_count(l_env_vars);
    in->env_vars = (char **)malloc(in->nb_env_vars * sizeof(char *));
    in->env_vars_values = (char **)malloc(in->nb_env_vars * sizeof(char *));
    i = 0;
    element *tmp = l_env_vars;
    while (tmp->next != NULL)
    {
      in->env_vars[i] = strdup(tmp->chaine);
      tmp_string = getenv(in->env_vars[i]);
      if (tmp_string != NULL)
      {
	in->env_vars_values[i] = strdup(tmp_string);
      }
      else
      {
	in->env_vars_values[i] = strdup("");
      }
      i++;
      tmp = tmp->next;
    }
    in->env_vars[i] = strdup(tmp->chaine);
    tmp_string = getenv(in->env_vars[i]);
    if (tmp_string != NULL)
    {
      in->env_vars_values[i] = strdup(tmp_string);
    }
    else
    {
      in->env_vars_values[i] = strdup("");
    }
  }
  if (selfie_llist_count(l_exclude_commands) > 0)
  {
    in->nb_exclude_commands = selfie_llist_count(l_exclude_commands);
    in->exclude_commands =
	(char **)malloc(in->nb_exclude_commands * sizeof(char *));
    i = 0;
    element *tmp = l_exclude_commands;
    while (tmp->next != NULL)
    {
      in->exclude_commands[i] = strdup(tmp->chaine);
      i++;
      tmp = tmp->next;
    }
    in->exclude_commands[i] = strdup(tmp->chaine);
  }
  selfie_llist_free(l_env_vars);
  selfie_llist_free(l_exclude_commands);

  if (error != 0)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
};

// Read configuration file

/// \brief       Get the configuration file name
///
/// \param[out]  s String containing the name of configuration file
/// \return      exit code
///
/// \details     The function test in order:
///                 - General configuration file
///                 - User home directory configuration file
///                 - environment variable
///              The last is the prioritary
///              If none of these files are found, then it returns an string
///              of 0 length
///
int selfie_get_conf_filename(char *s)
{
  struct stat st;
  char filename_tmp[MAX_FILENAME] = "";
  char *tmp_string = NULL;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  s[0] = 0;

  strncpy(filename_tmp, CONFIGFILEPATH, MAX_FILENAME);
  strncat(filename_tmp, "/", MAX_FILENAME - strlen(filename_tmp) + 1);
  strncat(filename_tmp, CONFIGFILENAME,
	  MAX_FILENAME - strlen(filename_tmp) + 1);
  if (!stat(filename_tmp, &st))
  {
    strncpy(s, filename_tmp, MAX_FILENAME);
  }
  tmp_string = getenv("HOME");
  if (tmp_string != NULL)
  {
    filename_tmp[MAX_FILENAME - 1] = '\0';
    strncpy(filename_tmp, tmp_string, MAX_FILENAME - 1);
    strncat(filename_tmp, "/.", MAX_FILENAME - strlen(filename_tmp) + 1);
    strncat(filename_tmp, CONFIGFILENAME,
	    MAX_FILENAME - strlen(filename_tmp) + 1);
    if (!stat(filename_tmp, &st))
    {
      strncpy(s, filename_tmp, MAX_FILENAME);
    }
    tmp_string = NULL;
  }
  tmp_string = getenv(ENVCONFIGFILE);
  if (tmp_string != NULL)
  {
    strncpy(filename_tmp, tmp_string, MAX_FILENAME - strlen(filename_tmp) + 1);
    if (!stat(filename_tmp, &st))
    {
      strncpy(s, filename_tmp, MAX_FILENAME);
    }
    tmp_string = NULL;
  }
  return EXIT_SUCCESS;
}

/// \details
int selfie_read_config_file(params_in *in)
{
  char filename[MAX_FILENAME] = "";
  int error = 0;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  selfie_get_conf_filename(filename);

  if (strlen(filename) > 0)
  {
    error = selfie_yaml_read(filename, in);
    if (error != EXIT_SUCCESS)
    {
      selfie_free_params_in(in);
      selfie_alloc_params_in(in);
    }
  }

  return EXIT_SUCCESS;
};

// Read environment variables

/// \details
int selfie_read_env_vars(params_in *in)
{
  char *list[] = ENVVARS_RUNTIME;
  char *tmp_string = NULL;
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  // print log
  tmp_string = getenv(list[0]);
  if (tmp_string != NULL)
  {
    in->log_print = atoi(tmp_string);
  }
  // get output file
  tmp_string = getenv(list[1]);
  if (tmp_string != NULL)
  {
    in->outputfile = strdup(tmp_string);
  }
  else
  {
    in->outputfile = NULL;
  }
  return EXIT_SUCCESS;
};

/// \details
int selfie_getenv(char const *env)
{
  int enable = 0;
  char *tmp_string = NULL;
  tmp_string = getenv(env);
  if (tmp_string != NULL)
  {
    enable = !atoi(tmp_string);
  }
  else
  {
    enable = 1;
  }
  return enable;
}

/// \detail
int selfie_check_exclude(params_in *in)
{
  int i = 0;
  int err = 0;
  regex_t preg;

  // Check exclude commands
  in->enable = 1;
  if (in->cmdline != NULL)
    for (i = 0; i < in->nb_exclude_commands; i++)
    {
      err = regcomp(&preg, in->exclude_commands[i], REG_NOSUB | REG_EXTENDED);
      if (err) {
        continue;
      }
      if (regexec(&preg, in->cmdline, 0, NULL, 0) == 0)
      {
	in->enable = 0;
      }
      // if(!strcmp(in->cmdline,in->exclude_commands[i]))
      regfree(&preg);
    }
  return in->enable;
}

// /// \detail  replace all occurrence of "str" with "rep" in "src"
// void strreplace(char *src, char *str, char *rep)
// {
//   char *p = strstr(src, str);
//   do
//   {
//     if (p)
//     {
//       char buf[1024];
//       memset(buf, '\0', strlen(buf));

//       if (src == p)
//       {
// 	strcpy(buf, rep);
// 	strcat(buf, p + strlen(str));
//       }
//       else
//       {
// 	strncpy(buf, src, strlen(src) - strlen(p));
// 	strcat(buf, rep);
// 	strcat(buf, p + strlen(str));
//       }

//       memset(src, '\0', strlen(src));
//       strcpy(src, buf);
//     }

//   } while (p && (p = strstr(src, str)));
// }

// Writing logs

/// \details
int selfie_write_outputfile(char *filename, char *outlog)
{
  FILE *f_output = NULL;

  f_output = fopen(filename, "a");
  if (f_output != NULL)
  {
    (void)fprintf(f_output, "%s\n", outlog);
    (void)fclose(f_output);
  }

  return EXIT_SUCCESS;
}

/// \details
int selfie_write_log(params_in *in, params_out *out)
{
  char *json_string = NULL;
  int len = 0;
  int i = 0;
  int display = 1;
#ifdef HAVE_DEBUG
  PINFO("");
#endif

  selfie_check_exclude(in);
  if (in->enable == 0)
    display = 0;

  // If execution time is greater than configuration file
  if (out->wtime < in->time_tol)
    display = 0;

  if (display == 1)
  {
    // Add env_vars to log
    for (i = 0; i < in->nb_env_vars; i++)
    {
      if (strlen(in->env_vars_values[i]) > 0)
      {
	selfie_json_string_to_log(out, in->env_vars[i], in->env_vars_values[i]);
      }
    }

    // Add timestamp
    selfie_json_llu_to_log(out, "timestamp", (unsigned long long)time(NULL));

    // Add wtime
    selfie_json_double_to_log(out, "wtime", out->wtime);

    // Add command line
    selfie_json_string_to_log(out, "command", in->cmdline);

    // JSON string
    len = strlen(out->log_string);
    json_string = (char *)malloc((len + 32) * sizeof(char));

    strcpy(json_string, "{ ");
    strcat(json_string, out->log_string);
    if (json_string[strlen(json_string) - 2] == ',')
    {
      json_string[strlen(json_string) - 2] = 0;
    }

    strcat(json_string, " }");

    // Syslog
    if (in->log_print != 0)
    {
      openlog("selfie", LOG_PERROR | LOG_PID | LOG_NDELAY, LOG_USER);
    }
    else
    {
      openlog("selfie", LOG_PID | LOG_NDELAY, LOG_USER);
    }
    syslog(LOG_INFO, json_string);
    closelog();

    // Outputfile

    if (in->outputfile != NULL)
    {
      selfie_write_outputfile(in->outputfile, json_string);
    }

    free(json_string);
  }
  return EXIT_SUCCESS;
};

/// \details
int selfie_write_output(params_in *in, params_out *out)
{
  int display = 1;
  int i = 0;
  char output[OUTPUT_ROWMAX] = "";
#ifdef HAVE_DEBUG
  PINFO("");
#endif
  output[OUTPUT_ROWMAX - 1] = '\0';

  // Check environment variable for displaying
  display = !selfie_getenv("SELFIE_PRINT");

  // If in exclude commands
  selfie_check_exclude(in);
  if (in->enable == 0)
    display = 0;

  // If execution time is greater than configuration file
  if (out->wtime < in->time_tol)
    display = 0;

  // Check environment variable for displaying
  display = !selfie_getenv("SELFIE_PRINT_");

  if (display != 0)
  {
    snprintf(output, OUTPUT_ROWMAX - 1, "\n=== SELFIE DETAILS ===\n\n");
    selfie_strcat_output_to_params_out(out, output);

    // Execution time
    snprintf(output, OUTPUT_ROWMAX - 1, "   - Execution time : %lf\n",
	     out->wtime);
    selfie_strcat_output_to_params_out(out, output);

    // Command line
    snprintf(output, OUTPUT_ROWMAX - 1, "   - Command : %s\n", in->cmdline);
    selfie_strcat_output_to_params_out(out, output);

    // Add env_vars to output
    snprintf(output, OUTPUT_ROWMAX - 1, "   - Environment variables -\n\n");
    selfie_strcat_output_to_params_out(out, output);
    for (i = 0; i < in->nb_env_vars; i++)
    {
      if (strlen(in->env_vars_values[i]) > 0)
      {
	snprintf(output, OUTPUT_ROWMAX - 1, "       %16s : %s\n",
		 in->env_vars[i], in->env_vars_values[i]);
	selfie_strcat_output_to_params_out(out, output);
      }
    }

    // Print to output
    fprintf(stderr, "%s\n", out->output_string);
  }
  return EXIT_SUCCESS;
};
