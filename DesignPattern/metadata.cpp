#include "metadata.h"

bool MetaData::hasMetadata(QString name) const
{
    return m_metadata.contains(name);
}

int MetaData::metaDataCount() const
{
    return m_metadata.count();
}

QStringList MetaData::metadataList() const
{
    return m_metadata.keys();
}

bool MetaData::removeMetadata(QString key)
{
    if (m_metadata.contains(key)) {
        m_metadata.remove(key);
        return true;
    }

    return false;
}

MetaData::MetaData(const QJsonObject &obj)
{
    auto map = obj.toVariantMap();

    for (auto it : map.keys())
        setMetadata(it, map[it]);

    for(auto it = obj.begin(); it!= obj.end(); it++) {
        if(it->isArray()) {
            QStringList sl;
            for(auto it2: it->toArray()) {
                if(it2.isString()) {
                    sl<<it2.toString();
                }
            }

            setMetadata(it.key(), sl);
        }
        else if(it->isString()) {
            setMetadata(it.key(), it->toString());
        }
    }
}

MetaData::operator QJsonObject() const
{
    QJsonObject obj;
    for (auto it : metadataList())
        obj.insert(it, metaData<QString>(it));

    return obj;
}
