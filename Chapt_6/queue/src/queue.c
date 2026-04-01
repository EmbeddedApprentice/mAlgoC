/******************************************************************************
*                                                                             *
* -------------------------------- queue.c ---------------------------------- *
*                                                                             *
******************************************************************************/

#include "queue.h"


/******************************************************************************
*                                                                             *
* ----------------------------- queue_enqueue ------------------------------- *
*                                                                             *
******************************************************************************/
int queue_enqueue(Queue *queue, const void *data) {

  if (queue == NULL) {
    return -1;
  }

  /* Insert after tail so elements dequeue in the order they were enqueued.   *
   * When the queue is empty list_tail returns NULL, which causes              *
   * list_insert_next to insert at the head — correct for the first element.  */

  return list_insert_next(queue, list_tail(queue), data);

}


/******************************************************************************
*                                                                             *
* ----------------------------- queue_dequeue ------------------------------- *
*                                                                             *
******************************************************************************/
int queue_dequeue(Queue *queue, void **data) {

  return list_rem_next(queue, NULL, data);

}
