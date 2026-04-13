// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCROLLBAR_H
#define QSCROLLBAR_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

#include <BobUIWidgets/qabstractslider.h>

BOBUI_REQUIRE_CONFIG(scrollbar);

BOBUI_BEGIN_NAMESPACE

class QScrollBarPrivate;
class QStyleOptionSlider;

class Q_WIDGETS_EXPORT QScrollBar : public QAbstractSlider
{
    Q_OBJECT
public:
    explicit QScrollBar(QWidget *parent = nullptr);
    explicit QScrollBar(BobUI::Orientation, QWidget *parent = nullptr);
    ~QScrollBar();

    QSize sizeHint() const override;
    bool event(QEvent *event) override;

#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *createStandardContextMenu(QPoint position);
#endif

protected:
#if BOBUI_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void hideEvent(QHideEvent*) override;
    void sliderChange(SliderChange change) override;
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *) override;
#endif
    virtual void initStyleOption(QStyleOptionSlider *option) const;


private:
    friend class QAbstractScrollAreaPrivate;
    friend Q_WIDGETS_EXPORT QStyleOptionSlider bobui_qscrollbarStyleOption(QScrollBar *scrollBar);

    Q_DISABLE_COPY(QScrollBar)
    Q_DECLARE_PRIVATE(QScrollBar)
#if BOBUI_CONFIG(itemviews)
    friend class BOBUIableView;
    friend class BOBUIreeViewPrivate;
    friend class QCommonListViewBase;
    friend class QListModeViewBase;
    friend class QAbstractItemView;
#endif
};

BOBUI_END_NAMESPACE

#endif // QSCROLLBAR_H
