#include <stdio.h>
#include "cb_arena.h"

int main(void) {
    printf("=== Arena Example ===\n");

    CB_Arena *arena = NULL;
    CB_Error err = cb_arena_create(4096, &arena);
    if (err != CB_OK) {
        printf("Failed to create arena: %s\n", cb_error_string(err));
        return 1;
    }

    int *values = (int *)cb_arena_alloc(arena, 10 * sizeof(int));
    if (values == NULL) {
        printf("Allocation failed\n");
        cb_arena_destroy(arena);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        values[i] = i * 100;
    }

    printf("Values: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    printf("Arena used: %zu / %zu bytes\n",
           cb_arena_used(arena), cb_arena_capacity(arena));

    cb_arena_reset(arena);
    printf("After reset - used: %zu / %zu bytes\n",
           cb_arena_used(arena), cb_arena_capacity(arena));

    char *text = (char *)cb_arena_alloc(arena, 32);
    if (text != NULL) {
        snprintf(text, 32, "Hello from arena!");
        printf("Text: %s\n", text);
    }

    cb_arena_destroy(arena);
    return 0;
}