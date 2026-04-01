extern "C" {
#include "stack.h"
}

#include "CppUTest/TestHarness.h"
#include <stdlib.h>

static int destroyCallCount;

static void countingDestroy(void *data) {
    (void)data;
    destroyCallCount++;
}

TEST_GROUP(Stack) {
    Stack stack;

    void setup() {
        stack_init(&stack, NULL);
    }

    void teardown() {
        stack_destroy(&stack);
    }
};

/* ── Stress tests (defined first → run last) ─────────────────────────────── */

static void pushPopN(int n) {
    Stack s;
    stack_init(&s, NULL);

    int *vals = (int *)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        vals[i] = i;
        LONGS_EQUAL(0, stack_push(&s, &vals[i]));
    }
    LONGS_EQUAL(n, stack_size(&s));

    void *data;
    for (int i = 0; i < n; i++) {
        LONGS_EQUAL(0, stack_pop(&s, &data));
    }
    LONGS_EQUAL(0, stack_size(&s));

    free(vals);
    stack_destroy(&s);
}

TEST(Stack, PushPop100) {
    pushPopN(100);
}

TEST(Stack, PushPop1000) {
    pushPopN(1000);
}

TEST(Stack, PushPop10000) {
    pushPopN(10000);
}

TEST(Stack, PushPop100000) {
    pushPopN(100000);
}

/* ── Unit tests ──────────────────────────────────────────────────────────── */

TEST(Stack, InitializesToSizeZero) {
    LONGS_EQUAL(0, stack_size(&stack));
}

TEST(Stack, InitIsEmpty) {
    CHECK(stack_is_empty(&stack));
}

TEST(Stack, PushIncreasesSize) {
    int val = 1;
    stack_push(&stack, &val);
    LONGS_EQUAL(1, stack_size(&stack));
}

TEST(Stack, PushMakesNotEmpty) {
    int val = 1;
    stack_push(&stack, &val);
    CHECK(!stack_is_empty(&stack));
}

TEST(Stack, PushSetsTop) {
    int val = 42;
    stack_push(&stack, &val);
    POINTERS_EQUAL(&val, stack_peek(&stack));
}

TEST(Stack, PushIsLIFO) {
    int a = 1, b = 2, c = 3;
    void *data;
    stack_push(&stack, &a);
    stack_push(&stack, &b);
    stack_push(&stack, &c);
    /* c was pushed last — must pop first */
    stack_pop(&stack, &data);
    POINTERS_EQUAL(&c, data);
    stack_pop(&stack, &data);
    POINTERS_EQUAL(&b, data);
    stack_pop(&stack, &data);
    POINTERS_EQUAL(&a, data);
}

TEST(Stack, PopDecreasesSize) {
    int val = 1;
    void *data;
    stack_push(&stack, &val);
    stack_pop(&stack, &data);
    LONGS_EQUAL(0, stack_size(&stack));
}

TEST(Stack, PopReturnsData) {
    int val = 99;
    void *data;
    stack_push(&stack, &val);
    stack_pop(&stack, &data);
    POINTERS_EQUAL(&val, data);
}

TEST(Stack, PopFromEmptyReturnsError) {
    void *data;
    LONGS_EQUAL(-1, stack_pop(&stack, &data));
}

TEST(Stack, PeekDoesNotChangeSize) {
    int val = 7;
    stack_push(&stack, &val);
    (void)stack_peek(&stack);
    LONGS_EQUAL(1, stack_size(&stack));
}

TEST(Stack, DestroyCallsUserDestroyFn) {
    destroyCallCount = 0;
    Stack s;
    stack_init(&s, countingDestroy);
    int a = 1, b = 2;
    stack_push(&s, &a);
    stack_push(&s, &b);
    stack_destroy(&s);
    LONGS_EQUAL(2, destroyCallCount);
}

TEST(Stack, PushNullStackReturnsError) {
    int val = 1;
    LONGS_EQUAL(-1, stack_push(NULL, &val));
}

TEST(Stack, PopNullStackReturnsError) {
    void *data;
    LONGS_EQUAL(-1, stack_pop(NULL, &data));
}

TEST(Stack, SizeOfNullStackIsZero) {
    LONGS_EQUAL(0, stack_size(NULL));
}
