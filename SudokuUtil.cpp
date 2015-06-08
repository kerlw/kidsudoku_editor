#include "SudokuUtil.h"


#define COUNT(a) bcounts[(a & m_uMask)]
static SudokuSolver* s_pSolver = 0;
static SudokuGenerator* s_pGenerator = 0;

static const unsigned char bcounts[512] = {
       0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
       1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
       2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
       3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
       4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,5,6,6,7,6,7,7,8,6,7,7,8,7,8,8,9
};

static const ushort bmasks[9] = { 0x01, 0x03, 0x07, 0x0f, 0x01f, 0x03f, 0x07f, 0x0ff, 0x01ff};

SudokuSolver::SudokuSolver()
    : m_uMask(0x1FF),
      m_uNumbers(0) {
}

SudokuSolver::~SudokuSolver() {
}

SudokuSolver* SudokuSolver::getInstance() {
    if (!s_pSolver) {
        s_pSolver = new SudokuSolver();
    }
    return s_pSolver;
}


static unsigned short RC[MAX_ROWS] = { 0 };  // constrains for rows, bit 0 indicates whether this row has number 1 or not
static unsigned short CC[MAX_COLS] = { 0 };  // constrains for cols
static unsigned short BC[MAX_BLOCKS] = { 0 };  // constrains for blocks
static unsigned short GC[MAX_CELLS] = { 0 }; // constrains for each cell value

int SudokuSolver::search(int k, bool oneShot) {
    unsigned short constraint = GC[k];
    int count = COUNT(GC[k]);
    if (count == 0)
        return 0;

    if (k == m_uCols * m_uRows)
        return 1;

    int scnt = 0;
    if (count == 1)
        scnt += search(k+1, oneShot);

    for (int i = 0; i < count; i++) {
        unsigned short num = getValue(constraint, i);
        supose(k, num);
        scnt += search(k+1, oneShot);
        if (scnt > 0 && oneShot)
            return scnt;
        resume(k, num);
    }

    return scnt;
}

void SudokuSolver::setSize(QSize grid, QSize box) {
    m_uNumbers = grid.width() * grid.height();
    m_uRows = grid.height() * box.height();
    m_uCols = grid.width() * box.width();

    m_uMask = bmasks[m_uNumbers];
}

void SudokuSolver::supose(int k, unsigned short number) {
    int row = k / m_uCols;
    int col = k % m_uCols;



}

void SudokuSolver::resume(int k, unsigned short number) {
    int row = k / m_uCols;
    int col = k % m_uCols;

}

void SudokuSolver::try_solve(bool oneShot) {
    m_iSolutionCount = search(0, oneShot);
}

void SudokuSolver::setNumber(int row, int col, int value) {
    if (value <= 0 || value > 9)
        return;

    unsigned short mask = 1 << (value - 1);
    GC[row*m_uCols + col] = mask;

    RC[row] ^= mask;
    CC[col] ^= mask;
//    (row / m_uGridRows) *

}


SudokuGenerator* SudokuGenerator::getInstance() {
    if (!s_pGenerator) {
        s_pGenerator = new SudokuGenerator();
    }
    return s_pGenerator;
}

void SudokuGenerator::setSize(QSize grid, QSize box) {
    m_solver.setSize(grid, box);
    for (int index = 0; index < m_solver.m_uNumbers; index++) {
        GC[index] = bmasks[m_solver.m_uNumbers];
    }
}

bool SudokuGenerator::generate() {
    if (m_solver.m_uNumbers <= 0)
        return false;

label_random:
    int ROW = 0, COL = 0, BLOCK = 0;
    unsigned short value = qrand() % m_solver.m_uNumbers + 1;
    m_solver.setNumber(0, 0, value);
    ROW |= (1 << (value - 1));
    COL |= (1 << (value - 1));
    BLOCK |= (1 << (value - 1));

    //generate col 0's random permutation
    for (int i = 1; i < m_solver.m_uRows; i++) {
        value = qrand() % m_solver.m_uNumbers + 1;
        while ((value & COL) != 0)
            value = (value + 1) % m_solver.m_uNumbers + 1;
        m_solver.setNumber(i, 0, value);
        COL |= (1 << (value - 1));
        if (i < m_solver.m_uGridRows)
            BLOCK |= (1 << (value - 1));
    }

    //generate block 0's random permutation
    for (int i = 0; i < m_solver.m_uGridRows; i++) {
        for (int j = 1; j < m_solver.m_uGridCols; j++) {
            value = qrand() % m_solver.m_uNumbers + 1;
            while ((value & BLOCK) != 0)
                value = (value + 1) % m_solver.m_uNumbers + 1;
            m_solver.setNumber(i, j, value);
            BLOCK |= (1 << (value - 1));

            //for row 0, the set value should change the constrain for col too.
            if (i == 0)
                COL |= (1 << (value - 1));
       }
    }

    //generate row 0's random permutation
    for (int i = m_solver.m_uGridCols; i < m_solver.m_uCols; i++) {
        value = qrand() % m_solver.m_uNumbers + 1;
        while ((value & ROW) != 0)
            value = (value + 1) % m_solver.m_uNumbers + 1;
        m_solver.setNumber(i, 0, value);
        ROW |= (1 << (value - 1));
    }

    m_solver.try_solve();
    if (m_solver.m_iSolutionCount <= 0)
        goto label_random;

    return true;
}

