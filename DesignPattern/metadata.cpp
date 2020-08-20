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

void MetaData::removeMetadata(QString key)
{
    if (m_metadata.contains(key))
        m_metadata.remove(key);
}
