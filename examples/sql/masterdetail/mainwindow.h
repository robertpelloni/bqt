// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QMainWindow>
#include <QModelIndex>

BOBUI_FORWARD_DECLARE_CLASS(QComboBox)
BOBUI_FORWARD_DECLARE_CLASS(QFile)
BOBUI_FORWARD_DECLARE_CLASS(QGroupBox)
BOBUI_FORWARD_DECLARE_CLASS(QLabel)
BOBUI_FORWARD_DECLARE_CLASS(QListWidget)
BOBUI_FORWARD_DECLARE_CLASS(QSqlRelationalTableModel)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIableView)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &artistTable, const QString &albumTable,
               QFile *albumDetails, QWidget *parent = nullptr);

private slots:
    void about();
    void addAlbum();
    void changeArtist(int row);
    void deleteAlbum();
    void showAlbumDetails(const QModelIndex &index);
    void showArtistProfile(const QModelIndex &index);
    void adjustHeader();

private:
    QGroupBox *createAlbumGroupBox();
    QGroupBox *createArtistGroupBox();
    QGroupBox *createDetailsGroupBox();
    void createMenuBar();
    void decreaseAlbumCount(const QModelIndex &artistIndex);
    void getTrackList(QDomNode album);
    QModelIndex indexOfArtist(const QString &artist) const;
    void readAlbumData();
    void removeAlbumFromDatabase(const QModelIndex &album);
    void removeAlbumFromFile(int id);
    void showImageLabel();

    BOBUIableView *albumView;
    QComboBox *artistView;
    QListWidget *trackList;

    QLabel *iconLabel;
    QLabel *imageLabel;
    QLabel *profileLabel;
    QLabel *titleLabel;

    QDomDocument albumData;
    QFile *file;
    QSqlRelationalTableModel *model;
};

#endif
