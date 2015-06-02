#include "QCampaignData.h"
#include "QStageData.h"

QCampaignData::QCampaignData(QObject *parent) : QObject(parent)
{

}

QCampaignData::~QCampaignData()
{
    std::vector<QStageData*>::iterator it = m_vctData.begin();
    while (it != m_vctData.end()) {
        delete *it;
        ++it;
    }
    m_vctData.clear();
}

void QCampaignData::addStageData(QStageData *pData, int index) {
    //default index or index is out of range, then push_back the new element
    if (index < 0 ||  index >= m_vctData.size()) {
        m_vctData.push_back(pData);
    } else {
        std::vector<QStageData*>::iterator it;
        it = m_vctData.begin() + index;
        m_vctData.insert(it, 1,  pData);
    }
}

void QCampaignData::removeStageDataAt(int index) {
    if (index < 0 || index >= m_vctData.size())
        return;

    std::vector<QStageData*>::iterator it;
    it = m_vctData.begin() + index;

    delete *it;
    m_vctData.erase(it);
}

QStageData* QCampaignData::getStageAt(int index) {
    if (index < 0 || index >= m_vctData.size())
        return NULL;

    return m_vctData[index];
}
