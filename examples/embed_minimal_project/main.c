#include <stdio.h>
#include <string.h>
#include <corebrick.h>

/*
 * CoreBrick-C embed minimal demo
 *
 * Demonstrates that an external project using find_package(CoreBrick CONFIG REQUIRED)
 * can successfully link and use CoreBrick::corebrick.
 *
 * Required modules:
 *   - Arena
 *   - ByteBuffer
 *   - StringView
 *   - Hash (FNV-1a, CRC32)
 *   - StringMap
 */

static int test_arena(void) {
    CB_Arena *arena = NULL;
    if (cb_arena_create(256, &arena) != CB_OK) return 0;
    int *p = (int *)cb_arena_alloc(arena, sizeof(int));
    if (!p) { cb_arena_destroy(arena); return 0; }
    *p = 42;
    int ok = (*p == 42);
    cb_arena_destroy(arena);
    return ok;
}

static int test_buffer(void) {
    CB_Buffer buf;
    if (cb_buffer_init(&buf, 16) != CB_OK) return 0;
    cb_buffer_append(&buf, "Hello", 5);
    cb_buffer_append_cstr(&buf, " World");
    int ok = (buf.size == 11 && memcmp(buf.data, "Hello World", 11) == 0);
    cb_buffer_free(&buf);
    return ok;
}

static int test_string_view(void) {
    CB_StringView sv = cb_sv_from_cstr("  Hello  ");
    sv = cb_sv_trim_ascii(sv);
    int ok = (sv.len == 5 && memcmp(sv.data, "Hello", 5) == 0);
    return ok;
}

static int test_hash(void) {
    const char *data = "hello";
    uint64_t fnv1 = cb_hash_fnv1a64(data, strlen(data));
    uint64_t fnv2 = cb_hash_fnv1a64(data, strlen(data));
    uint32_t crc1 = cb_crc32(data, strlen(data));
    uint32_t crc2 = cb_crc32(data, strlen(data));
    /* consistent and non-zero */
    int ok = (fnv1 == fnv2 && fnv1 != 0 && crc1 == crc2 && crc1 != 0);
    return ok;
}

static int test_string_map(void) {
    CB_StringMap *map = NULL;
    if (cb_string_map_create(4, &map) != CB_OK) return 0;
    cb_string_map_put(map, "key", "value");
    const char *v = NULL;
    int ok = (cb_string_map_get(map, "key", &v) == CB_OK && strcmp(v, "value") == 0);
    cb_string_map_destroy(map);
    return ok;
}

int main(void) {
    int passed = 0, failed = 0;

    if (test_arena())        passed++; else { printf("FAIL: Arena\n");        failed++; }
    if (test_buffer())       passed++; else { printf("FAIL: ByteBuffer\n");   failed++; }
    if (test_string_view())  passed++; else { printf("FAIL: StringView\n");   failed++; }
    if (test_hash())         passed++; else { printf("FAIL: Hash\n");          failed++; }
    if (test_string_map())   passed++; else { printf("FAIL: StringMap\n");    failed++; }

    printf("CoreBrick embed demo: %d passed, %d failed\n", passed, failed);

    if (failed == 0) {
        printf("CoreBrick embed demo: OK\n");
        return 0;
    }
    return 1;
}