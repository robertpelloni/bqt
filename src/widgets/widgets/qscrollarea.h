// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCROLLAREA_H
#define QSCROLLAREA_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractscrollarea.h>

BOBUI_REQUIRE_CONFIG(scrollarea);

BOBUI_BEGIN_NAMESPACE

class QScrollAreaPrivate;

class Q_WIDGETS_EXPORT QScrollArea : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable)
    Q_PROPERTY(BobUI::Alignment alignment READ alignment WRITE setAlignment)

public:
    explicit QScrollArea(QWidget *parent = nullptr);
    ~QScrollArea();

    QWidget *widget() const;
    void setWidget(QWidget *widget);
    QWidget *takeWidget();

    bool widgetResizable() const;
    void setWidgetResizable(bool resizable);

    QSize sizeHint() const override;

    bool focusNextPrevChild(bool next) override;

    BobUI::Alignment alignment() const;
    void setAlignment(BobUI::Alignment);

    void ensureVisible(int x, int y, int xmargin = 50, int ymargin = 50);
    void ensureWidgetVisible(QWidget *childWidget, int xmargin = 50, int ymargin = 50);

protected:
    QScrollArea(QScrollAreaPrivate &dd, QWidget *parent = nullptr);
    bool event(QEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void scrollContentsBy(int dx, int dy) override;

    QSize viewportSizeHint() const override;

private:
    Q_DECLARE_PRIVATE(QScrollArea)
    Q_DISABLE_COPY(QScrollArea)
};

BOBUI_END_NAMESPACE

#endif // QSCROLLAREA_H
