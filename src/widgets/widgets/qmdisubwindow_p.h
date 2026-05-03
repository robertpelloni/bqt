// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMDISUBWINDOW_P_H
#define QMDISUBWINDOW_P_H

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
#include "qmdisubwindow.h"

#include <QStyle>
#include <QStyleOptionTitleBar>
#if BOBUI_CONFIG(menubar)
#include <QMenuBar>
#endif
#if BOBUI_CONFIG(sizegrip)
#include <QSizeGrip>
#endif
#include <QBasicTimer>
#include <QPointer>
#include <private/qwidget_p.h>

BOBUI_REQUIRE_CONFIG(mdiarea);

BOBUI_BEGIN_NAMESPACE

class QVBoxLayout;
class QMouseEvent;

namespace QMdi {
template<typename T>
class ControlElement : public T             // ELFVERSION:ignore
{
public:
    ControlElement(QMdiSubWindow *child) : T(nullptr)
    {
        Q_ASSERT(child);
        mdiChild = child;
    }

    void *bobui_metacast(const char *classname) override
    {
        if (classname && strcmp(classname, "ControlElement") == 0)
            return this;
        return nullptr;
    }

    QPointer<QMdiSubWindow> mdiChild;
};

class ControlContainer : public QObject     // ELFVERSION:ignore
{
public:
    ControlContainer(QMdiSubWindow *mdiChild);
    ~ControlContainer();

#if BOBUI_CONFIG(menubar)
    void showButtonsInMenuBar(QMenuBar *menuBar);
    void removeButtonsFromMenuBar(QMenuBar *menuBar = nullptr);
    QMenuBar *menuBar() const { return m_menuBar; }
#endif
    void updateWindowIcon(const QIcon &windowIcon);
    QWidget *controllerWidget() const { return m_controllerWidget; }
    QWidget *systemMenuLabel() const { return m_menuLabel; }

private:
    QPointer<QWidget> previousLeft;
    QPointer<QWidget> previousRight;
#if BOBUI_CONFIG(menubar)
    QPointer<QMenuBar> m_menuBar;
#endif
    QPointer<QWidget> m_controllerWidget;
    QPointer<QWidget> m_menuLabel;
    QPointer<QMdiSubWindow> mdiChild;
};
} // namespace QMdi

class Q_AUTOTEST_EXPORT QMdiSubWindowPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QMdiSubWindow)
public:
    // Enums and typedefs.
    enum Operation {
        None,
        Move,
        TopResize,
        BottomResize,
        LeftResize,
        RightResize,
        TopLeftResize,
        TopRightResize,
        BottomLeftResize,
        BottomRightResize
    };

    enum ChangeFlag {
        HMove = 0x01,
        VMove = 0x02,
        HResize = 0x04,
        VResize = 0x08,
        HResizeReverse = 0x10,
        VResizeReverse = 0x20
    };

    enum WindowStateAction {
        RestoreAction,
        MoveAction,
        ResizeAction,
        MinimizeAction,
        MaximizeAction,
        StayOnTopAction,
        CloseAction,
        /* Add new states _above_ this line! */
        NumWindowStateActions
    };

    struct OperationInfo {
        uint changeFlags;
        BobUI::CursorShape cursorShape;
        QRegion region;
        bool hover;
        OperationInfo(uint changeFlags, BobUI::CursorShape cursorShape, bool hover = true)
            : changeFlags(changeFlags),
              cursorShape(cursorShape),
              hover(hover)
        {}
    };

    typedef QMap<Operation, OperationInfo> OperationInfoMap;

    QMdiSubWindowPrivate();

    // Variables.
    QPointer<QWidget> baseWidget;
    QPointer<QWidget> restoreFocusWidget;
    QPointer<QMdi::ControlContainer> controlContainer;
#if BOBUI_CONFIG(sizegrip)
    QPointer<QSizeGrip> sizeGrip;
#endif
#if BOBUI_CONFIG(rubberband)
    QRubberBand *rubberBand;
#endif
    QPoint mousePressPosition;
    QRect oldGeometry;
    QSize internalMinimumSize;
    QSize userMinimumSize;
    QSize restoreSize;
    bool resizeEnabled;
    bool moveEnabled;
    bool isInInteractiveMode;
#if BOBUI_CONFIG(rubberband)
    bool isInRubberBandMode;
#endif
    bool isShadeMode;
    bool ignoreWindowTitleChange;
    bool ignoreNextActivationEvent;
    bool activationEnabled;
    bool isShadeRequestFromMinimizeMode;
    bool isMaximizeMode;
    bool isWidgetHiddenByUs;
    bool isActive;
    bool isExplicitlyDeactivated;
    int keyboardSingleStep;
    int keyboardPageStep;
    QBasicTimer resizeTimer;
    Operation currentOperation;
    QStyle::SubControl hoveredSubControl;
    QStyle::SubControl activeSubControl;
    BobUI::FocusReason focusInReason;
    OperationInfoMap operationMap;
    QPointer<QMenu> systemMenu;
#ifndef BOBUI_NO_ACTION
    QPointer<QAction> actions[NumWindowStateActions];
#endif
    QMdiSubWindow::SubWindowOptions options;
    QString lastChildWindowTitle;
    QPalette titleBarPalette;
    QString windowTitle;
    QFont font;
    QIcon menuIcon;
    QStyleOptionTitleBar cachedStyleOptions;
    QString originalTitle;

    // Slots.
    void _q_updateStaysOnTopHint();
    void _q_enterInteractiveMode();
    void _q_processFocusChanged(QWidget *old, QWidget *now);

    // Functions.
    void leaveInteractiveMode();
    void removeBaseWidget();
    void initOperationMap();
#if BOBUI_CONFIG(menu)
    void createSystemMenu();
#endif
    void updateCursor();
    void updateDirtyRegions();
    void updateGeometryConstraints();
    void updateMask();
    void setNewGeometry(const QPoint &pos);
    void setMinimizeMode();
    void setNormalMode();
    void setMaximizeMode();
    void setActive(bool activate, bool changeFocus = true);
    void processClickedSubControl();
    QRegion getRegion(Operation operation) const;
    Operation getOperation(const QPoint &pos) const;
    QStyleOptionTitleBar titleBarOptions() const;
    void ensureWindowState(BobUI::WindowState state);
    int titleBarHeight(const QStyleOptionTitleBar &options) const;
    void sizeParameters(int *margin, int *minWidth) const;
    bool drawTitleBarWhenMaximized() const;
#if BOBUI_CONFIG(menubar)
    QMenuBar *menuBar() const;
    void showButtonsInMenuBar(QMenuBar *menuBar);
    void removeButtonsFromMenuBar();
#endif
    void updateWindowTitle(bool requestFromChild);
#if BOBUI_CONFIG(rubberband)
    void enterRubberBandMode();
    void leaveRubberBandMode();
#endif
    QPalette desktopPalette() const;
    void updateActions();
    void setFocusWidget();
    bool restoreFocus();
    void storeFocusWidget();
    void setWindowFlags(BobUI::WindowFlags windowFlags) override;
    using QWidgetPrivate::setVisible;
    void setVisible(WindowStateAction, bool visible = true);
#ifndef BOBUI_NO_ACTION
    void setEnabled(WindowStateAction, bool enable = true);
#if BOBUI_CONFIG(menu)
    void addToSystemMenu(WindowStateAction, const QString &text, const char *slot);
#endif
#endif // BOBUI_NO_ACTION
    QSize iconSize() const;
#if BOBUI_CONFIG(sizegrip)
    void setSizeGrip(QSizeGrip *sizeGrip);
    void setSizeGripVisible(bool visible = true) const;
#endif
    void updateInternalWindowTitle();
    QString originalWindowTitle();
    QString originalWindowTitleHelper() const;
    void setNewWindowTitle();

    inline int titleBarHeight() const
    {
        Q_Q(const QMdiSubWindow);
        if (!parent || q->windowFlags() & BobUI::FramelessWindowHint
            || (q->isMaximized() && !drawTitleBarWhenMaximized())) {
            return 0;
        }
        QStyleOptionTitleBar options = titleBarOptions();
        int height = options.rect.height();
        if (hasBorder(options))
            height += q->isMinimized() ? 8 : 4;
        return height;
    }

    inline QStyle::SubControl getSubControl(const QPoint &pos) const
    {
        Q_Q(const QMdiSubWindow);
        QStyleOptionTitleBar titleBarOptions = this->titleBarOptions();
        return q->style()->hitTestComplexControl(QStyle::CC_TitleBar, &titleBarOptions, pos, q);
    }

    inline void setNewGeometry(QRect *geometry)
    {
        Q_Q(QMdiSubWindow);
        Q_ASSERT(parent);
        geometry->setSize(geometry->size().expandedTo(internalMinimumSize));
#if BOBUI_CONFIG(rubberband)
        if (isInRubberBandMode)
            rubberBand->setGeometry(*geometry);
        else
#endif
            q->setGeometry(*geometry);
    }

    inline bool hasBorder(const QStyleOptionTitleBar &options) const
    {
        Q_Q(const QMdiSubWindow);
        return !q->style()->styleHint(QStyle::SH_TitleBar_NoBorder, &options, q);
    }

    inline bool autoRaise() const
    {
        Q_Q(const QMdiSubWindow);
        return q->style()->styleHint(QStyle::SH_TitleBar_AutoRaise, nullptr, q);
    }

    inline bool isResizeOperation() const
    {
        return currentOperation != None && currentOperation != Move;
    }

    inline bool isMoveOperation() const
    {
        return currentOperation == Move;
    }
};

BOBUI_END_NAMESPACE

#endif // QMDISUBWINDOW_P_H
