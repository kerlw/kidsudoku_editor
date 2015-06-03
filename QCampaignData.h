#ifndef QCAMPAIGNDATA_H
#define QCAMPAIGNDATA_H

#include <QObject>
#include <vector>
#include <QStringList>

#include "portable.h"

class QStageData;

class QCampaignData : public QObject
{
    Q_OBJECT

public:
    explicit QCampaignData(QObject *parent = 0);
    ~QCampaignData();

    int getStageDataCount() {   return m_vctData.size();    }

    void addStageData(QStageData* pData, int index = -1);
    void removeStageDataAt(int index);

    QStageData* getStageAt(int index);

    void toBytes(byte** buff, int& len);
    void parseFromBytes(const byte* buff, int len);

    QStringList getStagesStringList();

    QString& getSavePath() { return m_strPath; }
    void setSavePath(const QString& path) { m_strPath = path; }
signals:

public slots:

private:
    std::vector<QStageData*> m_vctData;
    QString m_strPath;
};

#endif // QCAMPAIGNDATA_H
