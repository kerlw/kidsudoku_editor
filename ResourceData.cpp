#include "ResourceData.h"

static ResourceDataManager* g_pResManager = 0;
static const int INTERNAL_RES_COUNT = 3;
static const QString INTERNAL_RES[INTERNAL_RES_COUNT] = {"numbers", "animals", "fruits" };

QString ResourceData::toString() {
    QString ret = isInternal ? "[ Internal ] " : "";
    ret.append(name);
    if (isDefault)
        ret.append(" (Default)");

    return ret;
}

ResourceDataManager* ResourceDataManager::getInstance() {
    if (!g_pResManager) {
        g_pResManager = new ResourceDataManager();
    }

    return g_pResManager;
}

ResourceDataManager::ResourceDataManager() {
    for (int i = 0; i < INTERNAL_RES_COUNT; i++) {
        ResourceData* data = new ResourceData();
        data->name = INTERNAL_RES[i];
        data->isInternal = true;
        data->isDefault = (i == 0);

        m_vctRes.push_back(data);
    }
}

ResourceDataManager::~ResourceDataManager() {
    std::vector<ResourceData*>::iterator it = m_vctRes.begin();
    while (it != m_vctRes.end()) {
        delete *it;
        it++;
    }
    m_vctRes.clear();
}
