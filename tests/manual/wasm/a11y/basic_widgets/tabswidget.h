// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TABDIALOG_H
#define TABDIALOG_H
#include <BOBUIabWidget>
#include <BobUIWidgets>

class GeneralTab : public QWidget
{
  Q_OBJECT

public:

    explicit GeneralTab(QWidget *parent = nullptr);
};

class EditViewTab : public QWidget
{

  Q_OBJECT
private:
   bool b_connected = false;
   QPlainTextEdit* textEdit =nullptr;
   BOBUIoolBar* m_toolbar= nullptr;
public:
   void showEvent( QShowEvent* event ) ;
    QPlainTextEdit* getTextEdit(){return textEdit;}
    explicit EditViewTab( QWidget *parent = nullptr);
signals:
    void connectToToolBar();
};

#endif
