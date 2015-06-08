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
    void supose(int k, unsigned short number);
    void resume(int k, unsigned short number);

private:
    SudokuSolver();
    int search(int k, bool oneShot = true);
    void try_solve(bool oneShot = true);
    void setNumber(int r, int c, int value);

    unsigned short getValue(unsigned short constraint, int index) {
        for (int i = 0; i < m_uNumbers; i++) {
            unsigned short mask = 1 << i;
            if (constraint & mask != 0 && index-- == 0)
                return (i + 1);
        }
        return 0;
    }

private:
    unsigned short m_uMask;

    unsigned short m_uNumbers;
    unsigned short m_uRows;
    unsigned short m_uCols;

    unsigned short m_uGridRows;
    unsigned short m_uGridCols;

    int m_iSolutionCount;

};

class SudokuGenerator {
public:
    static SudokuGenerator* getInstance();
    virtual ~SudokuGenerator() {}

    void setSize(QSize grid, QSize box);
    bool generate();

private:
    SudokuGenerator() { qsrand(time(0));   }

private:
    SudokuSolver m_solver;
};

#endif // __SUDOKU_UTIL__

