#include <stdio.h>
#include <string.h>
#include "cb_ring.h"

extern int tests_passed;
extern int tests_failed;

#define TEST(name, expr) do { \
    if (!(expr)) { \
        printf("  FAIL: %s (%s:%d)\n", name, __FILE__, __LINE__); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", name); \
        tests_passed++; \
    } \
} while (0)

void test_ring(void) {
    printf("\n--- RingBuffer Module ---\n");

    /* init success */
    CB_Ring ring;
    memset(&ring, 0, sizeof(ring));
    TEST("init success", cb_ring_init(&ring, sizeof(int), 4) == CB_OK);
    TEST("init element_size=4", ring.element_size == sizeof(int));
    TEST("init capacity=4", ring.capacity == 4);
    TEST("init size=0", ring.size == 0);

    /* init NULL failure */
    TEST("init NULL failure", cb_ring_init(NULL, sizeof(int), 4) == CB_ERR_INVALID_ARGUMENT);

    /* element_size 0 failure */
    CB_Ring ring0;
    TEST("element_size 0 failure", cb_ring_init(&ring0, 0, 4) == CB_ERR_INVALID_ARGUMENT);

    /* capacity 0 failure */
    TEST("capacity 0 failure", cb_ring_init(&ring0, sizeof(int), 0) == CB_ERR_INVALID_ARGUMENT);

    /* push one element */
    int val = 42;
    TEST("push one element", cb_ring_push(&ring, &val) == CB_OK);
    TEST("size=1 after push", ring.size == 1);

    /* pop one element */
    int out;
    TEST("pop one element", cb_ring_pop(&ring, &out) == CB_OK);
    TEST("pop value correct", out == 42);
    TEST("size=0 after pop", ring.size == 0);

    /* FIFO order */
    int v1 = 10, v2 = 20, v3 = 30;
    cb_ring_push(&ring, &v1);
    cb_ring_push(&ring, &v2);
    cb_ring_push(&ring, &v3);
    cb_ring_pop(&ring, &out); TEST("FIFO first=10", out == 10);
    cb_ring_pop(&ring, &out); TEST("FIFO second=20", out == 20);
    cb_ring_pop(&ring, &out); TEST("FIFO third=30", out == 30);

    /* peek does not remove */
    cb_ring_push(&ring, &v1);
    TEST("peek success", cb_ring_peek(&ring, &out) == CB_OK);
    TEST("peek value=10", out == 10);
    TEST("size still 1 after peek", ring.size == 1);
    cb_ring_pop(&ring, &out);

    /* push until full */
    for (int i = 0; i < 4; i++) {
        cb_ring_push(&ring, &i);
    }
    TEST("size=4 full", ring.size == 4);
    TEST("is_full true", cb_ring_is_full(&ring) == 1);

    /* push full returns error */
    int full_val = 99;
    TEST("push full error", cb_ring_push(&ring, &full_val) == CB_ERR_BUFFER_TOO_SMALL);

    /* pop empty test */
    CB_Ring empty;
    cb_ring_init(&empty, sizeof(int), 4);
    TEST("pop empty error", cb_ring_pop(&empty, &out) == CB_ERR_INDEX_OUT_OF_BOUNDS);
    cb_ring_free(&empty);

    /* peek empty test */
    CB_Ring empty2;
    cb_ring_init(&empty2, sizeof(int), 4);
    TEST("peek empty error", cb_ring_peek(&empty2, &out) == CB_ERR_INDEX_OUT_OF_BOUNDS);
    cb_ring_free(&empty2);

    /* wrap around behavior */
    cb_ring_free(&ring);
    cb_ring_init(&ring, sizeof(int), 3);
    cb_ring_push(&ring, &v1); cb_ring_push(&ring, &v2); cb_ring_push(&ring, &v3);
    cb_ring_pop(&ring, &out); TEST("wrap pop 1=10", out == 10);
    cb_ring_pop(&ring, &out); TEST("wrap pop 2=20", out == 20);
    int v4 = 40, v5 = 50;
    cb_ring_push(&ring, &v4);
    cb_ring_push(&ring, &v5);
    cb_ring_pop(&ring, &out); TEST("wrap pop 3=30", out == 30);
    cb_ring_pop(&ring, &out); TEST("wrap pop 4=40", out == 40);
    cb_ring_pop(&ring, &out); TEST("wrap pop 5=50", out == 50);
    TEST("size=0 after wrap pops", ring.size == 0);

    /* clear resets */
    cb_ring_push(&ring, &v1);
    cb_ring_push(&ring, &v2);
    size_t cap = ring.capacity;
    cb_ring_clear(&ring);
    TEST("clear size=0", ring.size == 0);
    TEST("clear capacity preserved", ring.capacity == cap);

    /* free */
    cb_ring_free(&ring);
    TEST("free data=NULL", ring.data == NULL);
    TEST("free capacity=0", ring.capacity == 0);

    /* struct */
    typedef struct { int x; double y; } Point;
    CB_Ring pr;
    cb_ring_init(&pr, sizeof(Point), 3);
    Point p1 = {1, 1.5}, p2 = {2, 2.5};
    cb_ring_push(&pr, &p1);
    cb_ring_push(&pr, &p2);
    Point po;
    cb_ring_pop(&pr, &po);
    TEST("struct pop x=1", po.x == 1);
    TEST("struct pop y~1.5", po.y > 1.4 && po.y < 1.6);
    cb_ring_free(&pr);

    /* double free */
    cb_ring_free(&ring);
    TEST("double free safe", 1);

    /* NULL safety */
    CB_Ring rs;
    cb_ring_init(&rs, sizeof(int), 4);
    TEST("push NULL element error", cb_ring_push(&rs, NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("is_empty on NULL", cb_ring_is_empty(NULL) == 1);
    TEST("is_full on NULL", cb_ring_is_full(NULL) == 0);
    cb_ring_free(&rs);

    /* is_empty/is_full after init */
    CB_Ring ri;
    cb_ring_init(&ri, sizeof(int), 4);
    TEST("is_empty after init", cb_ring_is_empty(&ri) == 1);
    TEST("is_full after init", cb_ring_is_full(&ri) == 0);
    int x = 1;
    cb_ring_push(&ri, &x);
    TEST("is_empty false after push", cb_ring_is_empty(&ri) == 0);
    TEST("is_full false after 1 push", cb_ring_is_full(&ri) == 0);
    cb_ring_free(&ri);
}