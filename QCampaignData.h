#ifndef QCAMPAIGNDATA_H
#define QCAMPAIGNDATA_H

#include <QObject>
#include <vector>

class QStageData;

class QCampaignData : public QObject
{
    Q_OBJECT


public:
    explicit QCampaignData(QObject *parent = 0);
    ~QCampaignData();

    void addStageData(QStageData* pData, int index = -1);
    void removeStageDataAt(int index);

    QStageData* getStageAt(int index);
signals:

public slots:

private:
    std::vector<QStageData*> m_vctData;
};

#endif // QCAMPAIGNDATA_H
