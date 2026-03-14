/******************************************************************************
*                                                                             *
* ----------------------------- buffer_pool.c -------------------------------- *
*                                                                             *
******************************************************************************/

#include "buffer_pool.h"
#include <stddef.h>

/******************************************************************************
*                                                                             *
* -------------------------- buffer_pool_init -------------------------------- *
*                                                                             *
******************************************************************************/
void buffer_pool_init(BufferPool *pool) {

  /* Walk the storage array and link every buffer into the free list.
   * Iterate in reverse so that storage[0] ends up at the head — the
   * order doesn't matter functionally but keeps things predictable
   * for tests and debugging. */

  pool->free_list = NULL;

  for (int i = BUFFER_POOL_CAPACITY - 1; i >= 0; i--) {
    pool->storage[i].next = pool->free_list;
    pool->free_list = &pool->storage[i];
  }

  pool->free_count = BUFFER_POOL_CAPACITY;
  pool->min_free   = BUFFER_POOL_CAPACITY;

}

/******************************************************************************
*                                                                             *
* -------------------------- buffer_pool_alloc ------------------------------- *
*                                                                             *
******************************************************************************/
Buffer *buffer_pool_alloc(BufferPool *pool) {

  if (pool->free_list == NULL) {
    return NULL;
  }

  /* Unlink the head buffer from the free list. */

  Buffer *buf     = pool->free_list;
  pool->free_list = buf->next;
  buf->next       = NULL;

  pool->free_count--;

  if (pool->free_count < pool->min_free) {
    pool->min_free = pool->free_count;
  }

  return buf;

}

/******************************************************************************
*                                                                             *
* -------------------------- buffer_pool_free -------------------------------- *
*                                                                             *
******************************************************************************/
void buffer_pool_free(BufferPool *pool, Buffer *buf) {

  /* Push the buffer back onto the head of the free list. */

  buf->next       = pool->free_list;
  pool->free_list = buf;
  pool->free_count++;

}

/******************************************************************************
*                                                                             *
* ----------------------- buffer_pool_free_count ----------------------------- *
*                                                                             *
******************************************************************************/
int buffer_pool_free_count(const BufferPool *pool) {

  return pool->free_count;

}

/******************************************************************************
*                                                                             *
* ------------------------- buffer_pool_min_free ----------------------------- *
*                                                                             *
******************************************************************************/
int buffer_pool_min_free(const BufferPool *pool) {

  return pool->min_free;

}
