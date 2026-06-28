#include <stdio.h>
#include <string.h>
#include "corebrick.h"

int main(void) {
    printf("=== CoreBrick-C Minimal Example ===\n\n");

    /* ---------- Version ---------- */
    printf("Version: %s\n\n", COREBRICK_VERSION_STRING);

    /* ---------- Arena ---------- */
    {
        CB_Arena *arena = NULL;
        if (cb_arena_create(1024, &arena) == CB_OK) {
            int *p = (int *)cb_arena_alloc(arena, sizeof(int));
            if (p) {
                *p = 42;
                printf("Arena: allocated int = %d\n", *p);
            }
            printf("Arena used: %zu / %zu\n", cb_arena_used(arena), cb_arena_capacity(arena));
            cb_arena_destroy(arena);
        }
    }

    /* ---------- ByteBuffer ---------- */
    {
        CB_Buffer buf;
        if (cb_buffer_init(&buf, 16) == CB_OK) {
            cb_buffer_append(&buf, "Hello", 5);
            cb_buffer_append_u8(&buf, '!');
            cb_buffer_append_cstr(&buf, " World");
            printf("Buffer: [%.*s] (len=%zu, cap=%zu)\n",
                   (int)buf.size, (const char *)buf.data, buf.size, buf.capacity);
            cb_buffer_free(&buf);
        }
    }

    /* ---------- StringView ---------- */
    {
        CB_StringView sv = cb_sv_from_cstr("  Hello, World!  ");
        sv = cb_sv_trim_ascii(sv);
        printf("StringView trimmed: \"");
        for (size_t i = 0; i < sv.len; i++) putchar(sv.data[i]);
        printf("\" (len=%zu)\n", sv.len);

        CB_StringView needle = cb_sv_from_cstr("World");
        size_t pos = 0;
        if (cb_sv_find(sv, needle, &pos)) {
            printf("StringView find 'World' at pos=%zu\n", pos);
        }

        /* split */
        CB_StringView csv = cb_sv_from_cstr("a,b,c");
        CB_SplitIter iter = cb_sv_split(csv, ',');
        CB_StringView part;
        printf("StringView split:");
        while (cb_sv_split_next(&iter, &part)) {
            printf(" \"");
            for (size_t i = 0; i < part.len; i++) putchar(part.data[i]);
            printf("\"");
        }
        printf("\n");
    }

    /* ---------- Vector ---------- */
    {
        CB_Vector vec;
        if (cb_vector_init(&vec, sizeof(int), 4) == CB_OK) {
            int vals[] = {10, 20, 30};
            for (int i = 0; i < 3; i++) {
                cb_vector_push(&vec, &vals[i]);
            }
            printf("Vector: [");
            for (size_t i = 0; i < 3; i++) {
                int *v = (int *)cb_vector_get_ptr(&vec, i);
                if (v) printf("%s%d", i > 0 ? ", " : "", *v);
            }
            printf("] (size=%zu)\n", vec.size);
            cb_vector_free(&vec);
        }
    }

    /* ---------- Hash ---------- */
    {
        const char *test = "hello";
        uint64_t fnv = cb_hash_fnv1a64(test, strlen(test));
        uint32_t crc = cb_crc32(test, strlen(test));
        printf("Hash FNV-1a(hello) = %llu\n", (unsigned long long)fnv);
        printf("Hash CRC32(hello)  = 0x%08X\n", crc);
    }

    /* ---------- BitSet ---------- */
    {
        CB_BitSet bs;
        if (cb_bitset_init(&bs, 16) == CB_OK) {
            cb_bitset_set(&bs, 3);
            cb_bitset_set(&bs, 7);
            int v = 0;
            cb_bitset_get(&bs, 3, &v);
            printf("BitSet: bit 3 = %d\n", v);
            cb_bitset_get(&bs, 5, &v);
            printf("BitSet: bit 5 = %d\n", v);
            cb_bitset_free(&bs);
        }
    }

    /* ---------- RingBuffer ---------- */
    {
        CB_Ring ring;
        if (cb_ring_init(&ring, sizeof(int), 4) == CB_OK) {
            int vals[] = {100, 200, 300};
            for (int i = 0; i < 3; i++) {
                cb_ring_push(&ring, &vals[i]);
            }
            printf("Ring: empty=%d, full=%d\n", cb_ring_is_empty(&ring), cb_ring_is_full(&ring));
            int out = 0;
            if (cb_ring_peek(&ring, &out) == CB_OK) {
                printf("Ring peek = %d\n", out);
            }
            cb_ring_free(&ring);
        }
    }

    /* ---------- Bloom Filter ---------- */
    {
        CB_Bloom bloom;
        if (cb_bloom_init(&bloom, 64, 3) == CB_OK) {
            cb_bloom_add(&bloom, "key1", 4);
            cb_bloom_add(&bloom, "", 0); /* empty key */
            int maybe = 0;
            cb_bloom_might_contain(&bloom, "key1", 4, &maybe);
            printf("Bloom: key1 might_contain = %d\n", maybe);
            cb_bloom_might_contain(&bloom, "key2", 4, &maybe);
            printf("Bloom: key2 might_contain = %d (probably 0)\n", maybe);
            cb_bloom_free(&bloom);
        }
    }

    /* ---------- StringMap ---------- */
    {
        CB_StringMap *map = NULL;
        if (cb_string_map_create(8, &map) == CB_OK) {
            cb_string_map_put(map, "name", "Alice");
            cb_string_map_put(map, "city", "Tokyo");
            const char *v = NULL;
            if (cb_string_map_get(map, "name", &v) == CB_OK) {
                printf("StringMap: name = %s\n", v);
            }
            printf("StringMap size = %zu\n", cb_string_map_size(map));
            cb_string_map_destroy(map);
        }
    }

    /* ---------- Timer ---------- */
    {
        CB_Timer timer;
        (void)timer; /* struct init not needed */
        cb_timer_start(&timer);
        volatile int x = 0;
        for (int i = 0; i < 100000; i++) x += i;
        double elapsed = cb_timer_elapsed_ns(&timer) / 1000.0;
        printf("Timer: small loop took %.2f us\n", elapsed);
    }

    /* ---------- File ---------- */
    {
        const char *tmp_path = "minimal_example_tmp.txt";
        CB_Error err = cb_file_write_all(tmp_path, "Hello from CoreBrick!", 21);
        printf("File write_all: %s\n", err == CB_OK ? "OK" : "FAILED");

        uint64_t fsize = 0;
        if (cb_file_size(tmp_path, &fsize) == CB_OK) {
            printf("File size: %llu bytes\n", (unsigned long long)fsize);
        }

        CB_Buffer buf;
        if (cb_buffer_init(&buf, 16) == CB_OK) {
            if (cb_file_read_all(tmp_path, &buf) == CB_OK) {
                printf("File read_all: [%.*s]\n", (int)buf.size, (const char *)buf.data);
            }
            cb_buffer_free(&buf);
        }

        remove(tmp_path);
    }

    printf("\n=== All modules demonstrated successfully! ===\n");
    return 0;
}