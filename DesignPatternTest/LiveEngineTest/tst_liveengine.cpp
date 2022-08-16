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

    QSignalSpy spy(engine.data(), SIGNAL(sObjectCreated(QUuid, QUrl, QObject*)));

    engine->createWindow(QUrl(baseName));

    QCOMPARE(spy.count(), 1);
    auto first = spy.takeFirst();

    QCOMPARE(first.at(1).toUrl(), fullPath);
    QCOMPARE(first.at(2).value<QObject*>() != nullptr, result);
}

void TestLiveEngine::windowCreation_data()
{
    QTest::addColumn<QSharedPointer<LiveQmlEngine>>("engine");
    QTest::addColumn<QString>("baseName");
    QTest::addColumn<QUrl>("fullPath");
    QTest::addColumn<bool>("result");

    auto engine = QSharedPointer<LiveQmlEngine>::create(nullptr, QString(FOLDER).split("|"));

    QTest::addRow("create ok")<<engine<<"TestViewOk.qml"<<QUrl("file:///"+QString(FOLDER).split("|").at(0)+"TestViewOk.qml")<<true;
    QTest::addRow("create fail")<<engine<<"TestViewBad.qml"<<QUrl("file:///"+QString(FOLDER).split("|").at(1)+"TestViewBad.qml")<<false;
}

void TestLiveEngine::fileChanged()
{
    QFETCH(QSharedPointer<LiveQmlEngine>, engine);
    QFETCH(QString, baseName);
    QFETCH(QString, fullPath);
    QFETCH(QString, newColor);
    QFETCH(QString, oldColor);

    QSignalSpy spy(engine.data(), SIGNAL(sObjectCreated(QUuid, QUrl, QObject*)));

    engine->createWindow(QUrl(baseName));

    QCOMPARE(spy.count(), 1);
    {
        QFile file(fullPath);
        file.open(QIODevice::ReadOnly);
        auto data = QString(file.readAll());
        data.replace(oldColor, newColor);
        file.close();

        QFile file2(fullPath);
        file2.open(QIODevice::WriteOnly);
        file2.write(data.toLatin1());
        file2.close();
    }
    QTest::qWait(5000);
    QCOMPARE(spy.count(), 2);
    {
        QFile file(fullPath);
        file.open(QIODevice::ReadOnly);
        auto data = QString(file.readAll());
        data.replace(newColor, oldColor);
        file.close();

        QFile file2(fullPath);
        file2.open(QIODevice::WriteOnly);
        file2.write(data.toLatin1());
        file2.close();
    }

}

void TestLiveEngine::fileChanged_data()
{
    QTest::addColumn<QSharedPointer<LiveQmlEngine>>("engine");
    QTest::addColumn<QString>("baseName");
    QTest::addColumn<QString>("fullPath");
    QTest::addColumn<QString>("newColor");
    QTest::addColumn<QString>("oldColor");

    auto engine = QSharedPointer<LiveQmlEngine>::create(nullptr, QString(FOLDER).split("|"));

    QTest::addRow("create ok")<<engine<<"TestViewOk.qml"<<QString(FOLDER).split("|").at(0)+"TestViewOk.qml"<<QString("green")<<QString("red");
}

void TestLiveEngine::objectDestroyed()
{
    QFETCH(QSharedPointer<LiveQmlEngine>, engine);
    QFETCH(QString, baseName);

    QSignalSpy spy(engine.data(), SIGNAL(sObjectCreated(QUuid, QUrl, QObject*)));

    engine->createWindow(QUrl(baseName));

    QCOMPARE(spy.count(), 1);
    auto first = spy.takeFirst();

    auto pointer = first.at(2).value<QObject*>();
    QSignalSpy destroyer(engine.data(), SIGNAL(sObjectDestroyed(QUuid)));
    delete pointer;

    QCOMPARE(destroyer.count(), 1);
}

void TestLiveEngine::objectDestroyed_data()
{
    QTest::addColumn<QSharedPointer<LiveQmlEngine>>("engine");
    QTest::addColumn<QString>("baseName");

    auto engine = QSharedPointer<LiveQmlEngine>::create(nullptr, QString(FOLDER).split("|"));

    QTest::addRow("Destroyed")<<engine<<"TestViewOk.qml";
}

QTEST_MAIN(TestLiveEngine)

#include "tst_liveengine.moc"
