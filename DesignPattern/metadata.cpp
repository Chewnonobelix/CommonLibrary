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
