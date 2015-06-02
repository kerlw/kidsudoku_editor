#ifndef QSTAGEDATA_H
#define QSTAGEDATA_H

#include "portable.h"
#include <QObject>
#include <QSize>
#include <set>

class QStageData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isSpecialRes READ isSpecialRes WRITE setIsSpecialRes DESIGNABLE true USER true) // NOTIFY onIsSpecialResChanged)
    Q_PROPERTY(QString plistFile READ plistFile WRITE setPListFile DESIGNABLE true USER true) // NOTIFY onPListFileChanged)
    Q_PROPERTY(QSize GridSize READ gridSize WRITE setGridSize DESIGNABLE true USER true)
    Q_PROPERTY(QSize BoxSize READ boxSize WRITE setBoxSize DESIGNABLE true USER true)
    Q_CLASSINFO("QStageData", "Stage Data")

private:
    QStageData();

public:
    ~QStageData();

    static QStageData* create(QSize gridSize, QSize boxSize);

    bool isSpecialRes() {        return m_isSpecialRes;    }
    void setIsSpecialRes(bool value) { m_isSpecialRes = value; }

    QString plistFile() { return m_strPListFile;   }
    void setPListFile(QString value) { m_strPListFile = value;  }

    QSize gridSize() {  return m_sizeGrid;  }
    void setGridSize(QSize& size) { m_sizeGrid = size;  }

    QSize boxSize() { return m_sizeBox; }
    void setBoxSize(QSize& size) { m_sizeBox = size;    }

    QString toQString();

    int numberAt(int index);

    void resetRowsPerGrid(int value);
    void resetColsPerGrid(int value);
    void resetGridsInRow(int value);
    void resetGridsInCol(int value);

    void resetGridSize(const QSize& size);
    void resetBoxSize(const QSize& size);

    void toBytes(byte** buff, int& len);
    void parseFromBytes(byte* buff, int len);
private:
    bool m_isSpecialRes;
    QString m_strPListFile;
    QSize m_sizeGrid;
    QSize m_sizeBox;

    int m_iCellCount;

    int* m_pNumbers;
    std::set<int> m_setKnownCells;
};

#endif // QSTAGEDATA_H
