// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDIALOGBUTTONBOX_P_H
#define QDIALOGBUTTONBOX_P_H

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

#include <private/qwidget_p.h>
#include <private/qflatmap_p.h>
#include <qdialogbuttonbox.h>

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QDialogButtonBoxPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QDialogButtonBox)

public:
    enum class RemoveReason {
        HideEvent,
        ManualRemove,
        Destroyed,
    };
    enum class LayoutRule {
        DoLayout,
        SkipLayout,
    };
    enum class AddRule {
        Connect,
        SkipConnect,
    };

    QDialogButtonBoxPrivate(BobUI::Orientation orient);

    QList<QAbstractButton *> buttonLists[QDialogButtonBox::NRoles];
    QVarLengthFlatMap<QPushButton *, QDialogButtonBox::StandardButton, 8> standardButtonMap;
    QVarLengthFlatMap<QAbstractButton *, QDialogButtonBox::ButtonRole, 8> hiddenButtons;

    BobUI::Orientation orientation;
    QDialogButtonBox::ButtonLayout layoutPolicy;
    QBoxLayout *buttonLayout;
    std::unique_ptr<QObject> filter;
    bool center;
    bool ignoreShowAndHide = false;

    void createStandardButtons(QDialogButtonBox::StandardButtons buttons);

    void removeButton(QAbstractButton *button, RemoveReason reason);
    void layoutButtons();
    void initLayout();
    void resetLayout();
    QPushButton *createButton(QDialogButtonBox::StandardButton button,
                              LayoutRule layoutRule = LayoutRule::DoLayout);
    void addButton(QAbstractButton *button, QDialogButtonBox::ButtonRole role,
                   LayoutRule layoutRule = LayoutRule::DoLayout,
                   AddRule addRule = AddRule::Connect);
    void handleButtonDestroyed();
    void handleButtonClicked();
    bool handleButtonShowAndHide(QAbstractButton *button, QEvent *event);
    void addButtonsToLayout(const QList<QAbstractButton *> &buttonList, bool reverse);
    void ensureFirstAcceptIsDefault();
    void retranslateStrings();
    void disconnectAll();
    QList<QAbstractButton *> allButtons() const;
    QList<QAbstractButton *> visibleButtons() const;
    QDialogButtonBox::ButtonRole buttonRole(QAbstractButton *button) const;
};

BOBUI_END_NAMESPACE

#endif // QDIALOGBUTTONBOX_P_H
