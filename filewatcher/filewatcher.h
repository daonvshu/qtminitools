#pragma once

#include <functional>
#include <QFileSystemWatcher>
#include <QTimer>

class QString;

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    FileWatcher(const std::function<void(const QString& fileName)>& callback);

    void setDirectory(const QString &path, const QString& suffix);

public slots:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    void addPaths(const QString& path, bool recurve = false);
    QStringList entrySubDirAndTargetFileSuffix(const QString& path, bool recurve);

private:
    QFileSystemWatcher watcher;
    QTimer timer;
    QString lastChangedPath;
    QString fileSuffix;
};
