#ifndef METADATA_H
#define METADATA_H

#include <QMap>
#include <QVariant>

class MetaData
{
private:
    QMap<QString, QVariant> m_metadata;

public:
    MetaData() = default;
    MetaData(const MetaData&) = default;
    ~MetaData() = default;
    MetaData& operator =(const MetaData&) = default;

    bool hasMetadata(QString name) const;
    int metaDataCount() const;
    QStringList metadataList() const;

    template<class T>
    T metaData(QString name) const
    {
        if(hasMetadata(name))
            return m_metadata[name].value<T>();
        else
            return T();
    }

    template<class T>
    void setMetadata(QString name, T value)
    {
        m_metadata[name] = QVariant::fromValue(value);
    }
};

#endif // METADATA_H
