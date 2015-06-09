#include "QSudokuBoxModel.h"
#include "QStageData.h"

QSudokuBoxModel::QSudokuBoxModel(QObject* parent)
    : QStandardItemModel(parent) {

}

QSudokuBoxModel::~QSudokuBoxModel()
{

}

QVariant QSudokuBoxModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
            return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        int value = QStandardItemModel::data(index, role).value<int>();
        return (value & 0x7fff);
    }
    case Qt::BackgroundColorRole:
    {
        int value = QStandardItemModel::data(index, Qt::EditRole).value<int>();
        return (value & QStageData::SELECTED_MASK) != 0 ?
                    QColor(255, 255, 0) : QColor(255,255,255);
    }
    default:
        return QStandardItemModel::data(index, role);
    }
}
