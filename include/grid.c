#include <stdlib.h>
#include <stdio.h>
#include "cell.c"

unsigned int grid_rows, grid_columns;

char * grid_char_buffer;
#define grid_char_buffer_get_cell_char(row, column)            *(grid_char_buffer + row * grid_columns + column)
#define grid_char_buffer_set_cell_char(row, column, cell_char) *(grid_char_buffer + row * grid_columns + column) = cell_char

struct Cell * grid;
#define grid_get_cell(row, column)       *(grid + row * grid_columns + column)
#define grid_set_cell(row, column, cell) *(grid + row * grid_columns + column) = cell; grid_char_buffer_set_cell_char(row, column, cell_get_char(cell))

#define grid_row(row)          (unsigned int row = 0; row < grid_rows; row++)
#define grid_column(column)    (unsigned int column = 0; column < grid_columns; column++)
#define grid_cell(row, column) grid_row(row) for grid_column(column)

void grid_set_rows_and_columns(unsigned int rows, unsigned int columns) {
    grid_rows    = rows;
    grid_columns = columns;

    free(grid);             grid             = (struct Cell *) calloc(sizeof(struct Cell), rows * columns);
    free(grid_char_buffer); grid_char_buffer = (char *)        calloc(sizeof(char),        rows * columns);
}

void grid_populate(int rows, int columns, int safe_row, int safe_column, int mines) {
    for grid_cell(row, column) grid_char_buffer_set_cell_char(row, column, '?');
    do {
        int row = rand() % grid_rows, column = rand() % grid_columns;
        if (row == safe_row && column == safe_column) continue;

        struct Cell cell = grid_get_cell(row, column);
        if (cell.is_mine) continue; else {
            cell.is_mine = true;
            grid_set_cell(row, column, cell);
        }

        mines--;
    } while (mines);
}

void grid_print() {
    grid_set_rows_and_columns(5, 5);
    for grid_cell(row, column) printf("%c", grid_char_buffer_get_cell_char(row, column));
}