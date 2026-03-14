/******************************************************************************
*                                                                             *
* -------------------------------- dlist.c ----------------------------------- *
*                                                                             *
******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "dlist.h"


/******************************************************************************
*                                                                             *
* ------------------------------ dlist_init ---------------------------------- *
*                                                                             *
******************************************************************************/
void dlist_init(dList *list, void (*destroy)(void *data)){
  /* Initialize the dlist */

  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;

  return;

}

/******************************************************************************
*                                                                             *
* ------------------------------ dlist_destroy ---------------------------------- *
*                                                                             *
******************************************************************************/
void dlist_destroy(dList *list){

  void *data;

  /* Remove each element */

  while (dlist_size(list) > 0) {

    if (dlist_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL){
      /* Call user defined function to free allocated data structure */

      list->destroy(data);
    }
  }

  /* clear the stucture as a precaution */

  memset(list, 0, sizeof(dList));

}

/******************************************************************************
*                                                                             *
* -------------------------- dlist_insert_next ------------------------------- *
*                                                                             *
******************************************************************************/
int dlist_insert_next(dList *list, dListElmt *element, const void *data){
  dListElmt  *new_element;

  if (list == NULL) {
    return -1;
  }

  /* Allocate storage for the element */

  if ((new_element = (dListElmt *)malloc (sizeof(dListElmt))) == NULL){
    return -1;
  }

  /* Insert the element in to the dlist */

  new_element->data = (void *)data;

  if (element == NULL) {
    /* Handle insertion at the start of the dlist */

    if (dlist_size(list) == 0) {
      list->tail = new_element;
    }
    new_element->next = list->head;
    new_element->prev = NULL;

    if (list->head != NULL) {
      list->head->prev = new_element;
    }

    list->head = new_element;
  }
  else {
    /* Handle insertion not at the head */

    if (element->next == NULL) {
      list->tail = new_element;
    }

    new_element->next = element->next;
    new_element->prev = element;

    if (element->next != NULL) {
      element->next->prev = new_element;
    }

    element->next = new_element;

  }

  /* Adjust the size of the dlist */
  list->size++;

  return 0;

}

/******************************************************************************
*                                                                             *
* ------------------------------ dlist_rem_next ------------------------------ *
*                                                                             *
******************************************************************************/
int dlist_rem_next(dList *list, dListElmt *element, void **data) {
  dListElmt  *old_element;

  if (list == NULL) {
    return -1;
  }

  /* Do not allow removal from an empty dlist */

  if (dlist_size(list) == 0) {
    return -1;
  }

  /* Remove element from the dlist */

  if (element == NULL) {
    /* Handle Removal from the head of the dlist */

    *data = list->head->data;
    old_element = list->head;
    list->head = old_element->next;

    if (list->head != NULL) {
      list->head->prev = NULL;
    }

    if (dlist_size(list) == 1) {
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

    if (element->next != NULL) {
      element->next->prev = element;
    }

    if (element->next == NULL) {
      list->tail = element;
    }

  }

  free(old_element);

  /* Adjust the size of the dlist */
  list->size--;

  return 0;


}
