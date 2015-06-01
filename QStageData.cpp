#include "QStageData.h"

QStageData::QStageData()
{

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

    return ret;
}
