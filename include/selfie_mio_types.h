#ifndef __SELFIE_MIO_TYPES_H__
#define __SELFIE_MIO_TYPES_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct mio_op;

struct mio_hint_map
{
  int mhm_nr_entries;
  int mhm_nr_set;
  int *mhm_keys;
  uint64_t *mhm_values;
};

enum mio_log_level
{
  MIO_LOG_INVALID = -1,
  MIO_ERROR = 0,
  MIO_WARN = 1,
  MIO_INFO = 2,
  MIO_TRACE = 3,
  MIO_DEBUG = 4,
  MIO_MAX_LEVEL,
};

enum mio_driver_id
{
  MIO_DRIVER_INVALID = -1,
  MIO_MERO,
  MIO_DRIVER_NUM
};

typedef int (*mio_driver_op_postprocess)(struct mio_op *op);
struct mio_driver_op
{
  /**
   * The function and data for post-processing.
   */
  mio_driver_op_postprocess mdo_post_proc;
  void *mdo_post_proc_data;

  /** Pointers to driver specific op. */
  void *mdo_op;

  struct mio_driver_op *mdo_next;
};

struct mio_driver_op_chain
{
  struct mio_driver_op *mdoc_head;
};

typedef void (*mio_callback)(struct mio_op *op);
struct mio_kvs_id;

enum mio_obj_opcode
{
  MIO_OBJ_INVALID,
  MIO_OBJ_CREATE,
  MIO_OBJ_DELETE,
  MIO_OBJ_OPEN,
  MIO_OBJ_CLOSE,
  MIO_OBJ_SYNC,
  MIO_OBJ_ATTRS_SET,
  MIO_OBJ_ATTRS_GET,
  MIO_OBJ_READ,
  MIO_OBJ_WRITE,
  MIO_OBJ_OP_NR
};

enum mio_kvs_opcode
{
  MIO_KVS_CREATE_SET = MIO_OBJ_OP_NR + 1,
  MIO_KVS_DELETE_SET,
  /** Check a kvs for an existence. */
  MIO_KVS_LOOKUP_SET,
  /** Lookup a value with the given key. */
  MIO_KVS_GET,
  /** Insert or update the value, given a key. */
  MIO_KVS_PUT,
  /** Delete the value, if any, for the given key. */
  MIO_KVS_DEL,
  MIO_KVS_OP_NR
};

enum mio_composite_obj_opcode
{
  MIO_COMP_OBJ_CREATE = MIO_KVS_OP_NR + 1,
  MIO_COMP_OBJ_DELETE,
  MIO_COMP_OBJ_ADD_LAYERS,
  MIO_COMP_OBJ_DEL_LAYERS,
  MIO_COMP_OBJ_LIST_LAYERS,
  MIO_COMP_OBJ_ADD_EXTENTS,
  MIO_COMP_OBJ_DEL_EXTENTS,
  MIO_COMP_OBJ_GET_EXTENTS,
  MIO_COMP_OBJ_OP_NR
};

enum mio_op_state
{
  MIO_OP_ONFLY = 0,
  MIO_OP_COMPLETED,
  MIO_OP_FAILED
};

struct mio_op_app_cbs
{
  mio_callback moc_cb_complete;
  mio_callback moc_cb_failed;
  void *moc_cb_data;
};

struct mio_op
{
  unsigned int mop_opcode; /* Type of operation. */
  union {		   /* Which object or key-value set. */
    struct mio_obj *obj;
    struct mio_kvs_id *kvs_id;
  } mop_who;
  int mop_state; /* Current state of operation. */
  int mop_rc;    /* == 0 when operation successes,
		  *  < 0 the error code if the
		  *      operation fails. */

  struct mio_op_app_cbs mop_app_cbs;

  /* See mio_drv_op_chain in mio_inernal for explanation. */
  struct mio_driver_op_chain mop_drv_op_chain;

  struct mio_op_ops *mop_op_ops;
};

struct mio_pollop
{
  struct mio_op *mp_op; /* Operation to poll. */
  int mp_retstate;      /* Returned state. */
};

enum mio_hint_type
{
  MIO_HINT_SESSION,
  MIO_HINT_PERSISTENT
};

enum mio_hint_key
{
  MIO_HINT_OBJ_CACHE_FLUSH_SIZE,
  MIO_HINT_OBJ_LIFETIME,
  MIO_HINT_OBJ_WHERE,
  MIO_HINT_KEY_NUM
};

struct mio_hints
{
  struct mio_hint_map mh_map;
};

enum
{
  MIO_OBJ_ID_LEN = 16,
  MIO_KVS_ID_LEN = 16
};
struct mio_obj_id
{
  uint8_t moi_bytes[MIO_OBJ_ID_LEN];
};

struct mio_kvs_id
{
  uint8_t mki_bytes[MIO_KVS_ID_LEN];
};

struct mio_pool
{
  uint64_t mp_hi;
  uint64_t mp_lo;
};

struct mio_iovec
{
  char *miov_base; /* Base address. */
  off_t miov_off;  /* Offset. */
  size_t miov_len; /* Length. */
};

enum
{
  MIO_MAX_HINTS_PER_OBJ = 16,
};

struct mio_obj_attrs
{
  uint64_t moa_size;
  uint64_t moa_wtime;

  /* Persistent hints only. */
  struct mio_hints moa_phints;
};

struct mio_obj
{
  struct mio_obj_id mo_id;
  struct mio_obj_op *mo_op;

  /** Driver specific object ops. */
  struct mio_obj_ops *mo_drv_obj_ops;

  /** Hints (persistent + session hints) set for this object. */
  struct mio_hints mo_hints;

  /** Associated metadata key-vaule set. */
  struct mio_kvs *mo_md_kvs;

  struct mio_obj_attrs mo_attrs;

  /** Pointer to driver specific object structure. */
  void *mo_drv_obj;
};

struct mio_kv_pair
{
  void *mkp_key;
  size_t mkp_klen;
  void *mkp_val;
  size_t mkp_vlen;
};

struct mio_kvs
{
  struct mio_kvs_id mk_id;
  /** Driver specific key-value set data structure. */
  void *mk_drv_kvs;
  struct mio_kvs_ops *mk_ops;
};

extern struct mio_kvs mio_obj_attrs_kvs;

struct mio_obj_ext
{
  off_t moe_off;
  size_t moe_size;
};

struct mio_comp_obj_layer
{
  int mcol_priority;
  struct mio_obj_id mcol_oid;
};

struct mio_comp_obj_layout
{
  int mlo_nr_layers;
  struct mio_comp_obj_layer *mlo_layers;
};
struct mio
{
  enum mio_log_level m_log_level;
  char *m_log_file;

  enum mio_driver_id m_driver_id;
  struct mio_driver *m_driver;
  void *m_driver_confs;
};
extern struct mio *mio_instance;
#endif
