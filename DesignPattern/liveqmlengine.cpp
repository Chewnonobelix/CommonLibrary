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

void LiveQmlEngine::createWindow(QUrl path, QQmlContext* context)
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

    if (m_windows.contains(QUrl(source))) {
        properties["visible"] = m_windows[QUrl(source)]->property("visible");
        delete m_windows[QUrl(source)];
    }

    auto comp = new QQmlComponent(&m_engine, QUrl(source));

    m_context[QUrl(source)] = context;
    QObject* item;
    if(properties.isEmpty())
        item = comp->create(context);
    else
        item = comp->createWithInitialProperties(properties, context);

    onObjectCreated(item, QUrl(source)/*, context*/);
    qCDebug(liveengineerror)<<path<<comp->errorString();
}

void LiveQmlEngine::onObjectCreated(QObject *window, QUrl url/*, QQmlContext* context*/)
{
    m_windows[url] = window;
    connect(window, &QObject::destroyed, this, &LiveQmlEngine::onDestroyed);
    emit sObjectCreated(url, window);
}

void LiveQmlEngine::onDestroyed(QObject *window)
{
    auto url = m_windows.key(window);
    m_windows.remove(url);

    emit sObjectDestroyed(url);
}

void LiveQmlEngine::onFileChanged(QString path)
{
    m_engine.clearComponentCache();
    for (auto it : m_windows.keys()) {
        auto source = it.toString();
        source.remove("file:///");
        for(auto it2: qmlSourceDir())
            if(source.startsWith(it2))
                source.remove(0, it2.size());

        createWindow(QUrl(source), m_context[it]);
    }

    if (QFile::exists(path)) {
        m_watcher.addPath(path);
    }
}

QQmlApplicationEngine &LiveQmlEngine::qmlEngine()
{
    return m_engine;
}
