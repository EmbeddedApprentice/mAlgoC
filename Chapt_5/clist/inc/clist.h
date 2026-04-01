/******************************************************************************
*                                                                             *
* -------------------------------- clist.h ---------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef CLIST_H
#define CLIST_H

#include <stdlib.h>

/******************************************************************************
*                                                                             *
* Define a structure for circular list elements.                              *
*                                                                             *
******************************************************************************/

typedef struct CListElmt_ {
  void  *data;
  struct CListElmt_  *next;
} CListElmt;

/******************************************************************************
*                                                                             *
* Define a structure for circular linked lists.                               *
*                                                                             *
* elem_alloc / elem_free are pluggable node allocators.  By default they      *
* wrap malloc / free.  Supply custom callbacks via clist_init_with_allocator  *
* to use a static element pool or any other backing store.                    *
*                                                                             *
* Callback signatures:                                                        *
*   void *elem_alloc(void *ctx)            – return a CListElmt-sized chunk   *
*   void  elem_free (void *ctx, void *elmt)– return the chunk to its source   *
*                                                                             *
******************************************************************************/

typedef struct CList_ {
  int size;

  int (*match) (const void *key1, const void *key2);
  void  (*destroy)(void *data);

  void  *allocator_ctx;
  void  *(*elem_alloc)(void *ctx);
  void   (*elem_free)(void *ctx, void *elmt);

  CListElmt  *head;

} CList;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

/* Init with system malloc/free as the node allocator. */
void clist_init(CList *list, void (*destroy)(void *data));

/* Init with a custom node allocator.
 * ctx is passed through to every alloc/free call.
 * alloc(ctx) must return sizeof(CListElmt) bytes or NULL on failure.
 * free(ctx, elmt) returns the node to the allocator. */
void clist_init_with_allocator(CList *list, void (*destroy)(void *data),
                               void *ctx,
                               void *(*alloc_fn)(void *ctx),
                               void  (*free_fn)(void *ctx, void *elmt));

void clist_destroy(CList *list);

int clist_insert_next(CList *list, CListElmt *element, const void *data);

int clist_rem_next(CList *list, CListElmt *element, void **data);

static inline int clist_size(const CList *list) { return list ? list->size : 0; }

#define clist_head(list)               ((list)->head)

#define clist_is_head(list, element)   ((element) == (list)->head ? 1 : 0)

#define clist_data(element)            ((element)->data)

#define clist_next(element)            ((element)->next)

#endif /* CLIST_H */
