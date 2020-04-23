#include "selfie_mio_test.h"
#include <stdio.h>
#include <stdlib.h>
/*
void mio_op_init(struct mio_op *op);
void mio_op_fini(struct mio_op *op);
struct mio_op* mio_op_alloc_init();
void mio_op_fini_free(struct mio_op *op);
int mio_op_poll(struct mio_pollop *ops, int nr_ops, uint64_t timeout);
void mio_op_callbacks_set(struct mio_op *op, mio_callback cb_complete, mio_callback cb_failed, void *cb_data);
int mio_obj_open(const struct mio_obj_id *oid, struct mio_obj *obj, struct mio_op *op);
void mio_obj_close(struct mio_obj *obj);
int mio_obj_create(const struct mio_obj_id *oid, const struct mio_pool *pool_id, struct mio_obj *obj, struct mio_op *op);
int mio_obj_delete(const struct mio_obj_id *oid, struct mio_op *op);
int mio_obj_writev(struct mio_obj *obj, const struct mio_iovec *iov, int iovcnt, struct mio_op *op);
int mio_obj_readv(struct mio_obj *obj, const struct mio_iovec *iov, int iovcnt, struct mio_op *op);
int mio_obj_sync(struct mio_obj *obj, struct mio_op *op);
int mio_obj_size(struct mio_obj *obj, struct mio_op *op);
int mio_kvs_pair_get(struct mio_kvs_id *kvs_id, int nr_kvps, struct mio_kv_pair *kvps, int32_t *rcs, struct mio_op *op);
int mio_kvs_pair_put(struct mio_kvs_id *kvs_id, int nr_kvps, struct mio_kv_pair *kvps, int32_t *rcs, struct mio_op *op);
int mio_kvs_pair_del(struct mio_kvs_id *kvs_id, int nr_kvps, struct mio_kv_pair *kvps, int32_t *rcs, struct mio_op *op); 
int mio_kvs_create_set(struct mio_kvs_id *kvs_id, struct mio_op *op);
int mio_kvs_del_set(struct mio_kvs_id *kvs_id, struct mio_op *op);
int mio_obj_hints_set(struct mio_obj *obj, struct mio_hints *hints);
int mio_obj_hints_get(struct mio_obj *obj, struct mio_hints *hints);
int mio_hints_init(struct mio_hints *hints);
void mio_hints_fini(struct mio_hints *hints);
int mio_hint_add(struct mio_hints *hints, int hint_key, uint64_t hint_value);
int mio_hint_lookup(struct mio_hints *hints, int hint_key, uint64_t *hint_value);
enum mio_hint_type mio_hint_type(enum mio_hint_key key);
char* mio_hint_name(enum mio_hint_key key);
int mio_composite_obj_create(const struct mio_obj_id *oid, struct mio_obj *obj, struct mio_op *op);
int mio_composite_obj_del(const struct mio_obj_id *oid, struct mio_op *op);
int mio_composite_obj_add_layers(struct mio_obj *obj, int nr_layers, struct mio_comp_obj_layer *layers, struct mio_op *op);
int mio_composite_obj_del_layers(struct mio_obj *obj, int nr_layers_to_del, struct mio_comp_obj_layer *layers_to_del, struct mio_op *op);
int mio_composite_obj_list_layers(struct mio_obj *obj, struct mio_comp_obj_layout *ret_layout, struct mio_op *op);
int mio_composite_obj_add_extents(struct mio_obj *obj, struct mio_obj_id *layer_id, int nr_exts, struct mio_obj_ext *exts, struct mio_op *op);
int mio_composite_obj_del_extents(struct mio_obj *obj, struct mio_obj_id *layer_id, int nr_exts, struct mio_obj_ext *exts, struct mio_op *op);
int mio_composite_obj_get_extents(struct mio_obj *obj, struct mio_obj_id *layer_id, off_t offset, int nr_exts, struct mio_obj_ext *exts, int *nr_ret_exts, struct mio_op *op);
void mio_fini();
int mio_thread_init(struct mio_thread *thread);
void mio_thread_fini(struct mio_thread *thread);
*/
int mio_init(const char *yaml_conf) { printf("%s : OK\n", __FUNCTION__); return EXIT_SUCCESS;};
