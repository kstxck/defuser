#include "include/grid.c"

enum GameState {Inactive, Active, Lost, Won};
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

void game_scan_row_and_column(unsigned int * row, unsigned int * column) {
    scanf("%u %u", row, column);
    (*row)--, (*column)--;
}
void game_main_loop() {
    char alias = ' ';

    grid_print();
    printf("Enter Instruction: ");
    scanf("%c", &alias);
    
    switch (alias) {
        case 'r':
            unsigned int reveal_row, reveal_column;
            game_scan_row_and_column(&reveal_row, &reveal_column);

            if (grid_reveal(reveal_row, reveal_column)) game_state = Lost;
            break;
        case 'f':
            unsigned int flag_row, flag_column;
            game_scan_row_and_column(&flag_row, &flag_column);

            struct Cell cell = grid_get_cell(flag_row, flag_column);

            cell.is_flagged = !cell.is_flagged;
            grid_set_cell(flag_row, flag_column, cell);

            break;
        default:
            printf("r [row][column] - reveal cell\n");
            printf("f [row][column] - flag cell\n\n");
            printf("Press any key to continue\n");
            char key; scanf("%c", &key);
            break;
    }    
}

void game_start(struct GameDifficulty * game_difficulty) {
    grid_mines = game_difficulty -> grid_mines;
    grid_set_rows_and_columns(game_difficulty -> grid_rows, game_difficulty -> grid_columns);
    
    game_state = Active;
    while (game_state == Active) {game_main_loop();}

    puts((game_state == Lost) ? "You lost, a mine decimated you..." : "You won, you dodged every mine!");
}

int main(void) {
    game_start(Easy);
}

