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
    void metadataCount();
    void metadataCount_data();
    void valueMetadata();
    void valueMetadata_data();
    void removeMetadata();
    void removeMetadata_data();
    void compareMetadata();
    void compareMetadata_data();
    void toJsonMetadata();
    void toJsonMetadata_data();
    void fromJsonMetadata();
    void fromJsonMetadata_data();
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

void MetaDataTest::metadataCount()
{
    QFETCH(QStringList, list);
    QFETCH(int, count);

    MetaData md;

    for(auto it: list) {
        md.setMetadata(it, 0);
    }

    QCOMPARE(md.metaDataCount(), count);
}

void MetaDataTest::metadataCount_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("count");

    QTest::newRow("01")<<QStringList{"data01"}<<1;
    QTest::newRow("02")<<QStringList{"data01", "data02"}<<2;
    QTest::newRow("03")<<QStringList{"data01", "data02", "data03"}<<3;
    QTest::newRow("04")<<QStringList{"data01", "data02", "data03", "data04"}<<4;
    QTest::newRow("05")<<QStringList{"data01", "data02", "data03", "data04", "data05"}<<5;
    QTest::newRow("06")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06"}<<6;
    QTest::newRow("07")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07"}<<7;
    QTest::newRow("08")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08"}<<8;
    QTest::newRow("09")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09"}<<9;
    QTest::newRow("10")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09", "data10"}<<10;
    QTest::newRow("11")<<QStringList{"data01", "data02", "data03", "data04", "data05", "data06", "data07", "data08", "data09", "data10", "data11"}<<11;
    QTest::newRow("Duplicate")<<QStringList{"data1", "data2", "data1"}<<2;
}

void MetaDataTest::valueMetadata() {}

void MetaDataTest::valueMetadata_data() {}

void MetaDataTest::removeMetadata()
{
    QFETCH(QSharedPointer<MetaData>, model);
    QFETCH(QString, field);
    QFETCH(QStringList, list);
    QFETCH(bool, res);

    QCOMPARE(model->removeMetadata(field), res);
    QCOMPARE(model->metadataList(), list);
}

void MetaDataTest::removeMetadata_data()
{
    QTest::addColumn<QSharedPointer<MetaData>>("model");
    QTest::addColumn<QString>("field");
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<bool>("res");

    auto md = QSharedPointer<MetaData>::create();
    md->setMetadata("data01", 0);
    md->setMetadata("data02", 0);
    md->setMetadata("data03", 0);

    QTest::addRow("!04")<<md<<"data04"<<QStringList{"data01", "data02", "data03"}<<false;
    QTest::addRow("02")<<md<<"data02"<<QStringList{"data01", "data03"}<<true;
    QTest::addRow("!02")<<md<<"data02"<<QStringList{"data01", "data03"}<<false;
}

void MetaDataTest::compareMetadata() {}

void MetaDataTest::compareMetadata_data() {}

void MetaDataTest::toJsonMetadata() {}

void MetaDataTest::toJsonMetadata_data() {}

void MetaDataTest::fromJsonMetadata() {}

void MetaDataTest::fromJsonMetadata_data() {}

QTEST_APPLESS_MAIN(MetaDataTest)

#include "metadatatest.moc"
