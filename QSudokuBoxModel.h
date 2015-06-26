#ifndef __QSUDOKUBOXMODEL_H__
#define __QSUDOKUBOXMODEL_H__

#include <QStandardItemModel>

class QSudokuBoxModel : public QStandardItemModel
{
    Q_OBJECT
public:
    QSudokuBoxModel(QObject *parent = 0);
    virtual ~QSudokuBoxModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setSize(const QSize& grid, const QSize& box);

protected:
    QSize m_sizeGrid;
    QSize m_sizeBox;
};

#endif // __QSUDOKUBOXMODEL_H__
