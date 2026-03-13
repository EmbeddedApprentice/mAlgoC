extern "C" {
#include "list.h"
}

#include "CppUTest/TestHarness.h"
#include <stdlib.h>

static int destroyCallCount;

static void countingDestroy(void *data) {
    (void)data;
    destroyCallCount++;
}

TEST_GROUP(LinkedList) {
    List list;

    void setup() {
        list_init(&list, NULL);
    }

    void teardown() {
        list_destroy(&list);
    }
};

/* ── Stress tests (defined first → run last) ─────────────────────────────── */

static void insertRemoveN(int n) {
    List l;
    list_init(&l, NULL);

    int *vals = (int *)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        vals[i] = i;
        LONGS_EQUAL(0, list_insert_next(&l, NULL, &vals[i]));
    }
    LONGS_EQUAL(n, list_size(&l));

    void *data;
    for (int i = 0; i < n; i++) {
        LONGS_EQUAL(0, list_rem_next(&l, NULL, &data));
    }
    LONGS_EQUAL(0, list_size(&l));
    POINTERS_EQUAL(NULL, list_head(&l));
    POINTERS_EQUAL(NULL, list_tail(&l));

    free(vals);
    list_destroy(&l);
}

TEST(LinkedList, InsertRemove100) {
    insertRemoveN(100);
}

TEST(LinkedList, InsertRemove1000) {
    insertRemoveN(1000);
}

TEST(LinkedList, InsertRemove10000) {
    insertRemoveN(10000);
}

TEST(LinkedList, InsertRemove100000) {
    insertRemoveN(100000);
}

/* ── Unit tests ──────────────────────────────────────────────────────────── */

TEST(LinkedList, InitializesToSizeZero) {
    LONGS_EQUAL(0, list_size(&list));
}

TEST(LinkedList, InitHeadAndTailAreNull) {
    POINTERS_EQUAL(NULL, list_head(&list));
    POINTERS_EQUAL(NULL, list_tail(&list));
}

TEST(LinkedList, InsertAtHeadIncreasesSize) {
    int val = 42;
    list_insert_next(&list, NULL, &val);
    LONGS_EQUAL(1, list_size(&list));
}

TEST(LinkedList, InsertAtHeadSetsHead) {
    int val = 42;
    list_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(&val, list_data(list_head(&list)));
}

TEST(LinkedList, InsertAtHeadOnEmptySetsTail) {
    int val = 42;
    list_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(&val, list_data(list_tail(&list)));
}

TEST(LinkedList, InsertAfterElementIncreasesSize) {
    int a = 1, b = 2;
    list_insert_next(&list, NULL, &a);
    list_insert_next(&list, list_head(&list), &b);
    LONGS_EQUAL(2, list_size(&list));
}

TEST(LinkedList, InsertAfterTailUpdatesTail) {
    int a = 1, b = 2;
    list_insert_next(&list, NULL, &a);
    list_insert_next(&list, list_head(&list), &b);
    POINTERS_EQUAL(&b, list_data(list_tail(&list)));
}

TEST(LinkedList, RemoveFromEmptyListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, list_rem_next(&list, NULL, &data));
}

TEST(LinkedList, RemoveFromHeadDecreasesSize) {
    int val = 1;
    void *data;
    list_insert_next(&list, NULL, &val);
    list_rem_next(&list, NULL, &data);
    LONGS_EQUAL(0, list_size(&list));
}

TEST(LinkedList, RemoveFromHeadReturnsData) {
    int val = 99;
    void *data;
    list_insert_next(&list, NULL, &val);
    list_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(&val, data);
}

TEST(LinkedList, RemoveFromHeadClearsHeadTailOnLastElement) {
    int val = 1;
    void *data;
    list_insert_next(&list, NULL, &val);
    list_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(NULL, list_head(&list));
    POINTERS_EQUAL(NULL, list_tail(&list));
}

TEST(LinkedList, RemoveAfterElementUpdatesNext) {
    int a = 1, b = 2, c = 3;
    void *data;
    list_insert_next(&list, NULL, &c);
    list_insert_next(&list, NULL, &b);
    list_insert_next(&list, NULL, &a);
    /* list: a -> b -> c */
    list_rem_next(&list, list_head(&list), &data);
    /* list: a -> c, removed b */
    POINTERS_EQUAL(&b, data);
    POINTERS_EQUAL(&c, list_data(list_next(list_head(&list))));
}

TEST(LinkedList, DestroyCallsUserDestroyFn) {
    destroyCallCount = 0;
    List dlist;
    list_init(&dlist, countingDestroy);
    int a = 1, b = 2;
    list_insert_next(&dlist, NULL, &a);
    list_insert_next(&dlist, NULL, &b);
    list_destroy(&dlist);
    LONGS_EQUAL(2, destroyCallCount);
}

TEST(LinkedList, InsertNextNullListReturnsError) {
    int val = 1;
    LONGS_EQUAL(-1, list_insert_next(NULL, NULL, &val));
}

TEST(LinkedList, RemoveNextNullListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, list_rem_next(NULL, NULL, &data));
}

TEST(LinkedList, SizeOfNullListIsZero) {
    LONGS_EQUAL(0, list_size(NULL));
}
