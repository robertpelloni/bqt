// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRIDLAYOUT_H
#define QGRIDLAYOUT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qlayout.h>
#ifdef BOBUI_INCLUDE_COMPAT
#include <BobUIWidgets/qwidget.h>
#endif

#include <limits.h>

BOBUI_BEGIN_NAMESPACE


class QGridLayoutPrivate;

class Q_WIDGETS_EXPORT QGridLayout : public QLayout
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QGridLayout)
    QDOC_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
    QDOC_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)

public:
    explicit QGridLayout(QWidget *parent = nullptr);
    ~QGridLayout();

    QSize sizeHint() const override;
    QSize minimumSize() const override;
    QSize maximumSize() const override;

    void setHorizontalSpacing(int spacing);
    int horizontalSpacing() const;
    void setVerticalSpacing(int spacing);
    int verticalSpacing() const;
    void setSpacing(int spacing) override;
    int spacing() const override;

    void setRowStretch(int row, int stretch);
    void setColumnStretch(int column, int stretch);
    int rowStretch(int row) const;
    int columnStretch(int column) const;

    void setRowMinimumHeight(int row, int minSize);
    void setColumnMinimumWidth(int column, int minSize);
    int rowMinimumHeight(int row) const;
    int columnMinimumWidth(int column) const;

    int columnCount() const;
    int rowCount() const;

    QRect cellRect(int row, int column) const;

    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int minimumHeightForWidth(int) const override;

    BobUI::Orientations expandingDirections() const override;
    void invalidate() override;

    inline void addWidget(QWidget *w) { QLayout::addWidget(w); }
    void addWidget(QWidget *, int row, int column, BobUI::Alignment = BobUI::Alignment());
    void addWidget(QWidget *, int row, int column, int rowSpan, int columnSpan, BobUI::Alignment = BobUI::Alignment());
    void addLayout(QLayout *, int row, int column, BobUI::Alignment = BobUI::Alignment());
    void addLayout(QLayout *, int row, int column, int rowSpan, int columnSpan, BobUI::Alignment = BobUI::Alignment());

    void setOriginCorner(BobUI::Corner);
    BobUI::Corner originCorner() const;

    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *itemAtPosition(int row, int column) const;
    QLayoutItem *takeAt(int index) override;
    int count() const override;
    void setGeometry(const QRect&) override;

    void addItem(QLayoutItem *item, int row, int column, int rowSpan = 1, int columnSpan = 1, BobUI::Alignment = BobUI::Alignment());

    void setDefaultPositioning(int n, BobUI::Orientation orient);
    void getItemPosition(int idx, int *row, int *column, int *rowSpan, int *columnSpan) const;

protected:
    void addItem(QLayoutItem *) override;

private:
    Q_DISABLE_COPY(QGridLayout)

};

BOBUI_END_NAMESPACE

#endif // QGRIDLAYOUT_H
