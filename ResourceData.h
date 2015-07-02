#ifndef RESOURCEDATA_H
#define RESOURCEDATA_H

#include <QString>
#include <vector>

class ResourceData {
public:
    QString name;
    bool isInternal;
    bool isDefault;

    QString toString();
};

class ResourceDataManager {
public:
    static ResourceDataManager* getInstance();
    virtual ~ResourceDataManager();

    int size() { return m_vctRes.size();    }
    ResourceData* operator[](int& index) {
        return m_vctRes[index];
    }

private:
    ResourceDataManager();

private:
    std::vector<ResourceData*> m_vctRes;
};

#endif // RESOURCEDATA_H
