/******************************************************************************
*                                                                             *
* -------------------------------- dlist.h ----------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>

/******************************************************************************
*                                                                             *
* Define a strcture for linked dlist elements.                                 *
*                                                                             *
******************************************************************************/

typedef struct dListElmt_ {
  void  *data;
  struct dListElmt_  *next;
  struct dListElmt_  *prev;
} dListElmt;

/******************************************************************************
*                                                                             *
* Define a strcture for linked dlists.                                 *
*                                                                             *
******************************************************************************/

typedef struct dList_ {
  int size;

  int (*match) (const void *key1, const void *key2);
  void  (*destroy)(void *data);

  dListElmt  *head;
  dListElmt  *tail;
} dList;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

void dlist_init(dList *dlist, void (*destroy)(void *data));

void dlist_destroy(dList *list);

int dlist_insert_next(dList *list, dListElmt *element, const void *data);

int dlist_rem_next(dList *list, dListElmt *element, void **data);

static inline int dlist_size(const dList *list) { return list ? list->size : 0; }

#define dlist_head(list) ((list)->head)

#define dlist_tail(list) ((list)->tail)

#define dlist_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define dlist_is_tail(list, element) ((element) == (list)->tail ? 1 : 0)

#define dlist_data(element) ((element)->data)

#define dlist_next(element) ((element)->next)

#define dlist_prev(element) ((element)->prev)

#endif /* DLIST_H */
