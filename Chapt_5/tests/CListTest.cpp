extern "C" {
#include "clist.h"
}

#include "CppUTest/TestHarness.h"
#include <stdlib.h>

static int destroyCallCount;

static void countingDestroy(void *data) {
    (void)data;
    destroyCallCount++;
}

TEST_GROUP(CList) {
    CList list;

    void setup() {
        clist_init(&list, NULL);
    }

    void teardown() {
        clist_destroy(&list);
    }
};

/* ── Stress tests (defined first → run last) ─────────────────────────────── */

/* Insert n elements after head, then remove via head as predecessor (O(1)). */
static void insertRemoveN(int n) {
    CList l;
    clist_init(&l, NULL);

    int *vals = (int *)malloc((size_t)n * sizeof(int));

    vals[0] = 0;
    LONGS_EQUAL(0, clist_insert_next(&l, NULL, &vals[0]));

    for (int i = 1; i < n; i++) {
        vals[i] = i;
        LONGS_EQUAL(0, clist_insert_next(&l, clist_head(&l), &vals[i]));
    }
    LONGS_EQUAL(n, clist_size(&l));

    void *data;
    while (clist_size(&l) > 0) {
        LONGS_EQUAL(0, clist_rem_next(&l, clist_head(&l), &data));
    }
    LONGS_EQUAL(0, clist_size(&l));
    POINTERS_EQUAL(NULL, clist_head(&l));

    free(vals);
    clist_destroy(&l);
}

TEST(CList, InsertRemove100) {
    insertRemoveN(100);
}

TEST(CList, InsertRemove1000) {
    insertRemoveN(1000);
}

TEST(CList, InsertRemove10000) {
    insertRemoveN(10000);
}

TEST(CList, InsertRemove100000) {
    insertRemoveN(100000);
}

/* ── Unit tests ──────────────────────────────────────────────────────────── */

TEST(CList, InitializesToSizeZero) {
    LONGS_EQUAL(0, clist_size(&list));
}

TEST(CList, InitHeadIsNull) {
    POINTERS_EQUAL(NULL, clist_head(&list));
}

TEST(CList, InsertFirstElementMakesSelfReferential) {
    int val = 1;
    clist_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(clist_head(&list), clist_next(clist_head(&list)));
}

TEST(CList, InsertIncreasesSize) {
    int val = 1;
    clist_insert_next(&list, NULL, &val);
    LONGS_EQUAL(1, clist_size(&list));
}

TEST(CList, InsertFirstSetsHead) {
    int val = 42;
    clist_insert_next(&list, NULL, &val);
    POINTERS_EQUAL(&val, clist_data(clist_head(&list)));
}

TEST(CList, InsertAfterElementLinksCorrectly) {
    int a = 1, b = 2;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    /* list: a -> b -> a (circular) */
    POINTERS_EQUAL(&b, clist_data(clist_next(clist_head(&list))));
    POINTERS_EQUAL(clist_head(&list), clist_next(clist_next(clist_head(&list))));
}

TEST(CList, InsertAfterIncreasesSize) {
    int a = 1, b = 2;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    LONGS_EQUAL(2, clist_size(&list));
}

TEST(CList, InsertNullOnNonEmptyListReturnsError) {
    int a = 1, b = 2;
    clist_insert_next(&list, NULL, &a);
    LONGS_EQUAL(-1, clist_insert_next(&list, NULL, &b));
}

TEST(CList, CircularLinkMaintainedAfterMultipleInserts) {
    int a = 1, b = 2, c = 3;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    clist_insert_next(&list, clist_head(&list), &c);
    /* Traverse full circle — must return to head after 3 steps */
    CListElmt *e = clist_head(&list);
    e = clist_next(e);
    e = clist_next(e);
    e = clist_next(e);
    POINTERS_EQUAL(clist_head(&list), e);
}

TEST(CList, RemoveFromEmptyListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, clist_rem_next(&list, NULL, &data));
}

TEST(CList, RemoveHeadDecreasesSize) {
    int val = 1;
    void *data;
    clist_insert_next(&list, NULL, &val);
    clist_rem_next(&list, NULL, &data);
    LONGS_EQUAL(0, clist_size(&list));
}

TEST(CList, RemoveHeadReturnsData) {
    int val = 99;
    void *data;
    clist_insert_next(&list, NULL, &val);
    clist_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(&val, data);
}

TEST(CList, RemoveLastElementSetsHeadNull) {
    int val = 1;
    void *data;
    clist_insert_next(&list, NULL, &val);
    clist_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(NULL, clist_head(&list));
}

TEST(CList, RemoveHeadUpdatesHead) {
    int a = 1, b = 2;
    void *data;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    /* list: a -> b -> a; remove head (a) → new head is b */
    clist_rem_next(&list, NULL, &data);
    POINTERS_EQUAL(&b, clist_data(clist_head(&list)));
}

TEST(CList, RemoveHeadMaintainsCircularLink) {
    int a = 1, b = 2, c = 3;
    void *data;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    clist_insert_next(&list, clist_head(&list), &c);
    /* list: a -> c -> b -> a; remove head (a) → new head is c */
    clist_rem_next(&list, NULL, &data);
    /* Traverse full circle — must return to new head after 2 steps */
    CListElmt *e = clist_head(&list);
    e = clist_next(e);
    e = clist_next(e);
    POINTERS_EQUAL(clist_head(&list), e);
}

TEST(CList, RemoveAfterElementSplicesCorrectly) {
    int a = 1, b = 2, c = 3;
    void *data;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    clist_insert_next(&list, clist_head(&list), &c);
    /* list: a -> c -> b -> a; remove element after head (c) */
    clist_rem_next(&list, clist_head(&list), &data);
    POINTERS_EQUAL(&c, data);
    /* list should now be: a -> b -> a */
    POINTERS_EQUAL(&b, clist_data(clist_next(clist_head(&list))));
    LONGS_EQUAL(2, clist_size(&list));
}

TEST(CList, RemoveViaElementUpdatesHeadWhenHeadRemoved) {
    int a = 1, b = 2;
    void *data;
    clist_insert_next(&list, NULL, &a);
    clist_insert_next(&list, clist_head(&list), &b);
    /* list: a -> b -> a; remove b->next (= a = head) via b */
    CListElmt *b_node = clist_next(clist_head(&list));
    clist_rem_next(&list, b_node, &data);
    POINTERS_EQUAL(&a, data);
    POINTERS_EQUAL(&b, clist_data(clist_head(&list)));
}

TEST(CList, DestroyCallsUserDestroyFn) {
    destroyCallCount = 0;
    CList dl;
    clist_init(&dl, countingDestroy);
    int a = 1, b = 2;
    clist_insert_next(&dl, NULL, &a);
    clist_insert_next(&dl, clist_head(&dl), &b);
    clist_destroy(&dl);
    LONGS_EQUAL(2, destroyCallCount);
}

TEST(CList, InsertNextNullListReturnsError) {
    int val = 1;
    LONGS_EQUAL(-1, clist_insert_next(NULL, NULL, &val));
}

TEST(CList, RemoveNextNullListReturnsError) {
    void *data;
    LONGS_EQUAL(-1, clist_rem_next(NULL, NULL, &data));
}

TEST(CList, SizeOfNullListIsZero) {
    LONGS_EQUAL(0, clist_size(NULL));
}
