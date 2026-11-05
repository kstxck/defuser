#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.c"

void grid_print_row_seperator_calculate(); // NOTE: Defined at the bottom of the file

unsigned int grid_rows, grid_columns;
unsigned int grid_mines;
unsigned int grid_opened_cells;
bool grid_populated;

char * grid_char_buffer;
#define grid_char_buffer_get_cell_char(row, column)            grid_char_buffer[row * grid_columns + column]
#define grid_char_buffer_set_cell_char(row, column, cell_char) grid_char_buffer[row * grid_columns + column] = cell_char

struct Cell * grid;
#define grid_get_cell(row, column)       grid[row * grid_columns + column]
#define grid_set_cell(row, column, cell) grid[row * grid_columns + column] = cell; grid_char_buffer_set_cell_char(row, column, cell_get_char(cell))

#define grid_row(row)          (unsigned int row = 0; row < grid_rows; row++)
#define grid_column(column)    (unsigned int column = 0; column < grid_columns; column++)
#define grid_cell(row, column) grid_row(row) for grid_column(column)

int grid_get_neighbours(unsigned int row, unsigned int column, unsigned int neighbours[static 8][2]) {
    unsigned int neigbour_count = 0;

    unsigned int start_row    = (row == 0)    ? row    : row    - 1;
    unsigned int start_column = (column == 0) ? column : column - 1;
    unsigned int stop_row     = (row + 1    < grid_rows - 1)    ? (row + 1)    : (grid_rows - 1);
    unsigned int stop_column  = (column + 1 < grid_columns - 1) ? (column + 1) : (grid_columns - 1);

    for (unsigned int neigbour_row = start_row;        neigbour_row <= stop_row;        neigbour_row++)
    for (unsigned int neighbour_column = start_column; neighbour_column <= stop_column; neighbour_column++) {
        if (neigbour_row == row && neighbour_column == column) continue;

        neighbours[neigbour_count][0] = neigbour_row;
        neighbours[neigbour_count][1] = neighbour_column;

        neigbour_count++;
    }

    return neigbour_count;
}










void grid_set_rows_and_columns(unsigned int rows, unsigned int columns) {
    grid_rows = rows, grid_columns = columns, grid_populated = false, grid_opened_cells = 0;

    free(grid);             grid             = (struct Cell *) calloc(sizeof(struct Cell), grid_rows * grid_columns);
    free(grid_char_buffer); grid_char_buffer = (char *)        calloc(sizeof(char),        grid_rows * grid_columns);

    for grid_cell(row, column) grid_char_buffer_set_cell_char(row, column, cell_get_char(grid_get_cell(row, column)));

    grid_print_row_seperator_calculate();
}

void grid_populate(unsigned int safe_row, unsigned int safe_column) {
    unsigned int mines = grid_mines;

    printf("Placing %u mines, safe row is %u and safe column is %u\n", grid_mines, safe_row, safe_column);
    if (mines == 0) return;
    do {
        unsigned int row = rand() % grid_rows, column = rand() % grid_columns;
        if (row == safe_row && column == safe_column) continue;

        struct Cell cell = grid_get_cell(row, column);
        if (cell.is_mine) continue; else {
            cell.is_mine = true;
            grid_set_cell(row, column, cell);
            printf("Mine placed @%u,%u\n", row, column);
        }

        unsigned int neighbours[8][2];
        unsigned int neighbour_count = grid_get_neighbours(row, column, neighbours);
        
        for (unsigned int neighbour_index = 0; neighbour_index < neighbour_count; neighbour_index++) {
            unsigned int neighbour_row    = neighbours[neighbour_index][0];
            unsigned int neighbour_column = neighbours[neighbour_index][1];

            struct Cell neighbour_cell = grid_get_cell(neighbour_row, neighbour_column);
            cell.mine_neighbours += 1;
            grid_set_cell(neighbour_row, neighbour_column, cell);
            printf("Incrementing mine_neighbours @%u,%u, new value: %u\n", neighbour_row, neighbour_column, cell.mine_neighbours);
        }

        mines--;
    } while (mines);

    grid_populated = true;
}










bool grid_reveal(unsigned int row, unsigned int column) {
    if (!grid_populated) {
        grid_populate(row, column);
    }

    struct Cell cell = grid_get_cell(row, column);

    if (cell.is_revealed) return false;
    
    cell.is_revealed = true;
    grid_set_cell(row, column, cell); grid_opened_cells++;

    if (cell.is_mine) {
        printf("Mine detected at @%u,%u, Mine Value: %b", row, column, cell.is_mine);
        return true;
    }
    if (cell.mine_neighbours == 0) {
        unsigned int neighbours[8][2];
        unsigned int neighbour_count = grid_get_neighbours(row, column, neighbours);

        for (unsigned int neighbour_index = 0; neighbour_index < neighbour_count; neighbour_index++) {
            int neighbour_row = neighbours[neighbour_index][0], neighbour_column = neighbours[neighbour_index][1];
            grid_reveal(neighbour_row, neighbour_column);
        }
    }

    return false;
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
    for grid_column(column) printf(" %d  ", column + 1);
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