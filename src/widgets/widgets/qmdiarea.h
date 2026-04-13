// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMDIAREA_H
#define QMDIAREA_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractscrollarea.h>
#if BOBUI_CONFIG(tabwidget)
#include <BobUIWidgets/bobuiabwidget.h>
#endif

BOBUI_REQUIRE_CONFIG(mdiarea);

BOBUI_BEGIN_NAMESPACE

class QMdiSubWindow;

class QMdiAreaPrivate;
class Q_WIDGETS_EXPORT QMdiArea : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(WindowOrder activationOrder READ activationOrder WRITE setActivationOrder)
    Q_PROPERTY(ViewMode viewMode READ viewMode WRITE setViewMode)
#if BOBUI_CONFIG(tabbar)
    Q_PROPERTY(bool documentMode READ documentMode WRITE setDocumentMode)
    Q_PROPERTY(bool tabsClosable READ tabsClosable WRITE setTabsClosable)
    Q_PROPERTY(bool tabsMovable READ tabsMovable WRITE setTabsMovable)
#endif
#if BOBUI_CONFIG(tabwidget)
    Q_PROPERTY(BOBUIabWidget::TabShape tabShape READ tabShape WRITE setTabShape)
    Q_PROPERTY(BOBUIabWidget::TabPosition tabPosition READ tabPosition WRITE setTabPosition)
#endif
public:
    enum AreaOption {
        DontMaximizeSubWindowOnActivation = 0x1
    };
    Q_DECLARE_FLAGS(AreaOptions, AreaOption)

    enum WindowOrder {
        CreationOrder,
        StackingOrder,
        ActivationHistoryOrder
    };
    Q_ENUM(WindowOrder)

    enum ViewMode {
        SubWindowView,
        TabbedView
    };
    Q_ENUM(ViewMode)

    QMdiArea(QWidget *parent = nullptr);
    ~QMdiArea();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QMdiSubWindow *currentSubWindow() const;
    QMdiSubWindow *activeSubWindow() const;
    QList<QMdiSubWindow *> subWindowList(WindowOrder order = CreationOrder) const;

    QMdiSubWindow *addSubWindow(QWidget *widget, BobUI::WindowFlags flags = BobUI::WindowFlags());
    void removeSubWindow(QWidget *widget);

    QBrush background() const;
    void setBackground(const QBrush &background);

    WindowOrder activationOrder() const;
    void setActivationOrder(WindowOrder order);

    void setOption(AreaOption option, bool on = true);
    bool testOption(AreaOption opton) const;

    void setViewMode(ViewMode mode);
    ViewMode viewMode() const;

#if BOBUI_CONFIG(tabbar)
    bool documentMode() const;
    void setDocumentMode(bool enabled);

    void setTabsClosable(bool closable);
    bool tabsClosable() const;

    void setTabsMovable(bool movable);
    bool tabsMovable() const;
#endif
#if BOBUI_CONFIG(tabwidget)
    void setTabShape(BOBUIabWidget::TabShape shape);
    BOBUIabWidget::TabShape tabShape() const;

    void setTabPosition(BOBUIabWidget::TabPosition position);
    BOBUIabWidget::TabPosition tabPosition() const;
#endif

Q_SIGNALS:
    void subWindowActivated(QMdiSubWindow *);

public Q_SLOTS:
    void setActiveSubWindow(QMdiSubWindow *window);
    void tileSubWindows();
    void cascadeSubWindows();
    void closeActiveSubWindow();
    void closeAllSubWindows();
    void activateNextSubWindow();
    void activatePreviousSubWindow();

protected Q_SLOTS:
    void setupViewport(QWidget *viewport) override;

protected:
    bool event(QEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void paintEvent(QPaintEvent *paintEvent) override;
    void childEvent(QChildEvent *childEvent) override;
    void resizeEvent(QResizeEvent *resizeEvent) override;
    void timerEvent(BOBUIimerEvent *timerEvent) override;
    void showEvent(QShowEvent *showEvent) override;
    bool viewportEvent(QEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;

private:
    Q_DISABLE_COPY(QMdiArea)
    Q_DECLARE_PRIVATE(QMdiArea)
    Q_PRIVATE_SLOT(d_func(), void _q_deactivateAllWindows())
    Q_PRIVATE_SLOT(d_func(), void _q_processWindowStateChanged(BobUI::WindowStates, BobUI::WindowStates))
    Q_PRIVATE_SLOT(d_func(), void _q_currentTabChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_closeTab(int))
    Q_PRIVATE_SLOT(d_func(), void _q_moveTab(int, int))
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QMdiArea::AreaOptions)

BOBUI_END_NAMESPACE

#endif // QMDIAREA_H
