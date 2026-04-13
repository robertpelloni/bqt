// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFRAME_H
#define QFRAME_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_BEGIN_NAMESPACE


class QFramePrivate;
class QStyleOptionFrame;

class Q_WIDGETS_EXPORT QFrame : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(Shape frameShape READ frameShape WRITE setFrameShape)
    Q_PROPERTY(Shadow frameShadow READ frameShadow WRITE setFrameShadow)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth)
    Q_PROPERTY(int midLineWidth READ midLineWidth WRITE setMidLineWidth)
    Q_PROPERTY(int frameWidth READ frameWidth)
    Q_PROPERTY(QRect frameRect READ frameRect WRITE setFrameRect DESIGNABLE false)

public:
    explicit QFrame(QWidget* parent = nullptr, BobUI::WindowFlags f = BobUI::WindowFlags());
    ~QFrame();

    int frameStyle() const;
    void setFrameStyle(int);

    int frameWidth() const;

    QSize sizeHint() const override;

    enum Shape {
        NoFrame  = 0, // no frame
        Box = 0x0001, // rectangular box
        Panel = 0x0002, // rectangular panel
        WinPanel = 0x0003, // rectangular panel (Windows)
        HLine = 0x0004, // horizontal line
        VLine = 0x0005, // vertical line
        StyledPanel = 0x0006 // rectangular panel depending on the GUI style
    };
    Q_ENUM(Shape)
    enum Shadow {
        Plain = 0x0010, // plain line
        Raised = 0x0020, // raised shadow effect
        Sunken = 0x0030 // sunken shadow effect
    };
    Q_ENUM(Shadow)

    enum StyleMask {
        Shadow_Mask = 0x00f0, // mask for the shadow
        Shape_Mask = 0x000f // mask for the shape
    };

    Shape frameShape() const;
    void setFrameShape(Shape);
    Shadow frameShadow() const;
    void setFrameShadow(Shadow);

    int lineWidth() const;
    void setLineWidth(int);

    int midLineWidth() const;
    void setMidLineWidth(int);

    QRect frameRect() const;
    void setFrameRect(const QRect &);

protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *) override;
    void changeEvent(QEvent *) override;
    void drawFrame(QPainter *);


protected:
    QFrame(QFramePrivate &dd, QWidget* parent = nullptr, BobUI::WindowFlags f = BobUI::WindowFlags());
    virtual void initStyleOption(QStyleOptionFrame *option) const;

private:
    Q_DISABLE_COPY(QFrame)
    Q_DECLARE_PRIVATE(QFrame)
};

Q_DECLARE_MIXED_ENUM_OPERATORS_SYMMETRIC(int, QFrame::Shape, QFrame::Shadow)

BOBUI_END_NAMESPACE

#endif // QFRAME_H
