#include <stdbool.h>

struct Cell {
    bool is_mine            : 1;
    bool is_revealed        : 1;
    bool is_flagged         : 1;
    unsigned int padding    : 1;
    unsigned int neighbours : 4;
};