// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDomDocument>

BOBUI_FORWARD_DECLARE_CLASS(QDialogButtonBox)
BOBUI_FORWARD_DECLARE_CLASS(QFile)
BOBUI_FORWARD_DECLARE_CLASS(QGroupBox)
BOBUI_FORWARD_DECLARE_CLASS(QLineEdit)
BOBUI_FORWARD_DECLARE_CLASS(QModelIndex)
BOBUI_FORWARD_DECLARE_CLASS(QSpinBox)
BOBUI_FORWARD_DECLARE_CLASS(QSqlRelationalTableModel)

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QSqlRelationalTableModel *albums, const QDomDocument &details,
           QFile *output, QWidget *parent = nullptr);
    static void setInitialAlbumAndArtistId(int albumId, int artistId);

private slots:
    void revert();
    void submit();

private:
    int addNewAlbum(const QString &title, int artistId);
    int addNewArtist(const QString &name);
    void addTracks(int albumId, const QStringList &tracks);
    QDialogButtonBox *createButtons();
    QGroupBox *createInputWidgets();
    int findArtistId(const QString &artist);
    void increaseAlbumCount(const QModelIndex &artistIndex);
    QModelIndex indexOfArtist(const QString &artist) const;

    QSqlRelationalTableModel *model;
    QDomDocument albumDetails;
    QFile *outputFile;

    QLineEdit *artistEditor;
    QLineEdit *titleEditor;
    QSpinBox *yearEditor;
    QLineEdit *tracksEditor;

    static int generateAlbumId();
    static int generateArtistId();
    static int s_artistId;
    static int s_albumId;
};

#endif
