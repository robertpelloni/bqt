// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICMAINWINDOW_H
#define QDYNAMICMAINWINDOW_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>
#if BOBUI_CONFIG(tabwidget)
#include <BobUIWidgets/bobuiabwidget.h>
#endif

BOBUI_REQUIRE_CONFIG(mainwindow);

BOBUI_BEGIN_NAMESPACE

class QDockWidget;
class QMainWindowPrivate;
class QMenuBar;
class QStatusBar;
class BOBUIoolBar;
class QMenu;

class Q_WIDGETS_EXPORT QMainWindow : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(BobUI::ToolButtonStyle toolButtonStyle READ toolButtonStyle WRITE setToolButtonStyle)
#if BOBUI_CONFIG(dockwidget)
    Q_PROPERTY(bool animated READ isAnimated WRITE setAnimated)
#if BOBUI_CONFIG(tabbar)
    Q_PROPERTY(bool documentMode READ documentMode WRITE setDocumentMode)
#endif // BOBUI_CONFIG(tabbar)
#if BOBUI_CONFIG(tabwidget)
    Q_PROPERTY(BOBUIabWidget::TabShape tabShape READ tabShape WRITE setTabShape)
#endif // BOBUI_CONFIG(tabwidget)
    Q_PROPERTY(bool dockNestingEnabled READ isDockNestingEnabled WRITE setDockNestingEnabled)
#endif // BOBUI_CONFIG(dockwidget)
    Q_PROPERTY(DockOptions dockOptions READ dockOptions WRITE setDockOptions)
#if BOBUI_CONFIG(toolbar)
    Q_PROPERTY(bool unifiedTitleAndToolBarOnMac READ unifiedTitleAndToolBarOnMac
               WRITE setUnifiedTitleAndToolBarOnMac)
#endif

public:
    enum DockOption {
        AnimatedDocks = 0x01,
        AllowNestedDocks = 0x02,
        AllowTabbedDocks = 0x04,
        ForceTabbedDocks = 0x08,  // implies AllowTabbedDocks, !AllowNestedDocks
        VerticalTabs = 0x10,      // implies AllowTabbedDocks
        GroupedDragging = 0x20    // implies AllowTabbedDocks
    };
    Q_ENUM(DockOption)
    Q_DECLARE_FLAGS(DockOptions, DockOption)
    Q_FLAG(DockOptions)

    explicit QMainWindow(QWidget *parent = nullptr, BobUI::WindowFlags flags = BobUI::WindowFlags());
    ~QMainWindow();

    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);

    BobUI::ToolButtonStyle toolButtonStyle() const;
    void setToolButtonStyle(BobUI::ToolButtonStyle toolButtonStyle);

#if BOBUI_CONFIG(dockwidget)
    bool isAnimated() const;
    bool isDockNestingEnabled() const;
#endif

#if BOBUI_CONFIG(tabbar)
    bool documentMode() const;
    void setDocumentMode(bool enabled);
#endif

#if BOBUI_CONFIG(tabwidget)
    BOBUIabWidget::TabShape tabShape() const;
    void setTabShape(BOBUIabWidget::TabShape tabShape);
    BOBUIabWidget::TabPosition tabPosition(BobUI::DockWidgetArea area) const;
    void setTabPosition(BobUI::DockWidgetAreas areas, BOBUIabWidget::TabPosition tabPosition);
#endif // BOBUI_CONFIG(tabwidget)

    void setDockOptions(DockOptions options);
    DockOptions dockOptions() const;

    bool isSeparator(const QPoint &pos) const;

#if BOBUI_CONFIG(menubar)
    QMenuBar *menuBar() const;
    void setMenuBar(QMenuBar *menubar);

    QWidget  *menuWidget() const;
    void setMenuWidget(QWidget *menubar);
#endif

#if BOBUI_CONFIG(statusbar)
    QStatusBar *statusBar() const;
    void setStatusBar(QStatusBar *statusbar);
#endif

    QWidget *centralWidget() const;
    void setCentralWidget(QWidget *widget);

    QWidget *takeCentralWidget();

#if BOBUI_CONFIG(dockwidget)
    void setCorner(BobUI::Corner corner, BobUI::DockWidgetArea area);
    BobUI::DockWidgetArea corner(BobUI::Corner corner) const;
#endif

#if BOBUI_CONFIG(toolbar)
    void addToolBarBreak(BobUI::ToolBarArea area = BobUI::TopToolBarArea);
    void insertToolBarBreak(BOBUIoolBar *before);

    void addToolBar(BobUI::ToolBarArea area, BOBUIoolBar *toolbar);
    void addToolBar(BOBUIoolBar *toolbar);
    BOBUIoolBar *addToolBar(const QString &title);
    void insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolbar);
    void removeToolBar(BOBUIoolBar *toolbar);
    void removeToolBarBreak(BOBUIoolBar *before);

    bool unifiedTitleAndToolBarOnMac() const;

    BobUI::ToolBarArea toolBarArea(const BOBUIoolBar *toolbar) const;
    bool toolBarBreak(BOBUIoolBar *toolbar) const;
#endif // BOBUI_CONFIG(toolbar)

#if BOBUI_CONFIG(dockwidget)
    void addDockWidget(BobUI::DockWidgetArea area, QDockWidget *dockwidget);
    void addDockWidget(BobUI::DockWidgetArea area, QDockWidget *dockwidget,
                       BobUI::Orientation orientation);
    void splitDockWidget(QDockWidget *after, QDockWidget *dockwidget,
                         BobUI::Orientation orientation);
#if BOBUI_CONFIG(tabbar)
    void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
    QList<QDockWidget*> tabifiedDockWidgets(QDockWidget *dockwidget) const;
#endif
    void removeDockWidget(QDockWidget *dockwidget);
    bool restoreDockWidget(QDockWidget *dockwidget);

    BobUI::DockWidgetArea dockWidgetArea(QDockWidget *dockwidget) const;

    void resizeDocks(const QList<QDockWidget *> &docks,
                     const QList<int> &sizes, BobUI::Orientation orientation);
#endif // BOBUI_CONFIG(dockwidget)

    QByteArray saveState(int version = 0) const;
    bool restoreState(const QByteArray &state, int version = 0);

#if BOBUI_CONFIG(menu)
    virtual QMenu *createPopupMenu();
#endif

public Q_SLOTS:
#if BOBUI_CONFIG(dockwidget)
    void setAnimated(bool enabled);
    void setDockNestingEnabled(bool enabled);
#endif
#if BOBUI_CONFIG(toolbar)
    void setUnifiedTitleAndToolBarOnMac(bool set);
#endif

Q_SIGNALS:
    void iconSizeChanged(const QSize &iconSize);
    void toolButtonStyleChanged(BobUI::ToolButtonStyle toolButtonStyle);
#if BOBUI_CONFIG(dockwidget)
    void tabifiedDockWidgetActivated(QDockWidget *dockWidget);
#endif

protected:
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif
    bool event(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(QMainWindow)
    Q_DISABLE_COPY(QMainWindow)
    friend class QDockWidgetGroupWindow;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QMainWindow::DockOptions)

BOBUI_END_NAMESPACE

#endif // QDYNAMICMAINWINDOW_H
