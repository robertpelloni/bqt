// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef INTERACTIVEWIDGET_H
#define INTERACTIVEWIDGET_H

#include "widgets.h"
#include "paintcommands.h"

#include <QMainWindow>

#include <private/qmath_p.h>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIoolBox)

class InteractiveWidget : public QMainWindow
{
    Q_OBJECT
public:
    InteractiveWidget();

public slots:
    void run();
    void load();
    void load(const QString &fname);
    void save();

protected:
    bool eventFilter(QObject *o, QEvent *e) override;

protected slots:
    void cmdSelected(QListWidgetItem *item);
    void enumSelected(QListWidgetItem *item);

private:
    BOBUIoolBox *m_commandsToolBox;
    BOBUIoolBox *m_enumsToolBox;
    OnScreenWidget<QWidget> *m_onScreenWidget;
    BOBUIextEdit *ui_textEdit;
    QString m_filename;
};

#endif
