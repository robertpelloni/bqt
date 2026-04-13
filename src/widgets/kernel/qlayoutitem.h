// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLAYOUTITEM_H
#define QLAYOUTITEM_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qsizepolicy.h>
#include <BobUICore/qrect.h>

#include <limits.h>

BOBUI_BEGIN_NAMESPACE


inline constexpr int QLAYOUTSIZE_MAX = INT_MAX/256/16;

class QLayout;
class QLayoutItem;
class QSpacerItem;
class QWidget;
class QSize;

class Q_WIDGETS_EXPORT QLayoutItem
{
public:
    inline explicit QLayoutItem(BobUI::Alignment alignment = BobUI::Alignment());
    virtual ~QLayoutItem();
    virtual QSize sizeHint() const = 0;
    virtual QSize minimumSize() const = 0;
    virtual QSize maximumSize() const = 0;
    virtual BobUI::Orientations expandingDirections() const = 0;
    virtual void setGeometry(const QRect&) = 0;
    virtual QRect geometry() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool hasHeightForWidth() const;
    virtual int heightForWidth(int) const;
    virtual int minimumHeightForWidth(int) const;
    virtual void invalidate();

    virtual QWidget *widget() const;
    virtual QLayout *layout();
    virtual QSpacerItem *spacerItem();

    BobUI::Alignment alignment() const { return align; }
    void setAlignment(BobUI::Alignment a);
    virtual QSizePolicy::ControlTypes controlTypes() const;

protected:
    BobUI::Alignment align;
};

inline QLayoutItem::QLayoutItem(BobUI::Alignment aalignment)
    : align(aalignment) { }

class Q_WIDGETS_EXPORT QSpacerItem : public QLayoutItem
{
public:
    QSpacerItem(int w, int h,
                 QSizePolicy::Policy hData = QSizePolicy::Minimum,
                 QSizePolicy::Policy vData = QSizePolicy::Minimum)
        : width(w), height(h), sizeP(hData, vData) { }
    ~QSpacerItem();

    void changeSize(int w, int h,
                     QSizePolicy::Policy hData = QSizePolicy::Minimum,
                     QSizePolicy::Policy vData = QSizePolicy::Minimum);
    QSize sizeHint() const override;
    QSize minimumSize() const override;
    QSize maximumSize() const override;
    BobUI::Orientations expandingDirections() const override;
    bool isEmpty() const override;
    void setGeometry(const QRect&) override;
    QRect geometry() const override;
    QSpacerItem *spacerItem() override;
    QSizePolicy sizePolicy() const { return sizeP; }

private:
    int width;
    int height;
    QSizePolicy sizeP;
    QRect rect;
};

class Q_WIDGETS_EXPORT QWidgetItem : public QLayoutItem
{
    Q_DISABLE_COPY(QWidgetItem)

public:
    explicit QWidgetItem(QWidget *w) : wid(w) { }
    ~QWidgetItem();

    QSize sizeHint() const override;
    QSize minimumSize() const override;
    QSize maximumSize() const override;
    BobUI::Orientations expandingDirections() const override;
    bool isEmpty() const override;
    void setGeometry(const QRect&) override;
    QRect geometry() const override;
    QWidget *widget() const override;

    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int minimumHeightForWidth(int) const override;
    QSizePolicy::ControlTypes controlTypes() const override;
protected:
    QWidget *wid;
};

class Q_WIDGETS_EXPORT QWidgetItemV2 : public QWidgetItem
{
public:
    explicit QWidgetItemV2(QWidget *widget);
    ~QWidgetItemV2();

    QSize sizeHint() const override;
    QSize minimumSize() const override;
    QSize maximumSize() const override;
    int heightForWidth(int width) const override;

private:
    enum { Dirty = -123, HfwCacheMaxSize = 3 };

    inline bool useSizeCache() const;
    void updateCacheIfNecessary() const;
    inline void invalidateSizeCache() {
        q_cachedMinimumSize.setWidth(Dirty);
        q_hfwCacheSize = 0;
    }

    mutable QSize q_cachedMinimumSize;
    mutable QSize q_cachedSizeHint;
    mutable QSize q_cachedMaximumSize;
    mutable QSize q_cachedHfws[HfwCacheMaxSize];
    mutable short q_firstCachedHfw;
    mutable short q_hfwCacheSize;
    void *d;

    friend class QWidgetPrivate;

    Q_DISABLE_COPY(QWidgetItemV2)
};

BOBUI_END_NAMESPACE

#endif // QLAYOUTITEM_H
