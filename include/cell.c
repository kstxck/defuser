#include <stdbool.h>

struct Cell {
    bool is_mine     : 1;
    bool is_revealed : 1;
    bool is_flagged  : 1;

    unsigned int mine_neighbours : 4;
};

char cell_get_ascii(struct Cell cell) {
    if (cell.is_revealed) {
        if (cell.mine_neighbours) return '0' + cell.mine_neighbours;
        else {
            if (cell.is_mine) return 'X';
            else              return ' ';
        }
    }
    return (cell.is_flagged) ? '!' : '?';
}