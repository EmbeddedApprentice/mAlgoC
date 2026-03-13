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
* ------------------------------ list_init ---------------------------------- *
*                                                                             *
******************************************************************************/
void list_init(List *list, void (*destroy)(void *data)){
  /* Initialize the list */

  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;

  return;

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

  if ((new_element = (ListElmt *)malloc (sizeof(ListElmt))) == NULL){
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

  free(old_element);

  /* Adjust the size of the list */
  list->size--;

  return 0;


}
