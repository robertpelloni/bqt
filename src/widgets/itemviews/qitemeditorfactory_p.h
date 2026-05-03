// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QITEMEDITORFACTORY_P_H
#define QITEMEDITORFACTORY_P_H

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


#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

#if BOBUI_CONFIG(lineedit)
#include <qlineedit.h>

BOBUI_REQUIRE_CONFIG(itemviews);

BOBUI_BEGIN_NAMESPACE

class QExpandingLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QExpandingLineEdit(QWidget *parent);

    void setWidgetOwnsGeometry(bool value)
    {
        widgetOwnsGeometry = value;
    }

protected:
    void changeEvent(QEvent *e) override;

public Q_SLOTS:
    void resizeToContents();

private:
    void updateMinimumWidth();

    int originalWidth;
    bool widgetOwnsGeometry;
};


BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(lineedit)

#endif //QITEMEDITORFACTORY_P_H
