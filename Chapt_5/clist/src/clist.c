/******************************************************************************
*                                                                             *
* -------------------------------- clist.c ---------------------------------- *
*                                                                             *
******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "clist.h"


/******************************************************************************
*                                                                             *
* Default allocator wrappers (system malloc / free).                         *
*                                                                             *
******************************************************************************/

static void *default_alloc(void *ctx) {
  (void)ctx;
  return malloc(sizeof(CListElmt));
}

static void default_free(void *ctx, void *elmt) {
  (void)ctx;
  free(elmt);
}


/******************************************************************************
*                                                                             *
* ------------------------------ clist_init --------------------------------- *
*                                                                             *
******************************************************************************/
void clist_init(CList *list, void (*destroy)(void *data)) {

  list->size    = 0;
  list->destroy = destroy;
  list->match   = NULL;
  list->head    = NULL;

  list->allocator_ctx = NULL;
  list->elem_alloc    = default_alloc;
  list->elem_free     = default_free;

}


/******************************************************************************
*                                                                             *
* ----------------------- clist_init_with_allocator ------------------------- *
*                                                                             *
******************************************************************************/
void clist_init_with_allocator(CList *list, void (*destroy)(void *data),
                               void *ctx,
                               void *(*alloc_fn)(void *ctx),
                               void  (*free_fn)(void *ctx, void *elmt)) {

  clist_init(list, destroy);

  list->allocator_ctx = ctx;
  list->elem_alloc    = alloc_fn;
  list->elem_free     = free_fn;

}


/******************************************************************************
*                                                                             *
* ----------------------------- clist_destroy ------------------------------- *
*                                                                             *
******************************************************************************/
void clist_destroy(CList *list) {

  void *data;

  /* Remove each element */

  while (clist_size(list) > 0) {

    if (clist_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
      /* Call user-defined function to free allocated data */

      list->destroy(data);
    }
  }

  /* Clear the structure as a precaution */

  memset(list, 0, sizeof(CList));

}


/******************************************************************************
*                                                                             *
* -------------------------- clist_insert_next ------------------------------ *
*                                                                             *
******************************************************************************/
int clist_insert_next(CList *list, CListElmt *element, const void *data) {
  CListElmt *new_element;

  if (list == NULL) {
    return -1;
  }

  /* Allocate storage for the element */

  if ((new_element = (CListElmt *)list->elem_alloc(list->allocator_ctx)) == NULL) {
    return -1;
  }

  new_element->data = (void *)data;

  if (clist_size(list) == 0) {
    /* First element — self-referential; element must be NULL */

    new_element->next = new_element;
    list->head = new_element;
  }
  else {
    /* Insert after element — element must not be NULL */

    if (element == NULL) {
      list->elem_free(list->allocator_ctx, new_element);
      return -1;
    }

    new_element->next = element->next;
    element->next = new_element;
  }

  list->size++;

  return 0;

}


/******************************************************************************
*                                                                             *
* ---------------------------- clist_rem_next ------------------------------- *
*                                                                             *
******************************************************************************/
int clist_rem_next(CList *list, CListElmt *element, void **data) {
  CListElmt *old_element;

  if (list == NULL) {
    return -1;
  }

  /* Do not allow removal from an empty list */

  if (clist_size(list) == 0) {
    return -1;
  }

  if (element == NULL) {
    /* Handle removal of the head element */

    *data = list->head->data;
    old_element = list->head;

    if (list->size == 1) {
      list->head = NULL;
    }
    else {
      /* Find the tail to re-link it to the new head */

      CListElmt *tail = list->head;
      while (tail->next != list->head) {
        tail = tail->next;
      }

      tail->next = list->head->next;
      list->head = list->head->next;
    }
  }
  else {
    /* Handle removal of element->next */

    *data = element->next->data;
    old_element = element->next;
    element->next = old_element->next;

    if (old_element == list->head) {
      list->head = (list->size == 1) ? NULL : element->next;
    }
  }

  list->elem_free(list->allocator_ctx, old_element);

  list->size--;

  return 0;

}
