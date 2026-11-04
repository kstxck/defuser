#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.c"

unsigned int grid_rows, grid_columns;

char * grid_char_buffer;
#define grid_char_buffer_get_cell_char(row, column)            grid_char_buffer[row * grid_columns + column]
#define grid_char_buffer_set_cell_char(row, column, cell_char) grid_char_buffer[row * grid_columns + column] = cell_char

void grid_print_row_seperator_calculate();

struct Cell * grid;
#define grid_get_cell(row, column)       grid[row * grid_columns + column]
#define grid_set_cell(row, column, cell) grid[row * grid_columns + column] = cell; grid_char_buffer_set_cell_char(row, column, cell_get_char(cell))

#define grid_row(row)          (unsigned int row = 0; row < grid_rows; row++)
#define grid_column(column)    (unsigned int column = 0; column < grid_columns; column++)
#define grid_cell(row, column) grid_row(row) for grid_column(column)



void grid_set_rows_and_columns(unsigned int rows, unsigned int columns) {
    grid_rows    = rows;
    grid_columns = columns;

    free(grid);             grid             = (struct Cell *) calloc(sizeof(struct Cell), grid_rows * grid_columns);
    free(grid_char_buffer); grid_char_buffer = (char *)        calloc(sizeof(char),        grid_rows * grid_columns);

    grid_print_row_seperator_calculate();
}

void grid_populate(unsigned int safe_row, unsigned int safe_column, unsigned int mines) {
    for grid_cell(row, column) grid_char_buffer_set_cell_char(row, column, '?');

    if (mines == 0) return;
    do {
        unsigned int row = rand() % grid_rows, column = rand() % grid_columns;
        if (row == safe_row && column == safe_column) continue;

        struct Cell cell = grid_get_cell(row, column);
        if (cell.is_mine) continue; else {
            cell.is_mine = true;
            grid_set_cell(row, column, cell);
        }

        mines--;
    } while (mines);
}

// Grid Print Structure Caching
#define GRID_PRINT_OFFSET               "   "
#define GRID_PRINT_ROW_SEPERATOR_CORNER "+"
#define GRID_PRINT_ROW_SEPERATOR_COLUMN "---"
#define GRID_PRINT_ROW_SEPERATOR_COLUMN_LENGTH sizeof(GRID_PRINT_ROW_SEPERATOR_COLUMN) - 1
#define GRID_PRINT_COLUMN               " %c |"

char * grid_print_row_seperator;
void grid_print_row_seperator_calculate() {
    free(grid_print_row_seperator);
    unsigned int grid_print_row_seperator_length = 1 + (GRID_PRINT_ROW_SEPERATOR_COLUMN_LENGTH + 1) * grid_columns + 1;
    grid_print_row_seperator = malloc(grid_print_row_seperator_length * sizeof(char));

    grid_print_row_seperator[0] = GRID_PRINT_ROW_SEPERATOR_CORNER[0];
    grid_print_row_seperator[1] = '\0';
    for grid_column(column) {
        strcat(grid_print_row_seperator, GRID_PRINT_ROW_SEPERATOR_COLUMN);
        strcat(grid_print_row_seperator, GRID_PRINT_ROW_SEPERATOR_CORNER);
    }
    
}

void grid_print() {
    fputs(GRID_PRINT_OFFSET, stdout);
    fputs(" ", stdout);
    for grid_column(column) printf("%2d  ", column + 1);
    puts("");

    fputs(GRID_PRINT_OFFSET, stdout);
    puts(grid_print_row_seperator);
    for grid_row(row) {
        printf("%2d |", row + 1);
        for grid_column(column) {
            printf(GRID_PRINT_COLUMN, grid_char_buffer_get_cell_char(row, column));
        }
        puts("");
        fputs(GRID_PRINT_OFFSET, stdout);
        puts(grid_print_row_seperator);
    }
}