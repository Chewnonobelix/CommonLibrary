#include "liveqmlengine.h"

//using namespace DesignPattern;

Q_LOGGING_CATEGORY(liveengineerror, "liveengine.componentError")
Q_LOGGING_CATEGORY(liveenginelog, "liveengine.log")

QStringList LiveQmlEngine::qmlSourceDir() const
{
    return m_qmlSourceDir;
}

void LiveQmlEngine::setQmlSourceDir(QStringList qmlSourceDir)
{
    m_qmlSourceDir = qmlSourceDir;
}

LiveQmlEngine::LiveQmlEngine(QObject *parent, QStringList sourceDir)
    : QObject(parent), m_qmlSourceDir(sourceDir)
{
    auto *context = qmlEngine().rootContext();
    context->setContextProperty("_liveQmlEngine", this);
#ifdef ENABLE_HOTRELOADING

    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &LiveQmlEngine::onFileChanged);
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &LiveQmlEngine::onFileChanged);

    for(auto it: qmlSourceDir()) {
        QDir dir(it);
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
    }
    qCDebug(liveenginelog) << m_watcher.files();
#endif
}

void LiveQmlEngine::createWindow(QUrl path, QQmlContext* context, QUuid id)
{
    QVariantMap properties;

    QString source = path.toString();
#ifdef ENABLE_HOTRELOADING //Setup qml source dir
    for(auto it: qmlSourceDir())
        if(QFile::exists(it + source))
            source.prepend(it);
    source.prepend("file:///");
#else
    source.prepend("qrc:");
#endif

    if (m_windows.contains(id)) {
        properties["visible"] = m_windows[id]->property("visible");
        delete m_windows[id];
    }

    auto comp = new QQmlComponent(&m_engine, QUrl(source));
    if(id.isNull()) {
        id = QUuid::createUuid();
        m_ids.insert(path, id);
    }

    m_context[id] = context;
    QObject* item;
    if(properties.isEmpty())
        item = comp->create(context);
    else
        item = comp->createWithInitialProperties(properties, context);

    onObjectCreated(item, QUrl(source), id);
    qCDebug(liveengineerror)<<path<<comp->errorString();
}

void LiveQmlEngine::onObjectCreated(QObject *window, QUrl url, QUuid id)
{
    m_windows[id] = window;
    connect(window, &QObject::destroyed, this, &LiveQmlEngine::onDestroyed);
    emit sObjectCreated(url, window);
}

void LiveQmlEngine::onDestroyed(QObject *window)
{
    auto id = m_windows.key(window);
    m_windows.remove(id);
    m_context.remove(id);
    auto url = m_ids.key(id);
    m_ids.remove(url, id);
    emit sObjectDestroyed(url);
}

void LiveQmlEngine::onFileChanged(QString path)
{
    m_engine.clearComponentCache();

    for (auto it : m_ids.keys()) {
        auto source = it.toString();
        source.remove("file:///");
        for(auto it2: qmlSourceDir())
            if(source.startsWith(it2))
                source.remove(0, it2.size());

        for(auto it3: m_ids.values(it)) {
            createWindow(QUrl(source), m_context[it3], it3);
        }
    }

    if (QFile::exists(path)) {
        m_watcher.addPath(path);
    }
}

QQmlApplicationEngine &LiveQmlEngine::qmlEngine()
{
    return m_engine;
}

void LiveQmlEngine::removeWindow(QObject* windows)
{
    auto id = m_windows.key(windows);
    auto url = m_ids.key(id);
    m_ids.remove(url, id);
    m_context.remove(id);
    m_windows.remove(id);
    windows->deleteLater();
}
