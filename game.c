#include "include/grid.c"

enum   GameState      {Inactive, Active, Lost, Won};
struct GameDifficulty {
    unsigned int grid_rows;
    unsigned int grid_columns;
    unsigned int grid_mines;
} game_difficulty_vector[] = {
    {9,  9,  10},
    {16, 16, 40},
    {30, 16, 99}
},
*Easy   = &game_difficulty_vector[0],
*Medium = &game_difficulty_vector[1],
*Hard   = &game_difficulty_vector[2];









enum GameState game_state = Inactive;

void game_clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void game_scan_row_and_column(unsigned int * row, unsigned int * column) {
    scanf("%u%u", row, column);
    (*row)--; (*column)--;
}

void game_start(struct GameDifficulty * game_difficulty);
void game_main_loop() {
    game_clear_screen();

    grid_print();
    
    char instruction_alias; scanf("%c", &instruction_alias);
    unsigned int instruction_row, instruction_column;
    switch (instruction_alias) {
        case 'r':
            game_scan_row_and_column(&instruction_row, &instruction_column);

            bool grid_reveal_result = grid_reveal(instruction_row, instruction_column);
            if (grid_reveal_result) game_state = Lost;

            break;
        case 'f':
            game_scan_row_and_column(&instruction_row, &instruction_column);

            struct Cell cell = grid_get_cell(instruction_row, instruction_column);
            cell.is_flagged = !cell.is_flagged;

            grid_set_cell(instruction_row, instruction_column, cell);
            break;
        default:
            break;
    };

    if ((grid_rows * grid_columns) - grid_opened_cells <= grid_mines) game_state = Won;
}

void game_start(struct GameDifficulty * game_difficulty) {
    grid_set_rows_and_columns(game_difficulty -> grid_rows, game_difficulty -> grid_columns);
    grid_mines = game_difficulty -> grid_mines;
    
    game_state = Active;
    while (game_state == Active) game_main_loop();

    puts((game_state == Lost) ? "You lost, a mine decimated you..." : "You won, you dodged every mine!");
}










int main(void) {
    unsigned int game_difficulty_index;
    printf("Select Difficulty (1 - %u): ", sizeof(game_difficulty_vector)/sizeof(struct GameDifficulty));
    scanf("%u", &game_difficulty_index);

    game_start(&game_difficulty_vector[game_difficulty_index - 1]);
}

