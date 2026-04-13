// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICTOOLBAR_H
#define QDYNAMICTOOLBAR_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/qaction.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(toolbar);

BOBUI_BEGIN_NAMESPACE

class BOBUIoolBarPrivate;

class QAction;
class QIcon;
class QMainWindow;
class QStyleOptionToolBar;

class Q_WIDGETS_EXPORT BOBUIoolBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool movable READ isMovable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(BobUI::ToolBarAreas allowedAreas READ allowedAreas WRITE setAllowedAreas
               NOTIFY allowedAreasChanged)
    Q_PROPERTY(BobUI::Orientation orientation READ orientation WRITE setOrientation
               NOTIFY orientationChanged)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
    Q_PROPERTY(BobUI::ToolButtonStyle toolButtonStyle READ toolButtonStyle WRITE setToolButtonStyle
               NOTIFY toolButtonStyleChanged)
    Q_PROPERTY(bool floating READ isFloating)
    Q_PROPERTY(bool floatable READ isFloatable WRITE setFloatable)

public:
    explicit BOBUIoolBar(const QString &title, QWidget *parent = nullptr);
    explicit BOBUIoolBar(QWidget *parent = nullptr);
    ~BOBUIoolBar();

    void setMovable(bool movable);
    bool isMovable() const;

    void setAllowedAreas(BobUI::ToolBarAreas areas);
    BobUI::ToolBarAreas allowedAreas() const;

    inline bool isAreaAllowed(BobUI::ToolBarArea area) const
    { return (allowedAreas() & area) == area; }

    void setOrientation(BobUI::Orientation orientation);
    BobUI::Orientation orientation() const;

    void clear();

    using QWidget::addAction;
#if BOBUI_WIDGETS_REMOVED_SINCE(6, 3)
    QAction *addAction(const QString &text);
    QAction *addAction(const QIcon &icon, const QString &text);
    QAction *addAction(const QString &text, const QObject *receiver, const char* member);
    QAction *addAction(const QIcon &icon, const QString &text,
                       const QObject *receiver, const char* member);
#endif

    QAction *addSeparator();
    QAction *insertSeparator(QAction *before);

    QAction *addWidget(QWidget *widget);
    QAction *insertWidget(QAction *before, QWidget *widget);

    QRect actionGeometry(QAction *action) const;
    QAction *actionAt(const QPoint &p) const;
    inline QAction *actionAt(int x, int y) const;

    QAction *toggleViewAction() const;

    QSize iconSize() const;
    BobUI::ToolButtonStyle toolButtonStyle() const;

    QWidget *widgetForAction(QAction *action) const;

    bool isFloatable() const;
    void setFloatable(bool floatable);
    bool isFloating() const;

public Q_SLOTS:
    void setIconSize(const QSize &iconSize);
    void setToolButtonStyle(BobUI::ToolButtonStyle toolButtonStyle);

Q_SIGNALS:
    void actionTriggered(QAction *action);
    void movableChanged(bool movable);
    void allowedAreasChanged(BobUI::ToolBarAreas allowedAreas);
    void orientationChanged(BobUI::Orientation orientation);
    void iconSizeChanged(const QSize &iconSize);
    void toolButtonStyleChanged(BobUI::ToolButtonStyle toolButtonStyle);
    void topLevelChanged(bool topLevel);
    void visibilityChanged(bool visible);

protected:
    void actionEvent(QActionEvent *event) override;
    void changeEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
    virtual void initStyleOption(QStyleOptionToolBar *option) const;


private:
    Q_DECLARE_PRIVATE(BOBUIoolBar)
    Q_DISABLE_COPY(BOBUIoolBar)
    Q_PRIVATE_SLOT(d_func(), void _q_toggleView(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_updateIconSize(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_updateToolButtonStyle(BobUI::ToolButtonStyle))

    friend class QMainWindow;
    friend class QMainWindowLayout;
    friend class BOBUIoolBarLayout;
    friend class BOBUIoolBarAreaLayout;
};

inline QAction *BOBUIoolBar::actionAt(int ax, int ay) const
{ return actionAt(QPoint(ax, ay)); }

BOBUI_END_NAMESPACE

#endif // QDYNAMICTOOLBAR_H
