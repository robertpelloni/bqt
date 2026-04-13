// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIABWIDGET_H
#define BOBUIABWIDGET_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>
#include <BobUIGui/qicon.h>

BOBUI_REQUIRE_CONFIG(tabwidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIabBar;
class BOBUIabWidgetPrivate;
class QStyleOptionTabWidgetFrame;

class Q_WIDGETS_EXPORT BOBUIabWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(TabPosition tabPosition READ tabPosition WRITE setTabPosition)
    Q_PROPERTY(TabShape tabShape READ tabShape WRITE setTabShape)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(BobUI::TextElideMode elideMode READ elideMode WRITE setElideMode)
    Q_PROPERTY(bool usesScrollButtons READ usesScrollButtons WRITE setUsesScrollButtons)
    Q_PROPERTY(bool documentMode READ documentMode WRITE setDocumentMode)
    Q_PROPERTY(bool tabsClosable READ tabsClosable WRITE setTabsClosable)
    Q_PROPERTY(bool movable READ isMovable WRITE setMovable)
    Q_PROPERTY(bool tabBarAutoHide READ tabBarAutoHide WRITE setTabBarAutoHide)

public:
    explicit BOBUIabWidget(QWidget *parent = nullptr);
    ~BOBUIabWidget();

    int addTab(QWidget *widget, const QString &);
    int addTab(QWidget *widget, const QIcon& icon, const QString &label);

    int insertTab(int index, QWidget *widget, const QString &);
    int insertTab(int index, QWidget *widget, const QIcon& icon, const QString &label);

    void removeTab(int index);

    bool isTabEnabled(int index) const;
    void setTabEnabled(int index, bool enabled);

    bool isTabVisible(int index) const;
    void setTabVisible(int index, bool visible);

    QString tabText(int index) const;
    void setTabText(int index, const QString &text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon & icon);

#if BOBUI_CONFIG(tooltip)
    void setTabToolTip(int index, const QString & tip);
    QString tabToolTip(int index) const;
#endif

#if BOBUI_CONFIG(whatsthis)
    void setTabWhatsThis(int index, const QString &text);
    QString tabWhatsThis(int index) const;
#endif

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(const QWidget *widget) const;
    int count() const;

    enum TabPosition { North, South, West, East };
    Q_ENUM(TabPosition)
    TabPosition tabPosition() const;
    void setTabPosition(TabPosition position);

    bool tabsClosable() const;
    void setTabsClosable(bool closeable);

    bool isMovable() const;
    void setMovable(bool movable);

    enum TabShape { Rounded, Triangular };
    Q_ENUM(TabShape)
    TabShape tabShape() const;
    void setTabShape(TabShape s);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int heightForWidth(int width) const override;
    bool hasHeightForWidth() const override;

    void setCornerWidget(QWidget * w, BobUI::Corner corner = BobUI::TopRightCorner);
    QWidget * cornerWidget(BobUI::Corner corner = BobUI::TopRightCorner) const;

    BobUI::TextElideMode elideMode() const;
    void setElideMode(BobUI::TextElideMode mode);

    QSize iconSize() const;
    void setIconSize(const QSize &size);

    bool usesScrollButtons() const;
    void setUsesScrollButtons(bool useButtons);

    bool documentMode() const;
    void setDocumentMode(bool set);

    bool tabBarAutoHide() const;
    void setTabBarAutoHide(bool enabled);

    void clear();

    BOBUIabBar* tabBar() const;

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

Q_SIGNALS:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabBarClicked(int index);
    void tabBarDoubleClicked(int index);

protected:
    virtual void tabInserted(int index);
    virtual void tabRemoved(int index);

    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void setTabBar(BOBUIabBar *);
    void changeEvent(QEvent *) override;
    bool event(QEvent *) override;
    virtual void initStyleOption(QStyleOptionTabWidgetFrame *option) const;


private:
    Q_DECLARE_PRIVATE(BOBUIabWidget)
    Q_DISABLE_COPY(BOBUIabWidget)
    void setUpLayout(bool = false);
};

BOBUI_END_NAMESPACE

#endif // BOBUIABWIDGET_H
