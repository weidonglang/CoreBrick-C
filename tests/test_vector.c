#include <stdio.h>
#include <string.h>
#include "cb_vector.h"

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

void test_vector(void) {
    printf("\n--- Vector Module ---\n");

    /* init success */
    CB_Vector vec;
    memset(&vec, 0, sizeof(vec));
    TEST("init success", cb_vector_init(&vec, sizeof(int), 4) == CB_OK);
    TEST("init data non-null", vec.data != NULL);
    TEST("init element_size=4", vec.element_size == sizeof(int));
    TEST("init size=0", vec.size == 0);
    TEST("init capacity=4", vec.capacity == 4);
    cb_vector_free(&vec);

    /* init NULL failure */
    TEST("init NULL failure", cb_vector_init(NULL, sizeof(int), 4) == CB_ERR_INVALID_ARGUMENT);

    /* element_size 0 failure */
    CB_Vector vec2;
    TEST("element_size 0 failure", cb_vector_init(&vec2, 0, 4) == CB_ERR_INVALID_ARGUMENT);

    /* push one element */
    cb_vector_init(&vec, sizeof(int), 1);
    int val = 42;
    TEST("push one element", cb_vector_push(&vec, &val) == CB_OK);
    TEST("size=1 after push", vec.size == 1);

    /* push many elements */
    for (int i = 0; i < 100; i++) {
        int v = i;
        cb_vector_push(&vec, &v);
    }
    TEST("size=101 after many pushes", vec.size == 101);

    /* auto grow */
    TEST("capacity grown", vec.capacity >= 101);

    /* get valid index */
    int out;
    TEST("get index 0", cb_vector_get(&vec, 0, &out) == CB_OK && out == 42);
    TEST("get index 50", cb_vector_get(&vec, 50, &out) == CB_OK && out == 49);

    /* get invalid index failure */
    TEST("get invalid index", cb_vector_get(&vec, 1000, &out) == CB_ERR_INDEX_OUT_OF_BOUNDS);

    /* get_ptr valid */
    int *ptr = (int *)cb_vector_get_ptr(&vec, 0);
    TEST("get_ptr valid", ptr != NULL && *ptr == 42);

    /* get_ptr invalid returns NULL */
    TEST("get_ptr invalid returns NULL", cb_vector_get_ptr(&vec, 1000) == NULL);
    TEST("get_ptr NULL vec returns NULL", cb_vector_get_ptr(NULL, 0) == NULL);

    /* set valid index */
    int new_val = 999;
    TEST("set index 0", cb_vector_set(&vec, 0, &new_val) == CB_OK);
    cb_vector_get(&vec, 0, &out);
    TEST("set verified", out == 999);

    /* set invalid index failure */
    TEST("set invalid index", cb_vector_set(&vec, 1000, &new_val) == CB_ERR_INDEX_OUT_OF_BOUNDS);

    /* pop success */
    int popped;
    TEST("pop success", cb_vector_pop(&vec, &popped) == CB_OK);
    TEST("pop value correct", popped == 99);
    TEST("size decreased after pop", vec.size == 100);

    /* pop without out_element */
    TEST("pop NULL out_element", cb_vector_pop(&vec, NULL) == CB_OK);
    TEST("size=99 after pop NULL", vec.size == 99);

    /* pop empty failure */
    CB_Vector empty_vec;
    cb_vector_init(&empty_vec, sizeof(int), 4);
    TEST("pop empty failure", cb_vector_pop(&empty_vec, &popped) == CB_ERR_INDEX_OUT_OF_BOUNDS);
    cb_vector_free(&empty_vec);

    /* clear keeps capacity */
    size_t cap_before = vec.capacity;
    cb_vector_clear(&vec);
    TEST("clear sets size=0", vec.size == 0);
    TEST("clear keeps capacity", vec.capacity == cap_before);

    /* free resets fields */
    cb_vector_free(&vec);
    TEST("free data=NULL", vec.data == NULL);
    TEST("free size=0", vec.size == 0);
    TEST("free capacity=0", vec.capacity == 0);
    TEST("free element_size=0", vec.element_size == 0);

    /* store int (already tested above) */

    /* store struct */
    typedef struct { int x; double y; } Point;
    CB_Vector pt_vec;
    cb_vector_init(&pt_vec, sizeof(Point), 2);
    Point p1 = {10, 20.5};
    Point p2 = {30, 40.5};
    TEST("push struct", cb_vector_push(&pt_vec, &p1) == CB_OK);
    TEST("push struct 2", cb_vector_push(&pt_vec, &p2) == CB_OK);
    Point p_out;
    cb_vector_get(&pt_vec, 0, &p_out);
    TEST("struct get x", p_out.x == 10);
    TEST("struct get y", p_out.y > 20.4 && p_out.y < 20.6);
    cb_vector_get(&pt_vec, 1, &p_out);
    TEST("struct get x2", p_out.x == 30);
    cb_vector_free(&pt_vec);

    /* NULL push */
    cb_vector_init(&vec, sizeof(int), 4);
    TEST("push NULL element", cb_vector_push(&vec, NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("push NULL vec", cb_vector_push(NULL, &val) == CB_ERR_INVALID_ARGUMENT);
    cb_vector_free(&vec);

    /* overflow protection - extreme capacity */
    /* Not testing actual overflow, just that it's handled */

    /* free NULL safety */
    cb_vector_free(NULL);
    TEST("free NULL safe", 1);

    /* clear NULL safety */
    cb_vector_clear(NULL);
    TEST("clear NULL safe", 1);
}