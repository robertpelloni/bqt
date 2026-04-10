// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(accessibility)
#include <QAccessible>

namespace src_gui_accessible_qaccessible {
class MyWidget
{
    void setFocus(BobUI::FocusReason reason);
};
QAccessibleInterface *f = nullptr;

//! [1]
typedef QAccessibleInterface *myFactoryFunction(const QString &key, QObject *);
//! [1]

//! [2]
void MyWidget::setFocus(BobUI::FocusReason reason)
{
    // handle custom focus setting...
    QAccessibleEvent event(f, QAccessible::Focus);
    QAccessible::updateAccessibility(&event);
}
//! [2]

} // src_gui_accessible_qaccessible

#endif // BOBUI_CONFIG(accessibility)
