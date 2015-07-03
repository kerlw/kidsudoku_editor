#ifndef QCAMPAIGNDATA_H
#define QCAMPAIGNDATA_H

#include <QObject>
#include <vector>
#include <QStringList>
#include <QDataStream>
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

    QStringList getStagesStringList();

    QString& getSavePath() { return m_strPath; }
    void setSavePath(const QString& path) { m_strPath = path; }

    int lengthInByte() const;

    void swap(int src, int dst);

    void setResource(QString& res);
    QString getResource();
signals:

public slots:

private:
    std::vector<QStageData*> m_vctData;
    QString m_strPath;
    QString m_strRes;

    friend QDataStream& operator<<(QDataStream& stream, const QCampaignData& data);
};

QDataStream& operator<<(QDataStream& stream, const QCampaignData& data);
QDataStream& operator>>(QDataStream& stream, QCampaignData& data);

#endif // QCAMPAIGNDATA_H
