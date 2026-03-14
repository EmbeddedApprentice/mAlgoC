extern "C" {
#include "buffer_pool.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(BPool) {
    BufferPool pool;

    void setup() {
        buffer_pool_init(&pool);
    }

    void teardown() {}
};

/* ── Init ──────────────────────────────────────────────────────────────────── */

TEST(BPool, InitFreeCountEqualsCapacity) {
    LONGS_EQUAL(BUFFER_POOL_CAPACITY, buffer_pool_free_count(&pool));
}

TEST(BPool, InitMinFreeEqualsCapacity) {
    LONGS_EQUAL(BUFFER_POOL_CAPACITY, buffer_pool_min_free(&pool));
}

/* ── Alloc ─────────────────────────────────────────────────────────────────── */

TEST(BPool, AllocReturnsNonNull) {
    Buffer *buf = buffer_pool_alloc(&pool);
    CHECK(buf != NULL);
}

TEST(BPool, AllocDecrementsFreeeCount) {
    buffer_pool_alloc(&pool);
    LONGS_EQUAL(BUFFER_POOL_CAPACITY - 1, buffer_pool_free_count(&pool));
}

TEST(BPool, AllocResultIsWithinStorageArray) {
    Buffer *buf = buffer_pool_alloc(&pool);
    CHECK(buf >= &pool.storage[0]);
    CHECK(buf <= &pool.storage[BUFFER_POOL_CAPACITY - 1]);
}

TEST(BPool, AllocOnExhaustedPoolReturnsNull) {
    for (int i = 0; i < BUFFER_POOL_CAPACITY; i++) {
        buffer_pool_alloc(&pool);
    }
    POINTERS_EQUAL(NULL, buffer_pool_alloc(&pool));
}

TEST(BPool, AllocedBufferNextIsNull) {
    Buffer *buf = buffer_pool_alloc(&pool);
    POINTERS_EQUAL(NULL, buf->next);
}

/* ── Free ──────────────────────────────────────────────────────────────────── */

TEST(BPool, FreeIncrementsFreeeCount) {
    Buffer *buf = buffer_pool_alloc(&pool);
    buffer_pool_free(&pool, buf);
    LONGS_EQUAL(BUFFER_POOL_CAPACITY, buffer_pool_free_count(&pool));
}

/* ── Round-trip ────────────────────────────────────────────────────────────── */

TEST(BPool, AllocAllFreAllRestoresCapacity) {
    Buffer *bufs[BUFFER_POOL_CAPACITY];
    for (int i = 0; i < BUFFER_POOL_CAPACITY; i++) {
        bufs[i] = buffer_pool_alloc(&pool);
    }
    for (int i = 0; i < BUFFER_POOL_CAPACITY; i++) {
        buffer_pool_free(&pool, bufs[i]);
    }
    LONGS_EQUAL(BUFFER_POOL_CAPACITY, buffer_pool_free_count(&pool));
}

/* ── Watermark ─────────────────────────────────────────────────────────────── */

TEST(BPool, MinFreeTracksWatermark) {
    Buffer *bufs[3];
    for (int i = 0; i < 3; i++) {
        bufs[i] = buffer_pool_alloc(&pool);
    }
    /* free_count is now CAPACITY - 3; that should be the min */
    LONGS_EQUAL(BUFFER_POOL_CAPACITY - 3, buffer_pool_min_free(&pool));

    /* Freeing does not raise min_free */
    for (int i = 0; i < 3; i++) {
        buffer_pool_free(&pool, bufs[i]);
    }
    LONGS_EQUAL(BUFFER_POOL_CAPACITY - 3, buffer_pool_min_free(&pool));
}

/* ── LIFO ──────────────────────────────────────────────────────────────────── */

TEST(BPool, AllocThenFreeReturnsSamePointer) {
    Buffer *first = buffer_pool_alloc(&pool);
    buffer_pool_free(&pool, first);
    Buffer *second = buffer_pool_alloc(&pool);
    POINTERS_EQUAL(first, second);
}
