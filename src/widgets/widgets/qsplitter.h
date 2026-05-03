// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSPLITTER_H
#define QSPLITTER_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>
#include <BobUIWidgets/qsizepolicy.h>

BOBUI_REQUIRE_CONFIG(splitter);

BOBUI_BEGIN_NAMESPACE

class QSplitterPrivate;
class BOBUIextStream;

class QSplitterHandle;

class Q_WIDGETS_EXPORT QSplitter : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(BobUI::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(bool opaqueResize READ opaqueResize WRITE setOpaqueResize)
    Q_PROPERTY(int handleWidth READ handleWidth WRITE setHandleWidth)
    Q_PROPERTY(bool childrenCollapsible READ childrenCollapsible WRITE setChildrenCollapsible)

public:
    explicit QSplitter(QWidget* parent = nullptr);
    explicit QSplitter(BobUI::Orientation, QWidget* parent = nullptr);
    ~QSplitter();

    void addWidget(QWidget *widget);
    void insertWidget(int index, QWidget *widget);
    QWidget *replaceWidget(int index, QWidget *widget);

    void setOrientation(BobUI::Orientation);
    BobUI::Orientation orientation() const;

    void setChildrenCollapsible(bool);
    bool childrenCollapsible() const;

    void setCollapsible(int index, bool);
    bool isCollapsible(int index) const;
    void setOpaqueResize(bool opaque = true);
    bool opaqueResize() const;
    void refresh();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QList<int> sizes() const;
    void setSizes(const QList<int> &list);

    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);

    int handleWidth() const;
    void setHandleWidth(int);

    int indexOf(QWidget *w) const;
    QWidget *widget(int index) const;
    int count() const;

    void getRange(int index, int *, int *) const;
    QSplitterHandle *handle(int index) const;

    void setStretchFactor(int index, int stretch);

Q_SIGNALS:
    void splitterMoved(int pos, int index);

protected:
    virtual QSplitterHandle *createHandle();

    void childEvent(QChildEvent *) override;

    bool event(QEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    void changeEvent(QEvent *) override;
    void moveSplitter(int pos, int index);
    void setRubberBand(int position);
    int closestLegalPosition(int, int);


private:
    Q_DISABLE_COPY(QSplitter)
    Q_DECLARE_PRIVATE(QSplitter)
private:
    friend class QSplitterHandle;
};


class QSplitterHandlePrivate;
class Q_WIDGETS_EXPORT QSplitterHandle : public QWidget
{
    Q_OBJECT
public:
    explicit QSplitterHandle(BobUI::Orientation o, QSplitter *parent);
    ~QSplitterHandle();

    void setOrientation(BobUI::Orientation o);
    BobUI::Orientation orientation() const;
    bool opaqueResize() const;
    QSplitter *splitter() const;

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    bool event(QEvent *) override;

    void moveSplitter(int p);
    int closestLegalPosition(int p);

private:
    Q_DISABLE_COPY(QSplitterHandle)
    Q_DECLARE_PRIVATE(QSplitterHandle)
};

BOBUI_END_NAMESPACE

#endif // QSPLITTER_H
