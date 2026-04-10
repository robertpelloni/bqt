// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QFileIconProvider>

//![0]
class FileListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FileListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;

    QFileInfo fileInfoAt(const QModelIndex &) const;

signals:
    void numberPopulated(const QString &path, int start, int number, int total);

public slots:
    void setDirPath(const QString &path);

protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

private:
    QFileInfoList fileList;
    QString path;
    QFileIconProvider iconProvider;
    int fileCount = 0;
};
//![0]

#endif // FILELISTMODEL_H
