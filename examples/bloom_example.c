#include <stdio.h>
#include <string.h>
#include "cb_bloom.h"

int main(void) {
    printf("=== Bloom Filter Example ===\n");

    CB_Bloom bloom;
    cb_bloom_init(&bloom, 256, 3);

    const char *items[] = {"apple", "banana", "cherry"};
    for (int i = 0; i < 3; i++) {
        cb_bloom_add(&bloom, items[i], strlen(items[i]));
        printf("Added: %s\n", items[i]);
    }

    int maybe;
    cb_bloom_might_contain(&bloom, "apple", 5, &maybe);
    printf("Contains 'apple': %s\n", maybe ? "maybe" : "no");

    cb_bloom_might_contain(&bloom, "durian", 6, &maybe);
    printf("Contains 'durian': %s\n", maybe ? "maybe (FP?)" : "no");

    printf("Inserted count: %zu\n", bloom.inserted_count);

    cb_bloom_free(&bloom);
    return 0;
}