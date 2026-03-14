/******************************************************************************
*                                                                             *
* -------------------------------- list.h ----------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/******************************************************************************
*                                                                             *
* Define a strcture for linked list elements.                                 *
*                                                                             *
******************************************************************************/

typedef struct ListElmt_ {
  void  *data;
  struct ListElmt_  *next;
} ListElmt;

/******************************************************************************
*                                                                             *
* Define a strcture for linked lists.                                         *
*                                                                             *
* elem_alloc / elem_free are pluggable node allocators.  By default they     *
* wrap malloc / free.  Supply custom callbacks via list_init_with_allocator   *
* to use a static element pool or any other backing store.                    *
*                                                                             *
* Callback signatures:                                                        *
*   void *elem_alloc(void *ctx)            – return a ListElmt-sized chunk   *
*   void  elem_free (void *ctx, void *elmt)– return the chunk to its source  *
*                                                                             *
******************************************************************************/

typedef struct List_ {
  int size;

  int (*match) (const void *key1, const void *key2);
  void  (*destroy)(void *data);

  void  *allocator_ctx;
  void  *(*elem_alloc)(void *ctx);
  void   (*elem_free)(void *ctx, void *elmt);

  ListElmt  *head;
  ListElmt  *tail;
} List;

/******************************************************************************
*                                                                             *
* ListElmtPool — zero-heap node allocator for List.                          *
*                                                                             *
* Each ListElmt already embeds a `next` pointer; ListElmtPool reuses it as   *
* the free-list link so no extra storage is needed.  Declare one as a local  *
* or static variable and pass it as allocator_ctx.                            *
*                                                                             *
******************************************************************************/

#define LIST_ELMT_POOL_CAPACITY 32

typedef struct ListElmtPool_ {
  ListElmt  storage[LIST_ELMT_POOL_CAPACITY];
  ListElmt *free_list;
} ListElmtPool;

void  list_elmt_pool_init(ListElmtPool *pool);

/* Allocator callbacks — pass directly to list_init_with_allocator. */
void *list_elmt_pool_alloc(void *ctx);
void  list_elmt_pool_free(void *ctx, void *elmt);

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

/* Init with system malloc/free as the node allocator. */
void list_init(List *list, void (*destroy)(void *data));

/* Init with a custom node allocator.
 * ctx is passed through to every alloc/free call.
 * alloc(ctx) must return sizeof(ListElmt) bytes or NULL on failure.
 * free(ctx, elmt) returns the node to the allocator. */
void list_init_with_allocator(List *list, void (*destroy)(void *data),
                              void *ctx,
                              void *(*alloc_fn)(void *ctx),
                              void  (*free_fn)(void *ctx, void *elmt));

void list_destroy(List *list);

int list_insert_next(List *list, ListElmt *element, const void *data);

int list_rem_next(List *list, ListElmt *element, void **data);

static inline int list_size(const List *list) { return list ? list->size : 0; }

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define list_is_tail(list, element) ((element) == (list)->tail ? 1 : 0)

#define list_data(element) ((element)->data)

#define list_next(element) ((element)->next)

#endif /* LIST_H */
