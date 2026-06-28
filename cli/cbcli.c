#include <stdio.h>
#include <string.h>
#include "corebrick.h"

static void print_version(void) {
    printf("CoreBrick-C v0.1.0\n");
}

static void print_usage(void) {
    printf("CoreBrick-C CLI\n");
    printf("Usage: cbcli <command>\n\n");
    printf("Commands:\n");
    printf("  version       Show version information\n");
    printf("  arena-test    Run arena demo\n");
    printf("  buffer-test   Run buffer demo\n");
    printf("  sv-test       Run string view demo\n");
    printf("  help          Show this help message\n");
}

static void arena_demo(void) {
    printf("=== Arena Demo ===\n");
    CB_Arena *arena = NULL;
    CB_Error err = cb_arena_create(1024, &arena);
    if (err != CB_OK) {
        printf("Failed to create arena: %s\n", cb_error_string(err));
        return;
    }

    int *numbers = (int *)cb_arena_alloc(arena, 5 * sizeof(int));
    if (numbers != NULL) {
        for (int i = 0; i < 5; i++) {
            numbers[i] = i * 10;
        }
        printf("Allocated 5 ints via arena: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    }

    printf("Arena used: %zu / %zu\n", cb_arena_used(arena), cb_arena_capacity(arena));
    cb_arena_destroy(arena);
}

static void buffer_demo(void) {
    printf("=== Buffer Demo ===\n");
    CB_Buffer buf;
    CB_Error err = cb_buffer_init(&buf, 16);
    if (err != CB_OK) {
        printf("Failed to init buffer: %s\n", cb_error_string(err));
        return;
    }

    err = cb_buffer_append_cstr(&buf, "Hello, ");
    if (err == CB_OK) {
        err = cb_buffer_append_cstr(&buf, "CoreBrick!");
    }
    if (err == CB_OK) {
        buf.data[buf.size] = '\0';
        printf("Buffer content: %s (size=%zu, capacity=%zu)\n",
               (const char *)buf.data, buf.size, buf.capacity);
    }

    cb_buffer_free(&buf);
}

static void sv_demo(void) {
    printf("=== StringView Demo ===\n");
    CB_StringView sv = cb_sv_from_cstr("  Hello, CoreBrick!  ");
    printf("Original: \"%.*s\" (len=%zu)\n", (int)sv.len, sv.data, sv.len);

    CB_StringView trimmed = cb_sv_trim_ascii(sv);
    printf("Trimmed: \"%.*s\" (len=%zu)\n", (int)trimmed.len, trimmed.data, trimmed.len);

    CB_StringView prefix = cb_sv_from_cstr("Hello");
    printf("Starts with \"Hello\": %s\n",
           cb_sv_starts_with(trimmed, prefix) ? "yes" : "no");

    int64_t val;
    CB_StringView num = cb_sv_from_cstr("  42  ");
    if (cb_sv_to_i64(num, &val) == CB_OK) {
        printf("Parse \"42\": %lld\n", (long long)val);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    if (strcmp(argv[1], "version") == 0) {
        print_version();
    } else if (strcmp(argv[1], "arena-test") == 0) {
        arena_demo();
    } else if (strcmp(argv[1], "buffer-test") == 0) {
        buffer_demo();
    } else if (strcmp(argv[1], "sv-test") == 0) {
        sv_demo();
    } else {
        print_usage();
    }

    return 0;
}