#include "SudokuUtil.h"

static SudokuSolver* s_pSolver = 0;

SudokuSolver* SudokuSolver::getInstance() {
    if (!s_pSolver) {
        s_pSolver = new SudokuSolver();
    }
    return s_pSolver;
}


unsigned short RC[9] = { 0 };  // constrains for rows, bit 0  indicates whether this row has number 1 or not
unsigned short CC[9] = { 0 };  // constrains for cols
unsigned short BC[9] = {0};    // constrains for blocks
unsigned short GC[81] = { 0 }; // constrains for each cell value

void search(int k) {
    unsigned short constraint = GC[k];
}

void try_solve() {

}

void setNumber(int row, int col, int value) {
    if (value <= 0 || value > 9)
        return;

    unsigned short mask = 1 << (value - 1);
    CELL[row*9 + col] = value;


}
