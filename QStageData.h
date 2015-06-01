#ifndef QSTAGEDATA_H
#define QSTAGEDATA_H

#include <QObject>
#include <QSize>

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

private:
    bool m_isSpecialRes;
    QString m_strPListFile;\
    QSize m_sizeGrid;
    QSize m_sizeBox;
};

#endif // QSTAGEDATA_H
