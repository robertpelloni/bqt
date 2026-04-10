// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLBOX_H
#define BOBUIOOLBOX_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>
#include <BobUIGui/qicon.h>

BOBUI_REQUIRE_CONFIG(toolbox);

BOBUI_BEGIN_NAMESPACE

class BOBUIoolBoxPrivate;

class Q_WIDGETS_EXPORT BOBUIoolBox : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)

public:
    explicit BOBUIoolBox(QWidget *parent = nullptr, BobUI::WindowFlags f = BobUI::WindowFlags());
    ~BOBUIoolBox();

    int addItem(QWidget *widget, const QString &text);
    int addItem(QWidget *widget, const QIcon &icon, const QString &text);
    int insertItem(int index, QWidget *widget, const QString &text);
    int insertItem(int index, QWidget *widget, const QIcon &icon, const QString &text);

    void removeItem(int index);

    void setItemEnabled(int index, bool enabled);
    bool isItemEnabled(int index) const;

    void setItemText(int index, const QString &text);
    QString itemText(int index) const;

    void setItemIcon(int index, const QIcon &icon);
    QIcon itemIcon(int index) const;

#if BOBUI_CONFIG(tooltip)
    void setItemToolTip(int index, const QString &toolTip);
    QString itemToolTip(int index) const;
#endif

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(const QWidget *widget) const;
    int count() const;

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

Q_SIGNALS:
    void currentChanged(int index);

protected:
    bool event(QEvent *e) override;
    virtual void itemInserted(int index);
    virtual void itemRemoved(int index);
    void showEvent(QShowEvent *e) override;
    void changeEvent(QEvent *) override;


private:
    Q_DECLARE_PRIVATE(BOBUIoolBox)
    Q_DISABLE_COPY(BOBUIoolBox)
    Q_PRIVATE_SLOT(d_func(), void _q_buttonClicked())
    Q_PRIVATE_SLOT(d_func(), void _q_widgetDestroyed(QObject*))
};


inline int BOBUIoolBox::addItem(QWidget *item, const QString &text)
{ return insertItem(-1, item, QIcon(), text); }
inline int BOBUIoolBox::addItem(QWidget *item, const QIcon &iconSet,
                              const QString &text)
{ return insertItem(-1, item, iconSet, text); }
inline int BOBUIoolBox::insertItem(int index, QWidget *item, const QString &text)
{ return insertItem(index, item, QIcon(), text); }

BOBUI_END_NAMESPACE

#endif // BOBUIOOLBOX_H
