#include <stdio.h>
#include <string.h>
#include "cb_string_map.h"

int main(void) {
    printf("=== StringMap Example ===\n");

    CB_StringMap *map = NULL;
    cb_string_map_create(16, &map);

    cb_string_map_put(map, "name", "CoreBrick");
    cb_string_map_put(map, "version", "0.3.0");
    cb_string_map_put(map, "language", "C11");

    const char *val;
    cb_string_map_get(map, "name", &val);
    printf("name: %s\n", val);

    printf("Size: %zu\n", cb_string_map_size(map));

    cb_string_map_destroy(map);
    return 0;
}