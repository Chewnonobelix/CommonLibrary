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
    void addMetadata();
    void addMetadata_data();
    void hasMetadata();
    void hasMetadata_data();
};

MetaDataTest::MetaDataTest() {}
MetaDataTest::~MetaDataTest() {}

void MetaDataTest::initTestCase()
{

}

void MetaDataTest::cleanupTestCase()
{

}

void MetaDataTest::addMetadata()
{
    QFETCH(QStringList, list);

    MetaData md;

    for(auto it: list) {
        md.setMetadata(it, 0);
    }

    auto res = list;
    res.removeDuplicates();
    QCOMPARE(md.metadataList(), res);
}

void MetaDataTest::addMetadata_data()
{
    QTest::addColumn<QStringList>("list");

    QTest::newRow("01")<<QStringList{"data01"};
    QTest::newRow("02")<<QStringList{"data01", "data02"};
    QTest::newRow("03")<<QStringList{"data01", "data02", "data03"};
    QTest::newRow("04")<<QStringList{"data01", "data02", "data03", "data04"};
    QTest::newRow("05")<<QStringList{"data01", "data02", "data03", "data04", "data05"};
    QTest::newRow("06")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06"};
    QTest::newRow("07")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07"};
    QTest::newRow("08")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08"};
    QTest::newRow("09")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09"};
    QTest::newRow("10")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09", "data10"};
    QTest::newRow("11")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09", "data10", "data11"};
    QTest::newRow("Duplicate")<<QStringList{"data1", "data2", "data1"};
}

void MetaDataTest::hasMetadata()
{
    QFETCH(MetaData, model);
    QFETCH(bool, res);
    QFETCH(QString, field);

    QCOMPARE(model.hasMetadata(field), res);
}

void MetaDataTest::hasMetadata_data()
{
    QTest::addColumn<MetaData>("model");
    QTest::addColumn<QString>("field");
    QTest::addColumn<bool>("res");

    MetaData md;
    md.setMetadata("data01", 0);
    md.setMetadata("data02", 0);
    md.setMetadata("data03", 0);

    QTest::newRow("01")<<md<<"data01"<<true;
    QTest::newRow("02")<<md<<"data02"<<true;
    QTest::newRow("04")<<md<<"data04"<<false;
    QTest::newRow("03")<<md<<"data03"<<true;
    QTest::newRow("016")<<md<<"data016"<<false;
    QTest::newRow("011")<<md<<"data011"<<false;
}


QTEST_APPLESS_MAIN(MetaDataTest)

#include "metadatatest.moc"
