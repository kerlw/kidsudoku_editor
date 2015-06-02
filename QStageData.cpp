#include "QStageData.h"

static const int MAX_SIZE = 9 * 9;

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

QString QStageData::toQString() {
    QString str;
    str.sprintf("Stage grid size: %dx%d box size: %dx%d",  m_sizeGrid.width(), m_sizeGrid.height(), m_sizeBox.width(), m_sizeBox.height());
    return str;
}

int QStageData::numberAt(int index) {
    if (index < 0 || index >= m_iCellCount)
        return -1;

    return m_pNumbers[index];
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
    //TODO move numbers to the new place
    m_sizeGrid = size;
    m_iCellCount = m_sizeGrid.width() * m_sizeGrid.height() * m_sizeBox.width() * m_sizeBox.height();
}

void QStageData::resetBoxSize(const QSize& size) {
    //TODO move numbers to the new place
    m_sizeBox = size;
    m_iCellCount = m_sizeGrid.width() * m_sizeGrid.height() * m_sizeBox.width() * m_sizeBox.height();
}

