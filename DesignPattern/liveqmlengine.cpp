#include "liveqmlengine.h"

//using namespace DesignPattern;

Q_LOGGING_CATEGORY(liveengineerror, "liveengine.componentError")
Q_LOGGING_CATEGORY(liveenginelog, "liveengine.log")

QString LiveQmlEngine::qmlSourceDir() const
{
    return m_qmlSourceDir;
}

void LiveQmlEngine::setQmlSourceDir(QString qmlSourceDir)
{
    m_qmlSourceDir = qmlSourceDir;
}

LiveQmlEngine::LiveQmlEngine(QObject *parent, QString sourceDir)
    : QObject(parent), m_qmlSourceDir(sourceDir)
{
    auto *context = qmlEngine().rootContext();
    context->setContextProperty("_liveQmlEngine", this);
#ifdef ENABLE_HOTRELOADING

    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &LiveQmlEngine::onFileChanged);
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &LiveQmlEngine::onFileChanged);

    QDir dir(qmlSourceDir());
    QList<QFileInfo> list = dir.entryInfoList(QDir::AllDirs | QDir::NoDotDot);

    while (!list.isEmpty()) {
        dir.cd(list.first().absoluteFilePath());
        qCDebug(liveenginelog) << dir.absolutePath();
        auto infoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for (auto it : infoList) {
            if (it.isDir()) {
                list << it;
            } else if (it.suffix() == "qml") {
                m_watcher.addPath(it.absoluteFilePath());
            }
        }
        list.pop_front();
    }
    qCDebug(liveenginelog) << m_watcher.files();
#endif
}

void LiveQmlEngine::createWindow(QUrl path, QQmlContext* context)
{
    QVariantMap properties;

    QString source = path.toString();
#ifdef ENABLE_HOTRELOADING //Setup qml source dir
    source.prepend(qmlSourceDir());
    source.prepend("file:/");
#else
    source.prepend("qrc:");
#endif
    if (m_windows.contains(QUrl(source))) {
        properties["visible"] = m_windows[QUrl(source)]->property("visible");
        delete m_windows[QUrl(source)];
    }

    auto comp = new QQmlComponent(&m_engine, QUrl(source));

    QObject* item;
    if(properties.isEmpty())
        item = comp->create(context);
    else
        item = comp->createWithInitialProperties(properties, context);

    onObjectCreated(item, QUrl(source), context);
    qCDebug(liveengineerror)<<path<<comp->errorString();
}

void LiveQmlEngine::onObjectCreated(QObject *window, QUrl url, QQmlContext* context)
{
    m_windows[url] = window;
    m_context[url] = context;
    connect(window, &QObject::destroyed, this, &LiveQmlEngine::onDestroyed);
    if(!window)
        m_engine.loadData("import QtQuick 2.15; import QtQuick.Controls 2.15;  ApplicationWindow { width: 600; height: 400; Text {anchors.fill: parent; text: \"Error in page\"}}", url);
}

void LiveQmlEngine::onDestroyed(QObject *window)
{
    m_windows.remove(m_windows.key(window));
}

void LiveQmlEngine::onFileChanged(QString path)
{
    m_engine.clearComponentCache();
    for (auto it : m_windows.keys()) {
        createWindow(QUrl(it.toString().remove("file:///" + qmlSourceDir())), m_context[it]);
    }

    if (QFile::exists(path)) {
        m_watcher.addPath(path);
    }
}

QQmlApplicationEngine &LiveQmlEngine::qmlEngine()
{
    return m_engine;
}
