#include "QSudokuBoxModel.h"
#include "QStageData.h"

static const QColor SELECTED_COLOR[2] = { QColor(255, 255, 0), QColor(0xac, 0xac, 0) };
static const QColor UNSELECTED_COLOR[2] = { QColor(255, 255, 255), QColor(0xac, 0xac, 0xac) };

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
        int offset = ((index.row() / m_sizeGrid.height() % 2) + index.column() / m_sizeGrid.width()) % 2;
        return (value & QStageData::SELECTED_MASK) != 0 ?
                    SELECTED_COLOR[offset] : UNSELECTED_COLOR[offset];
    }
    default:
        return QStandardItemModel::data(index, role);
    }
}

void QSudokuBoxModel::setSize(const QSize& grid, const QSize& box) {
    m_sizeGrid = grid;
    m_sizeBox = box;

    int rows = grid.height() * box.height();
    int cols = grid.width() * box.width();
    this->setRowCount(rows);
    this->setColumnCount(cols);

}
