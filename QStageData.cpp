#include "QStageData.h"
#include <QDataStream>

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

int QStageData::lengthInByte() const {
    int ret = 4;    //id
    ret += 4;       //TODO speicified resources
    ret += 4;       //grid size & box size
    ret += 2;       //count of known cells
    ret += 2 * m_setKnownCells.size();  //known cells position&number array
}

QDataStream& operator<<(QDataStream& stream, const QStageData& data) {
    stream << quint32(data.lengthInByte());
    stream << 1;    //TODO id
    stream << 0;    //TODO specified resource
    stream << quint8(data.m_sizeGrid.height());
    stream << quint8(data.m_sizeGrid.width());
    stream << quint8(data.m_sizeBox.height());
    stream << quint8(data.m_sizeBox.width());

    stream << quint16(data.m_setKnownCells.size());

    std::set<int>::const_iterator it = data.m_setKnownCells.begin();
    while (it != data.m_setKnownCells.end()) {
        stream << quint8(*it) << quint8(data.m_pNumbers[*it]);
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
    int count = i16;

    memset(data.m_pNumbers, 0, sizeof(int)*MAX_SIZE);
    for (int i = 0; i < count; i++) {
        stream >> i8;
        stream >> reinterpret_cast<quint8&>(data.m_pNumbers[i8]);
    }

    return stream;
}

