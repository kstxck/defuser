#include "include/grid.c"

int main(void) {
    grid_set_rows_and_columns(5, 5);
    grid_populate(0, 0, 1);
    
    grid_print();
    return 0;
}