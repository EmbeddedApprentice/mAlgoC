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
* Define a strcture for linked lists.                                 *
*                                                                             *
******************************************************************************/

typedef struct List_ {
  int size;

  int (*match) (const void *key1, const void *key2);
  void  (*destroy)(void *data);

  ListElmt  *head;
  ListElmt  *tail;
} List;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

void list_init(List *list, void (*destroy)(void *data));

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
