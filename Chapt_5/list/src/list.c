/******************************************************************************
*                                                                             *
* -------------------------------- list.c ----------------------------------- *
*                                                                             *
******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "list.h"


/******************************************************************************
*                                                                             *
* Default allocator wrappers (system malloc / free).                         *
*                                                                             *
******************************************************************************/

static void *default_alloc(void *ctx) {
  (void)ctx;
  return malloc(sizeof(ListElmt));
}

static void default_free(void *ctx, void *elmt) {
  (void)ctx;
  free(elmt);
}


/******************************************************************************
*                                                                             *
* ------------------------------ list_init ---------------------------------- *
*                                                                             *
******************************************************************************/
void list_init(List *list, void (*destroy)(void *data)){
  /* Initialize the list */

  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
  list->match = NULL;

  list->allocator_ctx = NULL;
  list->elem_alloc    = default_alloc;
  list->elem_free     = default_free;

  return;

}


/******************************************************************************
*                                                                             *
* ----------------------- list_init_with_allocator -------------------------- *
*                                                                             *
******************************************************************************/
void list_init_with_allocator(List *list, void (*destroy)(void *data),
                              void *ctx,
                              void *(*alloc_fn)(void *ctx),
                              void  (*free_fn)(void *ctx, void *elmt)) {

  list_init(list, destroy);

  list->allocator_ctx = ctx;
  list->elem_alloc    = alloc_fn;
  list->elem_free     = free_fn;

}


/******************************************************************************
*                                                                             *
* ------------------------------ list_destroy ---------------------------------- *
*                                                                             *
******************************************************************************/
void list_destroy(List *list){

  void *data;

  /* Remove each element */

  while (list_size(list) > 0) {

    if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL){
      /* Call user defined function to free allocated data structure */

      list->destroy(data);
    }
  }

  /* clear the stucture as a precaution */

  memset(list, 0, sizeof(List));

}

/******************************************************************************
*                                                                             *
* -------------------------- list_insert_next ------------------------------- *
*                                                                             *
******************************************************************************/
int list_insert_next(List *list, ListElmt *element, const void *data){
  ListElmt  *new_element;

  if (list == NULL) {
    return -1;
  }

  /* Allocate storage for the element */

  if ((new_element = (ListElmt *)list->elem_alloc(list->allocator_ctx)) == NULL){
    return -1;
  }

  /* Insert the element in to the list */

  new_element->data = (void *)data;

  if (element == NULL) {
    /* Handle insertion at the start of the list */

    if (list_size(list) == 0) {
      list->tail = new_element;
    }
    new_element->next = list->head;
    list->head = new_element;
  }
  else {
    /* Handle insertion not at the head */

    if (element->next == NULL) {
      list->tail = new_element;
    }

    new_element->next = element->next;
    element->next = new_element;

  }

  /* Adjust the size of the list */
  list->size++;

  return 0;

}

/******************************************************************************
*                                                                             *
* ------------------------------ list_rem_next ------------------------------ *
*                                                                             *
******************************************************************************/
int list_rem_next(List *list, ListElmt *element, void **data) {
  ListElmt  *old_element;

  if (list == NULL) {
    return -1;
  }

  /* Do not allow removal from an empty list */

  if (list_size(list) == 0) {
    return -1;
  }

  /* Remove element from the list */

  if (element == NULL) {
    /* Handle Removal from the head of the list */

    *data = list->head->data;
    old_element = list->head;
    list->head = old_element->next;

    if (list_size(list) == 1) {
      list->tail = NULL;
    }
  }
  else {
    /* Handle removal from someplace other than the head */
     if (element->next == NULL) {
      return -1;
    }

    *data = element->next->data;
    old_element = element->next;
    element->next = old_element->next;

    if (element->next == NULL) {
      list->tail = element;
    }

  }

  list->elem_free(list->allocator_ctx, old_element);

  /* Adjust the size of the list */
  list->size--;

  return 0;


}


/******************************************************************************
*                                                                             *
* ========================== ListElmtPool =================================== *
*                                                                             *
******************************************************************************/


/******************************************************************************
*                                                                             *
* ----------------------- list_elmt_pool_init ------------------------------- *
*                                                                             *
******************************************************************************/
void list_elmt_pool_init(ListElmtPool *pool) {

  /* Walk storage in reverse so storage[0] ends up at the head. */

  pool->free_list = NULL;

  for (int i = LIST_ELMT_POOL_CAPACITY - 1; i >= 0; i--) {
    pool->storage[i].next = pool->free_list;
    pool->free_list = &pool->storage[i];
  }

}


/******************************************************************************
*                                                                             *
* ----------------------- list_elmt_pool_alloc ------------------------------ *
*                                                                             *
******************************************************************************/
void *list_elmt_pool_alloc(void *ctx) {

  ListElmtPool *pool = (ListElmtPool *)ctx;

  if (pool->free_list == NULL) {
    return NULL;
  }

  ListElmt *elmt  = pool->free_list;
  pool->free_list = elmt->next;
  elmt->next      = NULL;

  return elmt;

}


/******************************************************************************
*                                                                             *
* ----------------------- list_elmt_pool_free ------------------------------- *
*                                                                             *
******************************************************************************/
void list_elmt_pool_free(void *ctx, void *elmt) {

  ListElmtPool *pool  = (ListElmtPool *)ctx;
  ListElmt     *node  = (ListElmt *)elmt;

  node->next      = pool->free_list;
  pool->free_list = node;

}
