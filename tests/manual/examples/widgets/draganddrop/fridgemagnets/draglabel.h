// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QLabel>

BOBUI_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDragMoveEvent;
class QFrame;
BOBUI_END_NAMESPACE

//! [0]
class DragLabel : public QLabel
{
public:
    DragLabel(const QString &text, QWidget *parent);
    QString labelText() const;

private:
    QString m_labelText;
};
//! [0]

#endif // DRAGLABEL_H
