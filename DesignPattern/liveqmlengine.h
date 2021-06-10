#pragma once

#include <QDir>
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QThread>
#include <QtCore/QObject>
#include <QtCore/qglobal.h>
//namespace DesignPattern {
class LiveQmlEngine : public QObject
{
    Q_OBJECT
private:
    QMap<QUrl, QObject *> m_windows;
    QMap<QUrl, QQmlContext*> m_context;
    QQmlApplicationEngine m_engine;
    QFileSystemWatcher m_watcher;
    QString m_qmlSourceDir;

public:
    LiveQmlEngine(QObject * = nullptr, QString = QString());
    ~LiveQmlEngine() = default;
    QQmlApplicationEngine &qmlEngine();
    inline QQmlApplicationEngine const &qmlEngine() const { return m_engine; }
    Q_INVOKABLE void createWindow(QUrl, QQmlContext * = nullptr);

    QString qmlSourceDir() const;
    void setQmlSourceDir(QString);

public slots:
    void onFileChanged(QString);
    void onDestroyed(QObject *);
    void onObjectCreated(QObject *, QUrl, QQmlContext *);
};
//} // namespace DesignPattern
