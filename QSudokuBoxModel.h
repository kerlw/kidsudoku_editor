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
};

#endif // __QSUDOKUBOXMODEL_H__
