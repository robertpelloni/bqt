// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLAYOUT_H
#define QLAYOUT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIWidgets/qlayoutitem.h>
#include <BobUIWidgets/qsizepolicy.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qmargins.h>

#include <limits.h>

BOBUI_BEGIN_NAMESPACE


class QLayout;
class QSize;


class QLayoutPrivate;

class Q_WIDGETS_EXPORT QLayout : public QObject, public QLayoutItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QLayout)

    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(QMargins contentsMargins READ contentsMargins WRITE setContentsMargins
               RESET unsetContentsMargins)

    Q_PROPERTY(SizeConstraint sizeConstraint READ sizeConstraint WRITE setSizeConstraint)
    Q_PROPERTY(SizeConstraint horizontalSizeConstraint READ horizontalSizeConstraint WRITE setHorizontalSizeConstraint)
    Q_PROPERTY(SizeConstraint verticalSizeConstraint READ verticalSizeConstraint WRITE setVerticalSizeConstraint)

public:
    enum SizeConstraint {
        SetDefaultConstraint,
        SetNoConstraint,
        SetMinimumSize,
        SetFixedSize,
        SetMaximumSize,
        SetMinAndMaxSize
    };
    Q_ENUM(SizeConstraint)

    explicit QLayout(QWidget *parent = nullptr);
    ~QLayout();

    virtual int spacing() const;
    virtual void setSpacing(int);

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void unsetContentsMargins();
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;
    QRect contentsRect() const;

    bool setAlignment(QWidget *w, BobUI::Alignment alignment);
    bool setAlignment(QLayout *l, BobUI::Alignment alignment);
    using QLayoutItem::setAlignment;

    void setSizeConstraint(SizeConstraint constraint);
    SizeConstraint sizeConstraint() const;

    void setSizeConstraints(SizeConstraint horizontal, SizeConstraint vertical);

    void setHorizontalSizeConstraint(SizeConstraint constraint);
    SizeConstraint horizontalSizeConstraint() const;

    void setVerticalSizeConstraint(SizeConstraint constraint);
    SizeConstraint verticalSizeConstraint() const;

    void setMenuBar(QWidget *w);
    QWidget *menuBar() const;

    QWidget *parentWidget() const;

    void invalidate() override;
    QRect geometry() const override;
    bool activate();
    void update();

    void addWidget(QWidget *w);
    virtual void addItem(QLayoutItem *) = 0;

    void removeWidget(QWidget *w);
    void removeItem(QLayoutItem *);

    BobUI::Orientations expandingDirections() const override;
    QSize minimumSize() const override;
    QSize maximumSize() const override;
    virtual void setGeometry(const QRect&) override;
    virtual QLayoutItem *itemAt(int index) const = 0;
    virtual QLayoutItem *takeAt(int index) = 0;
    virtual int indexOf(const QWidget *) const;
    virtual int indexOf(const QLayoutItem *) const;
    virtual int count() const = 0;
    bool isEmpty() const override;
    QSizePolicy::ControlTypes controlTypes() const override;

    virtual QLayoutItem *replaceWidget(QWidget *from, QWidget *to,
                                       BobUI::FindChildOptions options = BobUI::FindChildrenRecursively);

    int totalMinimumHeightForWidth(int w) const;
    int totalHeightForWidth(int w) const;
    QSize totalMinimumSize() const;
    QSize totalMaximumSize() const;
    QSize totalSizeHint() const;
    QLayout *layout() override;

    void setEnabled(bool);
    bool isEnabled() const;


    static QSize closestAcceptableSize(const QWidget *w, const QSize &s);

protected:
    void widgetEvent(QEvent *);
    void childEvent(QChildEvent *e) override;
    void addChildLayout(QLayout *l);
    void addChildWidget(QWidget *w);
    bool adoptLayout(QLayout *layout);

    QRect alignmentRect(const QRect&) const;
protected:
    QLayout(QLayoutPrivate &d, QLayout*, QWidget*);

private:
    Q_DISABLE_COPY(QLayout)

    static void activateRecursiveHelper(QLayoutItem *item);

    friend class QApplicationPrivate;
    friend class QWidget;

};

BOBUI_END_NAMESPACE

//### support old includes
#include <BobUIWidgets/qboxlayout.h>
#include <BobUIWidgets/qgridlayout.h>

#endif // QLAYOUT_H
