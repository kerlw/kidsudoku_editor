#ifndef __SUDOKU_UTIL__
#define __SUDOKU_UTIL__

#define MAX_ROWS 9
#define MAX_COLS 9
#define MAX_BLOCKS 9
#define MAX_CELLS 81
#define MAX_NUMBER 9

#include <QSize>
#include <time.h>

class SudokuSolver {
    friend class SudokuGenerator;
public:
    static SudokuSolver* getInstance();
    virtual ~SudokuSolver();

    void setSize(QSize grid, QSize box);

    bool supose(int row, int col, unsigned short number);
    void resume(int row, int col, unsigned short number);

private:
    SudokuSolver();

    void initConstraints();
    int search(int k, bool oneShot = true);
    void try_solve(bool oneShot = true);
    void setNumber(int r, int c, int value);
    void unsetNumber(int row, int col);

    unsigned short getValue(unsigned short constraint, int index) {
        for (int i = 0; i < m_uNumbers; i++) {
            unsigned short mask = 1 << i;
            if ((constraint & mask) != 0 && index-- == 0)
                return (i + 1);
        }
        return 0;
    }

    void printBox();

private:
    unsigned short m_uMask;

    unsigned short m_uNumbers;
    unsigned short m_uRows;
    unsigned short m_uCols;

    unsigned short m_uGridRows;
    unsigned short m_uGridCols;

    unsigned short m_uGridsInRow;
    unsigned short m_uGridsInCol;

    int m_iSolutionCount;
    unsigned char m_pSolution[MAX_CELLS];

    unsigned short m_pRowConstraints[MAX_ROWS];    // constrains for rows, bit 0 indicates whether this row has number 1 or not
    unsigned short m_pColConstraints[MAX_COLS];    // constrains for cols
    unsigned short m_pBlockConstraints[MAX_BLOCKS];  // constrains for blocks
    unsigned short m_pCellConstraints[MAX_CELLS];   // constrains for each cell value

};

class SudokuGenerator {
public:
    static SudokuGenerator* getInstance();
    virtual ~SudokuGenerator() {}

    void setSize(QSize grid, QSize box);
    bool generate();
    unsigned char* getPuzzle() { return m_solver.m_pSolution; }

private:
    SudokuGenerator() { qsrand(time(0));   }

private:
    SudokuSolver m_solver;
};

#endif // __SUDOKU_UTIL__

