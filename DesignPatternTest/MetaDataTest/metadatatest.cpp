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

void MetaDataTest::valueMetadata()
{
    QFETCH(MetaData, model);
    QFETCH(QString, field);
    QFETCH(double, res);

    QCOMPARE(model.metaData<double>(field), res);
}

void MetaDataTest::valueMetadata_data()
{
    QTest::addColumn<MetaData>("model");
    QTest::addColumn<QString>("field");
    QTest::addColumn<double>("res");

    MetaData model;
    model.setMetadata("data1", 1.2);
    model.setMetadata("data2", 1);
    model.setMetadata("data3", -1.2);
    model.setMetadata("data4", 1.61);

    QTest::addRow("1.2")<<model<<"data1"<<1.2;
    QTest::addRow("1")<<model<<"data2"<<1.;
    QTest::addRow("-1.2")<<model<<"data3"<<-1.2;
    QTest::addRow("1.61")<<model<<"data4"<<1.61;
}

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

void MetaDataTest::compareMetadata()
{
    QFETCH(MetaData, m1);
    QFETCH(MetaData, m2);
    QFETCH(QString, field);
    QFETCH(QPartialOrdering, res);

    QCOMPARE(compare(m1, m2, field), res);
}

void MetaDataTest::compareMetadata_data()
{
    QTest::addColumn<MetaData>("m1");
    QTest::addColumn<MetaData>("m2");
    QTest::addColumn<QString>("field");
    QTest::addColumn<QPartialOrdering>("res");

    MetaData m1, m2;
    m1.setMetadata("data01", 1);
    m1.setMetadata("data02", 3);
    m1.setMetadata("data03", 0);
    m1.setMetadata("data04", 0);
    m2.setMetadata("data01", 1);
    m2.setMetadata("data02", 2);
    m2.setMetadata("data04", 2);
    m2.setMetadata("data05", 2);

    QTest::addRow("01")<<m1<<m2<<"data01"<<QPartialOrdering::Equivalent;
    QTest::addRow("02")<<m1<<m2<<"data02"<<QPartialOrdering::Greater;
    QTest::addRow("03")<<m1<<m2<<"data03"<<QPartialOrdering::Unordered;
    QTest::addRow("04")<<m1<<m2<<"data04"<<QPartialOrdering::Less;
    QTest::addRow("05")<<m1<<m2<<"data05"<<QPartialOrdering::Unordered;
}

void MetaDataTest::toJsonMetadata()
{
    QFETCH(MetaData, model);
    QFETCH(QJsonObject, json);

    qDebug()<<(QJsonObject)model<<json;
    QCOMPARE((QJsonObject)model, json);
}

void MetaDataTest::toJsonMetadata_data()
{
    QTest::addColumn<MetaData>("model");
    QTest::addColumn<QJsonObject>("json");

    MetaData md;
    md.setMetadata("data01", 0);
    md.setMetadata("data02", QString("02"));
    md.setMetadata("data03", QStringList{"02", "03"});

    QJsonObject res;
    res["data01"] = "0";
    res["data02"] = "02";
    res["data03"] = QJsonArray{"02", "03"};

    QTest::addRow("To json")<<md<<res;
}

void MetaDataTest::fromJsonMetadata()
{
    QFETCH(QJsonObject, model);
    QFETCH(MetaData, res);

    MetaData md(model);

    QCOMPARE(md.metadataList(), res.metadataList());

    for(auto it: md.metadataList()) {
        QCOMPARE(md.metaData<QVariant>(it), res.metaData<QVariant>(it));
    }
}

void MetaDataTest::fromJsonMetadata_data()
{
    QTest::addColumn<QJsonObject>("model");
    QTest::addColumn<MetaData>("res");

    MetaData res;
    res.setMetadata("data01", 0);
    res.setMetadata("data02", QString("02"));
    res.setMetadata("data03", QStringList{"02", "03"});

    QJsonObject md;
    md["data01"] = "0";
    md["data02"] = "02";
    md["data03"] = QJsonArray::fromStringList({"02", "03"});

    QTest::addRow("From json")<<md<<res;
}

QTEST_APPLESS_MAIN(MetaDataTest)

#include "metadatatest.moc"
