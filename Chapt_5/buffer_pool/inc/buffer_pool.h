/******************************************************************************
*                                                                             *
* ----------------------------- buffer_pool.h -------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include <stdint.h>

/* Tune these for your target. */
#define BUFFER_DATA_SIZE     256
#define BUFFER_POOL_CAPACITY  16

/******************************************************************************
*                                                                             *
* Each buffer embeds its own free-list link so no heap node is ever needed.  *
* When a buffer is on the free list, `next` points to the next free buffer.  *
* When a buffer is in use, `next` is NULL.                                   *
*                                                                             *
******************************************************************************/

typedef struct Buffer_ {
  struct Buffer_  *next;
  uint8_t          data[BUFFER_DATA_SIZE];
} Buffer;

/******************************************************************************
*                                                                             *
* The pool owns a static array of Buffer objects. All metadata lives here;   *
* declare one of these as a static variable — no malloc required.            *
*                                                                             *
******************************************************************************/

typedef struct BufferPool_ {
  Buffer  storage[BUFFER_POOL_CAPACITY];
  Buffer *free_list;
  int     free_count;
  int     min_free;     /* instrumentation: lowest free_count ever seen */
} BufferPool;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

/* Link all buffers into the free list; call once at startup. */
void    buffer_pool_init(BufferPool *pool);

/* Pop one buffer from the free list. Returns NULL if the pool is exhausted. */
Buffer *buffer_pool_alloc(BufferPool *pool);

/* Push a buffer back onto the free list. */
void    buffer_pool_free(BufferPool *pool, Buffer *buf);

/* Current number of free buffers. */
int     buffer_pool_free_count(const BufferPool *pool);

/* Lowest free count recorded since init (stress-test watermark). */
int     buffer_pool_min_free(const BufferPool *pool);

#endif /* BUFFER_POOL_H */
