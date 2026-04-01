/******************************************************************************
*                                                                             *
* -------------------------------- queue.h ---------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

/******************************************************************************
*                                                                             *
* A queue is a singly-linked list restricted to FIFO access.  The List       *
* struct is reused directly — no additional storage is needed.               *
*                                                                             *
******************************************************************************/

typedef List Queue;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

#define queue_init(queue, destroy)    list_init(queue, destroy)

#define queue_destroy(queue)          list_destroy(queue)

int queue_enqueue(Queue *queue, const void *data);

int queue_dequeue(Queue *queue, void **data);

#define queue_peek(queue)             list_data(list_head(queue))

#define queue_size(queue)             list_size(queue)

#define queue_is_empty(queue)         (list_size(queue) == 0)

#endif /* QUEUE_H */
