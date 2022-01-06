#include <QSignalSpy>
#include <QtTest>

#include <metadata.h>

class MetaDataTest : public QObject
{
    Q_OBJECT

public:
    MetaDataTest();
    ~MetaDataTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
};

MetaDataTest::MetaDataTest() {}
MetaDataTest::~MetaDataTest() {}

void MetaDataTest::initTestCase()
{

}

void MetaDataTest::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(MetaDataTest)

#include "metadatatest.moc"
