#include <QTest>
#include <QSignalSpy>

#include <liveqmlengine.h>

class TestLiveEngine: public QObject
{
    Q_OBJECT
private:

public:
    TestLiveEngine() = default;
    ~TestLiveEngine() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void windowCreation();
    void windowCreation_data();
    void fileChanged();
    void fileChanged_data();
    void objectDestroyed();
    void objectDestroyed_data();
};

void TestLiveEngine::initTestCase()
{

}

void TestLiveEngine::cleanupTestCase()
{

}

void TestLiveEngine::windowCreation()
{
    QFETCH(QSharedPointer<LiveQmlEngine>, engine);
    QFETCH(QString, baseName);
    QFETCH(QUrl, fullPath);
    QFETCH(bool, result);

    QSignalSpy spy(engine.data(), SIGNAL(sObjectCreated(QUrl,QObject*)));

    engine->createWindow(QUrl(baseName));

    QCOMPARE(spy.count(), 1);
    auto first = spy.takeFirst();
    qDebug()<<first.at(0).toUrl()<< fullPath;
    QCOMPARE(first.at(0).toUrl(), fullPath);
    QCOMPARE(first.at(1).value<QObject*>() != nullptr, result);
}

void TestLiveEngine::windowCreation_data()
{
    QTest::addColumn<QSharedPointer<LiveQmlEngine>>("engine");
    QTest::addColumn<QString>("baseName");
    QTest::addColumn<QUrl>("fullPath");
    QTest::addColumn<bool>("result");

    auto engine = QSharedPointer<LiveQmlEngine>::create(nullptr, QString(FOLDER));

    QTest::addRow("create ok")<<engine<<"/TestViewOk.qml"<<QUrl(QString(FOLDER)+"/TestViewOk.qml")<<true;
    QTest::addRow("create fail")<<engine<<"/TestViewBad.qml"<<QUrl(QString(FOLDER)+"/TestViewBad.qml")<<false;
}

void TestLiveEngine::fileChanged() {}
void TestLiveEngine::fileChanged_data() {}
void TestLiveEngine::objectDestroyed() {}
void TestLiveEngine::objectDestroyed_data() {}

QTEST_MAIN(TestLiveEngine)

#include "tst_liveengine.moc"
