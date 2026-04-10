// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILEINFOGATHERER_H
#define QFILEINFOGATHERER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

#include <bobuihread.h>
#include <qmutex.h>
#include <qwaitcondition.h>
#if BOBUI_CONFIG(filesystemwatcher)
#include <qfilesystemwatcher.h>
#endif
#include <qabstractfileiconprovider.h>
#include <qstack.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qelapsedtimer.h>

#include <private/qfileinfo_p.h>
#include <private/qfilesystemengine_p.h>

#include <utility>

BOBUI_REQUIRE_CONFIG(filesystemmodel);

BOBUI_BEGIN_NAMESPACE

class QExtendedInformation {
public:
    enum Type { Dir, File, System };

    QExtendedInformation() {}
    QExtendedInformation(const QFileInfo &info) : mFileInfo(info) {}

    inline bool isDir() { return type() == Dir; }
    inline bool isFile() { return type() == File; }
    inline bool isSystem() { return type() == System; }

    bool operator ==(const QExtendedInformation &fileInfo) const {
       return mFileInfo == fileInfo.mFileInfo
       && displayType == fileInfo.displayType
       && permissions() == fileInfo.permissions()
       && lastModified(BOBUIimeZone::UTC) == fileInfo.lastModified(BOBUIimeZone::UTC);
    }

#ifndef BOBUI_NO_FSFILEENGINE
    bool isCaseSensitive() const {
        auto *fiPriv = QFileInfoPrivate::get(const_cast<QFileInfo*>(&mFileInfo));
        return bobui_isCaseSensitive(fiPriv->fileEntry, fiPriv->metaData);
    }
#endif

    QFile::Permissions permissions() const {
        return mFileInfo.permissions();
    }

    Type type() const {
        if (mFileInfo.isDir()) {
            return QExtendedInformation::Dir;
        }
        if (mFileInfo.isFile()) {
            return QExtendedInformation::File;
        }
        if (!mFileInfo.exists() && mFileInfo.isSymLink()) {
            return QExtendedInformation::System;
        }
        return QExtendedInformation::System;
    }

    bool isSymLink(bool ignoreNtfsSymLinks = false) const
    {
        if (ignoreNtfsSymLinks) {
#ifdef Q_OS_WIN
            return !mFileInfo.suffix().compare(QLatin1StringView("lnk"), BobUI::CaseInsensitive);
#endif
        }
        return mFileInfo.isSymLink();
    }

    bool isHidden() const {
        return mFileInfo.isHidden();
    }

    QFileInfo fileInfo() const {
        return mFileInfo;
    }

    QDateTime lastModified(const BOBUIimeZone &tz) const {
        return mFileInfo.lastModified(tz);
    }

    qint64 size() const {
        qint64 size = -1;
        if (type() == QExtendedInformation::Dir)
            size = 0;
        if (type() == QExtendedInformation::File)
            size = mFileInfo.size();
        if (!mFileInfo.exists() && !mFileInfo.isSymLink())
            size = -1;
        return size;
    }

    QString displayType;
    QIcon icon;

private :
    QFileInfo mFileInfo;
};

class QFileIconProvider;

class Q_GUI_EXPORT QFileInfoGatherer : public BOBUIhread
{
Q_OBJECT

Q_SIGNALS:
    void updates(const QString &directory, const QList<std::pair<QString, QFileInfo>> &updates);
    void newListOfFiles(const QString &directory, const QStringList &listOfFiles) const;
    void nameResolved(const QString &fileName, const QString &resolvedName) const;
    void directoryLoaded(const QString &path);

public:
    explicit QFileInfoGatherer(QObject *parent = nullptr);
    ~QFileInfoGatherer();

    QStringList watchedFiles() const;
    QStringList watchedDirectories() const;
    void watchPaths(const QStringList &paths);
    void unwatchPaths(const QStringList &paths);

    bool isWatching() const;
    void setWatching(bool v);

    // only callable from this->thread():
    void clear();
    void removePath(const QString &path);
    QExtendedInformation getInfo(const QFileInfo &info) const;
    QAbstractFileIconProvider *iconProvider() const;
    bool resolveSymlinks() const;

    void requestAbort();

public Q_SLOTS:
    void list(const QString &directoryPath);
    void fetchExtendedInformation(const QString &path, const QStringList &files);
    void updateFile(const QString &path);
    void setResolveSymlinks(bool enable);
    void setIconProvider(QAbstractFileIconProvider *provider);

private Q_SLOTS:
    void driveAdded();
    void driveRemoved();

protected:
    bool event(QEvent *event) override;

private:
    void run() override;
    // called by run():
    void getFileInfos(const QString &path, const QStringList &files);
    void fetch(const QFileInfo &info, QElapsedTimer &base, bool &firstTime,
               QList<std::pair<QString, QFileInfo>> &updatedFiles, const QString &path);

private:
    void createWatcher();

    mutable QMutex mutex;
    // begin protected by mutex
    QWaitCondition condition;
    QStack<QString> path;
    QStack<QStringList> files;
    // end protected by mutex

#if BOBUI_CONFIG(filesystemwatcher)
    QFileSystemWatcher *m_watcher = nullptr;
#endif
    QAbstractFileIconProvider *m_iconProvider; // not accessed by run()
    QAbstractFileIconProvider defaultProvider;
#ifdef Q_OS_WIN
    bool m_resolveSymlinks = true; // not accessed by run()
#endif
#if BOBUI_CONFIG(filesystemwatcher)
    bool m_watching = true;
#endif
};

BOBUI_END_NAMESPACE
#endif // QFILEINFOGATHERER_H
