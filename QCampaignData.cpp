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

void QCampaignData::toBytes(byte** buff, int& len) {
    QDataStream stream;
    len = 0;
    stream << quint16(1) << quint16(1);  //TODO version
    len += 4;
    stream << 0 << 0 << 0 << 0; //TODO id
    len += 16;
    stream << 0; //TODO specified resource
    len += 4;
    stream << qint32(m_vctData.size());
    len += 4;

    int start_offset = len + sizeof(int) * m_vctData.size();
    std::vector<int> offsets;
    QDataStream stream_data;
    std::vector<QStageData*>::iterator it = m_vctData.begin();
    byte* tmp;
    int length = 0;
    while (it != m_vctData.end()) {
        start_offset += length;
        offsets.push_back(start_offset);

        (*it)->toBytes(&tmp, length);
        stream_data << QByteArray((const char*)tmp, length);
        ++it;
        len += length;
    }

    std::vector<int>::iterator it2 = offsets.begin();
    while (it2 != offsets.end()) {
        stream << *it2;
        ++it2;
        len += 4;
    }

    *buff = new byte[len];
    int read = stream.readRawData((char*)buff, len);
    stream_data.readRawData((char*)buff+read, len - read);
}

void QCampaignData::parseFromBytes(const byte* buff, int len) {
    QDataStream stream(QByteArray((const char *)buff, len));
    stream.skipRawData(4 + 16); //TODO skipped version and id
    int tmp;
    stream >> reinterpret_cast<quint32&>(tmp);
    if (tmp > 0) {
        stream.skipRawData(tmp);    //TODO sepecified resource
    }

    stream >> tmp;
    int count = tmp;
    stream.skipRawData(count*sizeof(int));
    for (int index = 0; index < tmp; index++) {
        char* buffer;
        stream >> reinterpret_cast<quint32&>(tmp);
        stream.readBytes(buffer, reinterpret_cast<uint&>(tmp));

        QStageData* data = QStageData::create(QSize(1,1), QSize(1,1));
        data->parseFromBytes((byte*)buffer, tmp);

        this->addStageData(data);
    }
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
