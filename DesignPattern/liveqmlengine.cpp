#include "liveqmlengine.h"

namespace DesignPattern {

LiveQmlEngine::LiveQmlEngine(QObject* parent)
	: QObject(parent) {
	auto* context = qmlEngine().rootContext();
	context->setContextProperty("_liveQmlEngine", this);
#ifdef ENABLE_HOTRELOADING

    connect(&(m_engine),
            &QQmlApplicationEngine::objectCreated,
            this,
            &LiveQmlEngine::onObjectCreated);

    connect(&m_watcher),
            &QFileSystemWatcher::fileChanged,
            this,
            &LiveQmlEngine::onFileChanged);
    connect(&m_watcher),
            &QFileSystemWatcher::directoryChanged,
            this,
            &LiveQmlEngine::onFileChanged);

    QDir dir(QString(ARTLANTIS_QML_SOURCE));

    auto infoList = dir.entryInfoList(QStringList("*.qml"));

    for (auto it : infoList) {
        m_watcher.addPath(it.absoluteFilePath());
    }
#endif
}

void LiveQmlEngine::createWindow(QUrl path)
{
    QVariantMap properties;
    if (m_windows.contains(path)) {
        properties["visible"] = m_windows[path]->property("visible");
        m_windows[path]->deleteLater();
    }

    QString source = path.toString().split("/").last();
#ifdef ENABLE_HOTRELOADING //Setup qml source dir
    source.prepend(QML_SOURCE);
    source.prepend("file:/");
#else
    source.prepend("qrc:/");
#endif

    m_engine.setInitialProperties(properties);
    m_engine.load(source);
}

void LiveQmlEngine::onObjectCreated(QObject *window, QUrl url)
{
    m_windows[url] = window;
    connect(window, &QObject::destroyed, this, &LiveQmlEngine::onDestroyed);
}

void LiveQmlEngine::onDestroyed(QObject *window)
{
    m_windows.remove(m_windows.key(window));
}

void LiveQmlEngine::onFileChanged(QString path)
{
    m_engine.clearComponentCache();
    for (auto const it : m_windows.keys())
        createWindow(it);

    if (QFile::exists(path)) {
        m_watcher.addPath(path);
    }
}

QQmlApplicationEngine &LiveQmlEngine::qmlEngine()
{
    return m_engine;
}

} // namespace DesignPattern
