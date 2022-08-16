#pragma once

#include <QDir>
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QThread>
#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QLoggingCategory>
#include <QUuid>
#include <QQuickWindow>

Q_DECLARE_LOGGING_CATEGORY(livreengineerror)
Q_DECLARE_LOGGING_CATEGORY(livreenginelog)

//namespace DesignPattern {
class LiveQmlEngine : public QObject
{
    Q_OBJECT
private:
    QMultiMap<QUrl, QUuid> m_ids;
    QMap<QUuid, QObject *> m_windows;
    QMap<QUuid, QQmlContext*> m_context;

    QQmlApplicationEngine m_engine;
    QFileSystemWatcher m_watcher;
    QStringList m_qmlSourceDir;

public:
    LiveQmlEngine(QObject * = nullptr, QStringList = QStringList());
    ~LiveQmlEngine() = default;
    QQmlApplicationEngine &qmlEngine();
    inline QQmlApplicationEngine const &qmlEngine() const { return m_engine; }
    Q_INVOKABLE void createWindow(QUrl, QQmlContext * = nullptr, QUuid = QUuid());
    Q_INVOKABLE void removeWindow(QObject*);

    QStringList qmlSourceDir() const;
    void setQmlSourceDir(QStringList);

public slots:
    void onFileChanged(QString);
    void onDestroyed(QObject *);
    void onObjectCreated(QObject *, QUrl, QUuid);

signals:
    void sObjectCreated(QUuid, QUrl, QObject*);
    void sObjectDestroyed(QUuid);
};
//} // namespace DesignPattern
