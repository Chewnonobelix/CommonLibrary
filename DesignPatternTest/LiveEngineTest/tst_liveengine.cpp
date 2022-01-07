#include <QTest>

class TestLiveEngine: public QObject
{
    Q_OBJECT

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

void TestLiveEngine::windowCreation() {}
void TestLiveEngine::windowCreation_data() {}
void TestLiveEngine::fileChanged() {}
void TestLiveEngine::fileChanged_data() {}
void TestLiveEngine::objectDestroyed() {}
void TestLiveEngine::objectDestroyed_data() {}

QTEST_APPLESS_MAIN(TestLiveEngine)

#include "tst_liveengine.moc"
