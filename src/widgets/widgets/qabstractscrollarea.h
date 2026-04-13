// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTSCROLLAREA_H
#define QABSTRACTSCROLLAREA_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>

BOBUI_BEGIN_NAMESPACE


#if BOBUI_CONFIG(scrollarea)

class QMargins;
class QScrollBar;
class QAbstractScrollAreaPrivate;

class Q_WIDGETS_EXPORT QAbstractScrollArea : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(BobUI::ScrollBarPolicy verticalScrollBarPolicy READ verticalScrollBarPolicy
               WRITE setVerticalScrollBarPolicy)
    Q_PROPERTY(BobUI::ScrollBarPolicy horizontalScrollBarPolicy READ horizontalScrollBarPolicy
               WRITE setHorizontalScrollBarPolicy)
    Q_PROPERTY(SizeAdjustPolicy sizeAdjustPolicy READ sizeAdjustPolicy WRITE setSizeAdjustPolicy)

public:
    explicit QAbstractScrollArea(QWidget *parent = nullptr);
    ~QAbstractScrollArea();

    enum SizeAdjustPolicy {
        AdjustIgnored,
        AdjustToContentsOnFirstShow,
        AdjustToContents
    };
    Q_ENUM(SizeAdjustPolicy)

    BobUI::ScrollBarPolicy verticalScrollBarPolicy() const;
    void setVerticalScrollBarPolicy(BobUI::ScrollBarPolicy);
    QScrollBar *verticalScrollBar() const;
    void setVerticalScrollBar(QScrollBar *scrollbar);

    BobUI::ScrollBarPolicy horizontalScrollBarPolicy() const;
    void setHorizontalScrollBarPolicy(BobUI::ScrollBarPolicy);
    QScrollBar *horizontalScrollBar() const;
    void setHorizontalScrollBar(QScrollBar *scrollbar);

    QWidget *cornerWidget() const;
    void setCornerWidget(QWidget *widget);

    void addScrollBarWidget(QWidget *widget, BobUI::Alignment alignment);
    QWidgetList scrollBarWidgets(BobUI::Alignment alignment);

    QWidget *viewport() const;
    void setViewport(QWidget *widget);
    QSize maximumViewportSize() const;

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    virtual void setupViewport(QWidget *viewport);

    SizeAdjustPolicy sizeAdjustPolicy() const;
    void setSizeAdjustPolicy(SizeAdjustPolicy policy);

protected:
    QAbstractScrollArea(QAbstractScrollAreaPrivate &dd, QWidget *parent = nullptr);
    void setViewportMargins(int left, int top, int right, int bottom);
    void setViewportMargins(const QMargins &margins);
    QMargins viewportMargins() const;

    bool eventFilter(QObject *, QEvent *) override;
    bool event(QEvent *) override;
    virtual bool viewportEvent(QEvent *);

    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
#if BOBUI_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *) override;
#endif
#if BOBUI_CONFIG(draganddrop)
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dropEvent(QDropEvent *) override;
#endif

    void keyPressEvent(QKeyEvent *) override;

    virtual void scrollContentsBy(int dx, int dy);

    virtual QSize viewportSizeHint() const;

private:
    Q_DECLARE_PRIVATE(QAbstractScrollArea)
    Q_DISABLE_COPY(QAbstractScrollArea)
    Q_PRIVATE_SLOT(d_func(), void _q_hslide(int))
    Q_PRIVATE_SLOT(d_func(), void _q_vslide(int))
    Q_PRIVATE_SLOT(d_func(), void _q_showOrHideScrollBars())

    friend class QStyleSheetStyle;
    friend class QWidgetPrivate;
};

#endif // BOBUI_CONFIG(scrollarea)

BOBUI_END_NAMESPACE

#endif // QABSTRACTSCROLLAREA_H
