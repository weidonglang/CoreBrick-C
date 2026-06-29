#include <stdio.h>
#include <string.h>
#include "cb_string_map.h"

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

static char buf[256];
const char *format_key(int n) {
    snprintf(buf, sizeof(buf), "key-%d", n);
    return buf;
}

void test_string_map(void) {
    printf("\n--- StringMap Module ---\n");

    CB_StringMap *map = NULL;
    TEST("create success", cb_string_map_create(16, &map) == CB_OK && map != NULL);
    TEST("size=0 after create", cb_string_map_size(map) == 0);
    TEST("create NULL out error", cb_string_map_create(16, NULL) == CB_ERR_INVALID_ARGUMENT);

    CB_StringMap *m0 = NULL;
    TEST("create 0 cap ok", cb_string_map_create(0, &m0) == CB_OK);
    cb_string_map_destroy(m0);

    /* put and get */
    TEST("put 'hello'->'world'", cb_string_map_put(map, "hello", "world") == CB_OK);
    TEST("size=1", cb_string_map_size(map) == 1);
    const char *val;
    cb_string_map_get(map, "hello", &val);
    TEST("get 'hello' returns 'world'", val && strcmp(val, "world") == 0);

    /* get missing */
    cb_string_map_get(map, "nope", &val);
    TEST("get missing returns NULL", val == NULL);

    /* contains */
    int c;
    cb_string_map_contains(map, "hello", &c); TEST("contains 'hello' true", c == 1);
    cb_string_map_contains(map, "nope", &c);  TEST("contains 'nope' false", c == 0);

    /* overwrite */
    TEST("put overwrite", cb_string_map_put(map, "hello", "new") == CB_OK);
    cb_string_map_get(map, "hello", &val);
    TEST("get overwritten 'new'", strcmp(val, "new") == 0);
    TEST("size still 1", cb_string_map_size(map) == 1);

    /* remove */
    TEST("remove 'hello'", cb_string_map_remove(map, "hello") == CB_OK);
    TEST("size=0 after remove", cb_string_map_size(map) == 0);
    TEST("remove missing safe", cb_string_map_remove(map, "nope") == CB_OK);

    /* multiple entries */
    cb_string_map_put(map, "a", "1"); cb_string_map_put(map, "b", "2"); cb_string_map_put(map, "c", "3");
    TEST("size=3 after 3 puts", cb_string_map_size(map) == 3);
    cb_string_map_clear(map);
    TEST("size=0 after clear", cb_string_map_size(map) == 0);

    /* NULL value stores as empty */
    cb_string_map_put(map, "e", NULL);
    cb_string_map_get(map, "e", &val);
    TEST("NULL value stored as \"\"", val && strcmp(val, "") == 0);

    /* many inserts with rehash */
    cb_string_map_clear(map);
    int ok = 1;
    char k[32], v[32];
    for (int i = 0; i < 100; i++) {
        snprintf(k, sizeof(k), "key-%d", i);
        snprintf(v, sizeof(v), "val-%d", i);
        if (cb_string_map_put(map, k, v) != CB_OK) { ok = 0; break; }
    }
    TEST("100 inserts ok", ok == 1 && cb_string_map_size(map) == 100);

    int all_found = 1;
    for (int i = 0; i < 100; i++) {
        snprintf(k, sizeof(k), "key-%d", i);
        snprintf(v, sizeof(v), "val-%d", i);
        cb_string_map_get(map, k, &val);
        if (!val || strcmp(val, v) != 0) { all_found = 0; break; }
    }
    TEST("all 100 accessible after rehash", all_found == 1);

    /* collision handling */
    cb_string_map_clear(map);
    for (int i = 0; i < 50; i++) {
        snprintf(k, sizeof(k), "k%d", i);
        snprintf(v, sizeof(v), "v%d", i);
        cb_string_map_put(map, k, v);
    }
    int coll_ok = 1;
    for (int i = 0; i < 50; i++) {
        snprintf(k, sizeof(k), "k%d", i);
        snprintf(v, sizeof(v), "v%d", i);
        cb_string_map_get(map, k, &val);
        if (!val || strcmp(val, v) != 0) { coll_ok = 0; break; }
    }
    TEST("collision handling ok", coll_ok == 1);

    /* tombstone reuse */
    cb_string_map_clear(map);
    cb_string_map_put(map, "a", "1"); cb_string_map_put(map, "b", "2"); cb_string_map_put(map, "c", "3");
    cb_string_map_remove(map, "b");
    cb_string_map_get(map, "a", &val); TEST("tombstone 'a' ok", val && strcmp(val, "1") == 0);
    cb_string_map_get(map, "c", &val); TEST("tombstone 'c' ok", val && strcmp(val, "3") == 0);
    cb_string_map_put(map, "d", "4");
    cb_string_map_get(map, "d", &val); TEST("add after tombstone ok", val && strcmp(val, "4") == 0);

    /* rehash preserves keys */
    cb_string_map_clear(map);
    for (int i = 0; i < 50; i++) {
        snprintf(k, sizeof(k), "key-%d", i);
        snprintf(v, sizeof(v), "val-%d", i);
        cb_string_map_put(map, k, v);
    }
    int all_present = 1;
    for (int i = 0; i < 50; i++) {
        snprintf(k, sizeof(k), "key-%d", i);
        snprintf(v, sizeof(v), "val-%d", i);
        cb_string_map_get(map, k, &val);
        if (!val || strcmp(val, v) != 0) { all_present = 0; break; }
    }
    TEST("rehash preserves all keys", all_present == 1);

    /* rehash after tombstones */
    cb_string_map_clear(map);
    for (int i = 0; i < 12; i++) {
        snprintf(k, sizeof(k), "k%d", i);
        snprintf(v, sizeof(v), "v%d", i);
        cb_string_map_put(map, k, v);
    }
    /* Remove several to create tombstones */
    cb_string_map_remove(map, "k1");
    cb_string_map_remove(map, "k3");
    cb_string_map_remove(map, "k5");
    /* Add more to trigger rehash */
    for (int i = 12; i < 20; i++) {
        snprintf(k, sizeof(k), "k%d", i);
        snprintf(v, sizeof(v), "v%d", i);
        cb_string_map_put(map, k, v);
    }
    int after_rehash = 1;
    for (int i = 0; i < 20; i++) {
        if (i == 1 || i == 3 || i == 5) continue; /* removed */
        snprintf(k, sizeof(k), "k%d", i);
        snprintf(v, sizeof(v), "v%d", i);
        cb_string_map_get(map, k, &val);
        if (!val || strcmp(val, v) != 0) { after_rehash = 0; break; }
    }
    TEST("rehash after tombstones ok", after_rehash == 1);

    /* overwrite after rehash */
    cb_string_map_put(map, "k0", "new0");
    cb_string_map_get(map, "k0", &val);
    TEST("overwrite after rehash", val && strcmp(val, "new0") == 0);

    /* remove after rehash */
    cb_string_map_remove(map, "k0");
    cb_string_map_contains(map, "k0", &c);
    TEST("remove after rehash", c == 0);

    /* many inserts then many gets */
    CB_StringMap *big = NULL;
    cb_string_map_create(0, &big);
    ok = 1;
    for (int i = 0; i < 500; i++) {
        snprintf(k, sizeof(k), "big-%d", i);
        snprintf(v, sizeof(v), "value-%d", i);
        if (cb_string_map_put(big, k, v) != CB_OK) { ok = 0; break; }
    }
    TEST("500 inserts ok", ok == 1);
    int get_ok = 1;
    for (int i = 0; i < 500; i++) {
        snprintf(k, sizeof(k), "big-%d", i);
        snprintf(v, sizeof(v), "value-%d", i);
        cb_string_map_get(big, k, &val);
        if (!val || strcmp(val, v) != 0) { get_ok = 0; break; }
    }
    TEST("500 gets ok", get_ok == 1);
    cb_string_map_destroy(big);

    /* NULL safety */
    TEST("put NULL key error", cb_string_map_put(map, NULL, "x") == CB_ERR_INVALID_ARGUMENT);
    TEST("get NULL key error", cb_string_map_get(map, NULL, &val) == CB_ERR_INVALID_ARGUMENT);
    TEST("contains NULL key error", cb_string_map_contains(map, NULL, &c) == CB_ERR_INVALID_ARGUMENT);
    TEST("remove NULL key error", cb_string_map_remove(map, NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("put NULL map error", cb_string_map_put(NULL, "k", "v") == CB_ERR_INVALID_ARGUMENT);
    TEST("get NULL map error", cb_string_map_get(NULL, "k", &val) == CB_ERR_INVALID_ARGUMENT);
    TEST("size NULL=0", cb_string_map_size(NULL) == 0);

    cb_string_map_destroy(NULL);
    TEST("destroy NULL safe", 1);
    cb_string_map_destroy(map);
}