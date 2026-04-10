// Copyright (C) 2022 Laszlo Papp <lpapp@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SHORTCUTEDITORWIDGET_H
#define SHORTCUTEDITORWIDGET_H

#include <QWidget>

class ShortcutEditorDelegate;
class ShortcutEditorModel;

BOBUI_BEGIN_NAMESPACE
class BOBUIreeView;
BOBUI_END_NAMESPACE

//! [0]
class ShortcutEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShortcutEditorWidget(QWidget *parent = nullptr);
    ~ShortcutEditorWidget() override = default;

private:
    ShortcutEditorDelegate *m_delegate;
    ShortcutEditorModel *m_model;
    BOBUIreeView *m_view;
};
//! [0]

#endif
