extern "C" {
#include "dlist.h"
}

#include "CppUTest/TestHarness.h"
#include <stdlib.h>

static int destroyCallCount;

static void countingDestroy(void *data) {
    (void)data;
    destroyCallCount++;
}

TEST_GROUP(DList) {
    dList list;

    void setup() {
        dlist_init(&list, NULL);
    }

    void teardown() {
        dlist_destroy(&list);
    }
};

/* ── Stress tests (defined first → run last) ─────────────────────────────── */

static void dlistInsertRemoveN(int n) {
    dList l;
    dlist_init(&l, NULL);

    int *vals = (int *)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        vals[i] = i;
        LONGS_EQUAL(0, dlist_insert_next(&l, NULL, &vals[i]));
    }
    LONGS_EQUAL(n, dlist_size(&l));

    void *data;
    for (int i = 0; i < n; i++) {
        LONGS_EQUAL(0, dlist_rem_next(&l, NULL, &data));
    }
    LONGS_EQUAL(0, dlist_size(&l));
    POINTERS_EQUAL(NULL, dlist_head(&l));
    POINTERS_EQUAL(NULL, dlist_tail(&l));

    free(vals);
    dlist_destroy(&l);
}

TEST(DList, InsertRemove100) {
    dlistInsertRemoveN(100);
}

TEST(DList, InsertRemove1000) {
    dlistInsertRemoveN(1000);
}

TEST(DList, InsertRemove10000) {
    dlistInsertRemoveN(10000);
}

TEST(DList, InsertRemove100000) {
    dlistInsertRemoveN(100000);
}

/* ── Unit tests ──────────────────────────────────────────────────────────── */

TEST(DList, InitializesToSizeZero) {
    LONGS_EQUAL(0, dlist_size(&list));
}

TEST(DList, InitHeadAndTailAreNull) {
    POINTERS_EQUAL(NULL, dlist_head(&list));
    POINTERS_EQUAL(NULL, dlist_tail(&list));
}

TEST(DList, InsertAtHeadIncreasesSize) {
    int val = 42;
    dlist_insert_next(&list, NULL, &val);
    LONGS_EQUAL(1, dlist_size(&list));
}

TEST(DList, InsertAtHeadSetsHead) {
    int val = 42;
    dlist_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(&val, dlist_data(dlist_head(&list)));
}

TEST(DList, InsertAtHeadOnEmptySetsTail) {
    int val = 42;
    dlist_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(&val, dlist_data(dlist_tail(&list)));
}

TEST(DList, InsertAfterElementIncreasesSize) {
    int a = 1, b = 2;
    dlist_insert_next(&list, NULL, &a);
    dlist_insert_next(&list, dlist_head(&list), &b);
    LONGS_EQUAL(2, dlist_size(&list));
}

TEST(DList, InsertAfterTailUpdatesTail) {
    int a = 1, b = 2;
    dlist_insert_next(&list, NULL, &a);
    dlist_insert_next(&list, dlist_head(&list), &b);
    POINTERS_EQUAL(&b, dlist_data(dlist_tail(&list)));
}

TEST(DList, RemoveFromEmptyListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, dlist_rem_next(&list, NULL, &data));
}

TEST(DList, RemoveFromHeadDecreasesSize) {
    int val = 1;
    void *data;
    dlist_insert_next(&list, NULL, &val);
    dlist_rem_next(&list, NULL, &data);
    LONGS_EQUAL(0, dlist_size(&list));
}

TEST(DList, RemoveFromHeadReturnsData) {
    int val = 99;
    void *data;
    dlist_insert_next(&list, NULL, &val);
    dlist_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(&val, data);
}

TEST(DList, RemoveFromHeadClearsHeadTailOnLastElement) {
    int val = 1;
    void *data;
    dlist_insert_next(&list, NULL, &val);
    dlist_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(NULL, dlist_head(&list));
    POINTERS_EQUAL(NULL, dlist_tail(&list));
}

TEST(DList, RemoveAfterElementUpdatesNext) {
    int a = 1, b = 2, c = 3;
    void *data;
    dlist_insert_next(&list, NULL, &c);
    dlist_insert_next(&list, NULL, &b);
    dlist_insert_next(&list, NULL, &a);
    /* list: a <-> b <-> c */
    dlist_rem_next(&list, dlist_head(&list), &data);
    /* list: a <-> c, removed b */
    POINTERS_EQUAL(&b, data);
    POINTERS_EQUAL(&c, dlist_data(dlist_next(dlist_head(&list))));
}

TEST(DList, DestroyCallsUserDestroyFn) {
    destroyCallCount = 0;
    dList dl;
    dlist_init(&dl, countingDestroy);
    int a = 1, b = 2;
    dlist_insert_next(&dl, NULL, &a);
    dlist_insert_next(&dl, NULL, &b);
    dlist_destroy(&dl);
    LONGS_EQUAL(2, destroyCallCount);
}

TEST(DList, InsertNextNullListReturnsError) {
    int val = 1;
    LONGS_EQUAL(-1, dlist_insert_next(NULL, NULL, &val));
}

TEST(DList, RemoveNextNullListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, dlist_rem_next(NULL, NULL, &data));
}

TEST(DList, SizeOfNullListIsZero) {
    LONGS_EQUAL(0, dlist_size(NULL));
}

/* ── Prev-pointer tests ───────────────────────────────────────────────────── */

TEST(DList, HeadPrevIsNull) {
    int val = 1;
    dlist_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(NULL, dlist_prev(dlist_head(&list)));
}

TEST(DList, InsertAfterHeadSetsSecondElementPrev) {
    int a = 1, b = 2;
    dlist_insert_next(&list, NULL, &a);
    dlist_insert_next(&list, dlist_head(&list), &b);
    /* list: a <-> b */
    dListElmt *second = dlist_next(dlist_head(&list));
    POINTERS_EQUAL(dlist_head(&list), dlist_prev(second));
}

TEST(DList, InsertAtHeadUpdatesOldHeadPrev) {
    int a = 1, b = 2;
    dlist_insert_next(&list, NULL, &a);
    dlist_insert_next(&list, NULL, &b);
    /* list: b <-> a */
    POINTERS_EQUAL(dlist_head(&list), dlist_prev(dlist_tail(&list)));
}

TEST(DList, RemoveAfterElementFixesPrev) {
    int a = 1, b = 2, c = 3;
    void *data;
    dlist_insert_next(&list, NULL, &c);
    dlist_insert_next(&list, NULL, &b);
    dlist_insert_next(&list, NULL, &a);
    /* list: a <-> b <-> c */
    dlist_rem_next(&list, dlist_head(&list), &data);
    /* list: a <-> c */
    POINTERS_EQUAL(dlist_head(&list), dlist_prev(dlist_tail(&list)));
}

TEST(DList, RemoveHeadFixesNewHeadPrev) {
    int a = 1, b = 2;
    void *data;
    dlist_insert_next(&list, NULL, &b);
    dlist_insert_next(&list, NULL, &a);
    /* list: a <-> b */
    dlist_rem_next(&list, NULL, &data);
    /* list: b */
    POINTERS_EQUAL(NULL, dlist_prev(dlist_head(&list)));
}
