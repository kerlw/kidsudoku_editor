#include <Qt>
#include <QDataStream>

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
    if (!pData)
        return;

    //associate the stage data to the campaign data, make it auto released at last.
    pData->setParent(this);

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

QStringList QCampaignData::getStagesStringList() {
    QStringList sl;
    std::vector<QStageData*>::iterator it = m_vctData.begin();
    while (it != m_vctData.end()) {
        sl << (*it)->toQString();
        ++it;
    }
    return sl;
}

int QCampaignData::lengthInByte() const {
    int len = 4;    //version
    len += 16;      //uuid
    len += 4 + 0;   //TODO speicified resource
    len += 4 + 4 * m_vctData.size(); //stage data count and offset array.
    std::vector<QStageData*>::const_iterator it = m_vctData.begin();
    while (it != m_vctData.end()) {
        len += 4 + (*it)->lengthInByte();
        ++it;
    }
    return 0;
}

QDataStream& operator<<(QDataStream& stream, const QCampaignData& data) {
    int len = 0;        // used to calculate the start offset of stage data.

    stream << data.lengthInByte();
    len += 4;
    stream << quint16(1) << quint16(1);  //TODO version
    len += 4;
    stream << 0 << 0 << 0 << 0; //TODO id
    len += 16;
    stream << 0; //TODO specified resource
    len += 4;
    stream << qint32(data.m_vctData.size());
    len += 4;

    //calculate the start offset for the first stage datga
    int start_offset = len + sizeof(int) * data.m_vctData.size();

    //write stage data start offset arrays into stream
    std::vector<QStageData*>::const_iterator it = data.m_vctData.begin();
    while (it != data.m_vctData.end()) {
        stream << quint32(start_offset);
        start_offset += sizeof(quint32) + (*it)->lengthInByte();
        ++it;
    }

    it = data.m_vctData.begin();
    while (it != data.m_vctData.end()) {
        stream << *(*it);
        ++it;
    }

    return stream;
}

QDataStream& operator>>(QDataStream& stream, QCampaignData& data) {
    int tmp;

    stream >> tmp;  //skip length of campaign data
    stream.skipRawData(4 + 16); //TODO skipped version and id
    stream >> reinterpret_cast<quint32&>(tmp);
    if (tmp > 0) {
        stream.skipRawData(tmp);    //TODO sepecified resource
    }

    //get count of stage data.
    stream >> tmp;
    int count = tmp;

    stream.skipRawData(count*sizeof(int));  //skip offset arrays

    for (int index = 0; index < count; index++) {
        QStageData* stage = QStageData::create(QSize(1,1), QSize(1,1));
        stream >> tmp;  //skip length of stage data
        stream >> *stage;
        data.addStageData(stage);
    }

    //TODO parse resources
    return stream;
}
