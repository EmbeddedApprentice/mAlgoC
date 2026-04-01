extern "C" {
#include "queue.h"
}

#include "CppUTest/TestHarness.h"
#include <stdlib.h>

static int destroyCallCount;

static void countingDestroy(void *data) {
    (void)data;
    destroyCallCount++;
}

TEST_GROUP(Queue) {
    Queue queue;

    void setup() {
        queue_init(&queue, NULL);
    }

    void teardown() {
        queue_destroy(&queue);
    }
};

/* ── Stress tests (defined first → run last) ─────────────────────────────── */

static void enqueueDequeueN(int n) {
    Queue q;
    queue_init(&q, NULL);

    int *vals = (int *)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        vals[i] = i;
        LONGS_EQUAL(0, queue_enqueue(&q, &vals[i]));
    }
    LONGS_EQUAL(n, queue_size(&q));

    void *data;
    for (int i = 0; i < n; i++) {
        LONGS_EQUAL(0, queue_dequeue(&q, &data));
    }
    LONGS_EQUAL(0, queue_size(&q));

    free(vals);
    queue_destroy(&q);
}

TEST(Queue, EnqueueDequeue100) {
    enqueueDequeueN(100);
}

TEST(Queue, EnqueueDequeue1000) {
    enqueueDequeueN(1000);
}

TEST(Queue, EnqueueDequeue10000) {
    enqueueDequeueN(10000);
}

TEST(Queue, EnqueueDequeue100000) {
    enqueueDequeueN(100000);
}

/* ── Unit tests ──────────────────────────────────────────────────────────── */

TEST(Queue, InitializesToSizeZero) {
    LONGS_EQUAL(0, queue_size(&queue));
}

TEST(Queue, InitIsEmpty) {
    CHECK(queue_is_empty(&queue));
}

TEST(Queue, EnqueueIncreasesSize) {
    int val = 1;
    queue_enqueue(&queue, &val);
    LONGS_EQUAL(1, queue_size(&queue));
}

TEST(Queue, EnqueueMakesNotEmpty) {
    int val = 1;
    queue_enqueue(&queue, &val);
    CHECK(!queue_is_empty(&queue));
}

TEST(Queue, EnqueueSetsFront) {
    int val = 42;
    queue_enqueue(&queue, &val);
    POINTERS_EQUAL(&val, queue_peek(&queue));
}

TEST(Queue, EnqueueIsFIFO) {
    int a = 1, b = 2, c = 3;
    void *data;
    queue_enqueue(&queue, &a);
    queue_enqueue(&queue, &b);
    queue_enqueue(&queue, &c);
    /* a was enqueued first — must dequeue first */
    queue_dequeue(&queue, &data);
    POINTERS_EQUAL(&a, data);
    queue_dequeue(&queue, &data);
    POINTERS_EQUAL(&b, data);
    queue_dequeue(&queue, &data);
    POINTERS_EQUAL(&c, data);
}

TEST(Queue, DequeueDecreasesSize) {
    int val = 1;
    void *data;
    queue_enqueue(&queue, &val);
    queue_dequeue(&queue, &data);
    LONGS_EQUAL(0, queue_size(&queue));
}

TEST(Queue, DequeueReturnsData) {
    int val = 99;
    void *data;
    queue_enqueue(&queue, &val);
    queue_dequeue(&queue, &data);
    POINTERS_EQUAL(&val, data);
}

TEST(Queue, DequeueFromEmptyReturnsError) {
    void *data;
    LONGS_EQUAL(-1, queue_dequeue(&queue, &data));
}

TEST(Queue, PeekDoesNotChangeSize) {
    int val = 7;
    queue_enqueue(&queue, &val);
    (void)queue_peek(&queue);
    LONGS_EQUAL(1, queue_size(&queue));
}

TEST(Queue, PeekReturnsFrontWithoutRemoving) {
    int a = 1, b = 2;
    queue_enqueue(&queue, &a);
    queue_enqueue(&queue, &b);
    POINTERS_EQUAL(&a, queue_peek(&queue));
    LONGS_EQUAL(2, queue_size(&queue));
}

TEST(Queue, DestroyCallsUserDestroyFn) {
    destroyCallCount = 0;
    Queue q;
    queue_init(&q, countingDestroy);
    int a = 1, b = 2;
    queue_enqueue(&q, &a);
    queue_enqueue(&q, &b);
    queue_destroy(&q);
    LONGS_EQUAL(2, destroyCallCount);
}

TEST(Queue, EnqueueNullQueueReturnsError) {
    int val = 1;
    LONGS_EQUAL(-1, queue_enqueue(NULL, &val));
}

TEST(Queue, DequeueNullQueueReturnsError) {
    void *data;
    LONGS_EQUAL(-1, queue_dequeue(NULL, &data));
}

TEST(Queue, SizeOfNullQueueIsZero) {
    LONGS_EQUAL(0, queue_size(NULL));
}
