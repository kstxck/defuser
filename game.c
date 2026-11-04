#include "include/grid.c"

enum GameState {Inactive, Active, Lost, Won};
struct GameDifficulty {
    char * label;

    unsigned int grid_rows;
    unsigned int grid_columns;
    unsigned int grid_mines;
} game_difficulty_vector[] = {
    {"Easy",   9,  9,  10},
    {"Medium", 16, 16, 40},
    {"Hard",   30, 16, 99}
},

*Easy   = &game_difficulty_vector[0],
*Medium = &game_difficulty_vector[1],
*Hard   = &game_difficulty_vector[2];

void game_instruction_reveal(unsigned int row, unsigned int column);
void game_instruction_flag(unsigned int row, unsigned int column);
void game_instruction_help(unsigned int row, unsigned int column);

char game_instruction_alias_vector[] = {'r', 'f', 'h'};
void (*game_instruction_function_vector[])(unsigned int, unsigned int) = {
    game_instruction_reveal,
    game_instruction_flag,
    game_instruction_help,
};

void game_instruction_execute(char alias, unsigned int row, unsigned int column) {
    unsigned int instruction_index;
    
    for (instruction_index = 0; instruction_index < (sizeof(game_instruction_alias_vector)/sizeof(void *)); instruction_index++)
        if (game_instruction_alias_vector[instruction_index] == alias) break;

    game_instruction_function_vector[instruction_index](row, column);
}

enum GameState game_state = Inactive;
void game_main_loop() {
    static char instruction_alias = 'h';
    static unsigned int instruction_row = 0, instruction_column = 0;

    grid_print();
    printf("Enter Instruction (use 'h' for help): ");
    scanf("%c %u %u", &instruction_alias, &instruction_row, &instruction_column);

    game_instruction_execute(instruction_alias, instruction_row - 1, instruction_column - 1);
}

void game_start(struct GameDifficulty * game_difficulty) {
    printf("Setting up variables");
    grid_set_rows_and_columns(game_difficulty -> grid_rows, game_difficulty -> grid_columns);
    
    game_state = Active;
    printf("Entering the game_main_loop");
    while (game_state == Active) game_main_loop();

    puts((game_state == Lost) ? "You lost, a mine decimated you..." : "You won, you dodged every mine!");
}

void game_instruction_reveal(unsigned int row, unsigned int column) {
    if (grid_reveal(row, column)) game_state = Lost;
}
void game_instruction_flag(unsigned int row, unsigned int column) {
    struct Cell cell = grid_get_cell(row, column);

    cell.is_flagged = !cell.is_flagged;
    grid_set_cell(row, column, cell);
}
void game_instruction_help(unsigned int row, unsigned int column) {
    printf("r [row][column] reveal this cell");
    printf("f [row][column] flag this cell");
}

int main(void) {
    game_start(Easy);
    return 0;
}

