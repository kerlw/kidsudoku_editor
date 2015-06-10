#include "QStageData.h"
#include <QDataStream>

#include "SudokuUtil.h"

static const int MAX_SIZE = 9 * 9;

const unsigned short QStageData::SELECTED_MASK = (1 << 15);

QStageData::QStageData()
{
    m_pNumbers = new int[MAX_SIZE];
    memset(m_pNumbers, 0, sizeof(int)*MAX_SIZE);
}

QStageData::~QStageData()
{

}

QStageData* QStageData::create(QSize gridSize, QSize boxSize) {
    QStageData* ret = new QStageData();
    Q_ASSERT(gridSize.width() > 0 && gridSize.width() <= 3);
    Q_ASSERT(gridSize.height() > 0 && gridSize.height() <= 3);
    Q_ASSERT(boxSize.width() > 0 && boxSize.width() <= 3);
    Q_ASSERT(boxSize.height() > 0 && boxSize.height() <= 3);

    ret->m_sizeGrid = gridSize;
    ret->m_sizeBox = boxSize;
    ret->m_iCellCount = gridSize.width() * gridSize.height() * boxSize.width() * boxSize.height();

    return ret;
}

QStageData* QStageData::create(QStageData *src) {
    QStageData* ret = new QStageData();
    Q_ASSERT(src);
    ret->m_sizeGrid = src->m_sizeGrid;
    ret->m_sizeBox = src->m_sizeBox;
    ret->m_iCellCount = src->m_iCellCount;
    Q_ASSERT(ret->m_iCellCount <= MAX_SIZE);
    memcpy(ret->m_pNumbers, src->m_pNumbers, sizeof(int)*ret->m_iCellCount);

    return ret;
}

QString QStageData::toQString() {
    QString str;
    str.sprintf("Stage grid size: %dx%d box size: %dx%d",  m_sizeGrid.width(), m_sizeGrid.height(), m_sizeBox.width(), m_sizeBox.height());
    return str;
}

int QStageData::numberAt(int row, int col) {
    int index = row * m_sizeBox.width() * m_sizeGrid.width() + col;
    if (index < 0 || index >= m_iCellCount)
        return -1;

    int ret = m_pNumbers[index];
    if (ret != 0 && m_setKnownCells.find(index) != m_setKnownCells.end())
        ret |= SELECTED_MASK;

    return ret;
}

void QStageData::resetRowsPerGrid(int value) {
    QSize size(m_sizeGrid.width(), value);
    resetGridSize(size);
}

void QStageData::resetColsPerGrid(int value) {
    QSize size(value, m_sizeGrid.height());
    resetGridSize(size);
}

void QStageData::resetGridsInRow(int value) {
    QSize size(m_sizeBox.width(), value);
    resetBoxSize(size);
}

void QStageData::resetGridsInCol(int value) {
    QSize size(value, m_sizeBox.height());
    resetBoxSize(size);
}

void QStageData::resetGridSize(const QSize& size) {
    m_sizeGrid = size;
    m_iCellCount = m_sizeGrid.width() * m_sizeGrid.height() * m_sizeBox.width() * m_sizeBox.height();
    m_setKnownCells.clear();
    memset(m_pNumbers, 0, sizeof(int) * m_iCellCount);
}

void QStageData::resetBoxSize(const QSize& size) {
    m_sizeBox = size;
    m_iCellCount = m_sizeGrid.width() * m_sizeGrid.height() * m_sizeBox.width() * m_sizeBox.height();
    m_setKnownCells.clear();
    memset(m_pNumbers, 0, sizeof(int) * m_iCellCount);
}

int QStageData::lengthInByte() const {
    int ret = 4;    //id
    ret += 4;       //TODO speicified resources
    ret += 4;       //grid size & box size
    ret += 2;       //count of cells
    ret += m_iCellCount;    //number in each cell
    ret += 2;       //count of known cells
    ret += m_setKnownCells.size();  //known cells position

    return ret;
}

void QStageData::updateData() {
    SudokuGenerator* generator = SudokuGenerator::getInstance();
    generator->setSize(m_sizeGrid, m_sizeBox);
    generator->generate();
    unsigned char* puzzle = generator->getPuzzle();
    for (int i = 0; i < m_iCellCount; i++) {
        m_pNumbers[i] = puzzle[i];
    }
}

void QStageData::toggleSelected(int row, int col) {
    int index = row * m_sizeBox.width() * m_sizeGrid.width() + col;
    if (index < 0 || index >= m_iCellCount)
        return;

    //if this cell has not been associated with a valid number, ignore
    if (m_pNumbers[index] <= 0)
        return;

    if (m_setKnownCells.find(index) != m_setKnownCells.end())
        m_setKnownCells.erase(index);
    else
        m_setKnownCells.insert(index);
}

void QStageData::initSolver(SudokuSolver *solver) {
    if (!solver)
        return;

    int cols = m_sizeGrid.width() * m_sizeBox.width();
    solver->setSize(m_sizeGrid, m_sizeBox);
    std::set<int>::iterator it = m_setKnownCells.begin();
    while (it != m_setKnownCells.end()) {
        int row = (*it) / cols;
        int col = (*it) % cols;
        solver->setNumber(row, col, m_pNumbers[*it]);
        ++it;
    }
}

QDataStream& operator<<(QDataStream& stream, const QStageData& data) {
    stream << quint32(data.lengthInByte());
    stream << 1;    //TODO id
    stream << 0;    //TODO specified resource
    stream << quint8(data.m_sizeGrid.height());
    stream << quint8(data.m_sizeGrid.width());
    stream << quint8(data.m_sizeBox.height());
    stream << quint8(data.m_sizeBox.width());

    //solution
    stream << quint16(data.m_iCellCount);
    for (int i = 0; i < data.m_iCellCount; i++) {
        stream << quint8(data.m_pNumbers[i]);
    }

    //known cells
    stream << quint16(data.m_setKnownCells.size());
    std::set<int>::const_iterator it = data.m_setKnownCells.begin();
    while (it != data.m_setKnownCells.end()) {
        stream << quint8(*it);
        ++it;
    }
    return stream;
}

QDataStream& operator>>(QDataStream& stream, QStageData& data) {
    quint32 i32;
    quint16 i16;
    quint8 i8;

    stream >> i32;
    stream >> i32;
    if (i32 > 0)
        stream.skipRawData(i32); //TODO read specified resource

    stream >> i8;
    data.m_sizeGrid.setHeight(i8);
    stream >> i8;
    data.m_sizeGrid.setWidth(i8);
    stream >> i8;
    data.m_sizeBox.setHeight(i8);
    stream >> i8;
    data.m_sizeBox.setWidth(i8);

    stream >> i16;
    data.m_iCellCount = i16;
    int count = i16;
    memset(data.m_pNumbers, 0, sizeof(int)*MAX_SIZE);
    for (int i = 0; i < count; i++) {
        stream >> reinterpret_cast<quint8&>(data.m_pNumbers[i]);
    }

    stream >> i16;
    count = i16;
    for (int i = 0; i < count; i++) {
        stream >> i8;
        data.m_setKnownCells.insert(i8);
    }

    return stream;
}

