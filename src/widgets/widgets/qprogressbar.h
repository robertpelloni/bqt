// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPROGRESSBAR_H
#define QPROGRESSBAR_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>

BOBUI_REQUIRE_CONFIG(progressbar);

BOBUI_BEGIN_NAMESPACE

class QProgressBarPrivate;
class QStyleOptionProgressBar;

class Q_WIDGETS_EXPORT QProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(BobUI::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)
    Q_PROPERTY(BobUI::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(bool invertedAppearance READ invertedAppearance WRITE setInvertedAppearance)
    Q_PROPERTY(Direction textDirection READ textDirection WRITE setTextDirection)
    Q_PROPERTY(QString format READ format WRITE setFormat RESET resetFormat)

public:
    enum Direction { TopToBottom, BottomToTop };
    Q_ENUM(Direction)

    explicit QProgressBar(QWidget *parent = nullptr);
    ~QProgressBar();

    int minimum() const;
    int maximum() const;

    int value() const;

    virtual QString text() const;
    void setTextVisible(bool visible);
    bool isTextVisible() const;

    BobUI::Alignment alignment() const;
    void setAlignment(BobUI::Alignment alignment);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    BobUI::Orientation orientation() const;

    void setInvertedAppearance(bool invert);
    bool invertedAppearance() const;
    void setTextDirection(QProgressBar::Direction textDirection);
    QProgressBar::Direction textDirection() const;

    void setFormat(const QString &format);
    void resetFormat();
    QString format() const;

public Q_SLOTS:
    void reset();
    void setRange(int minimum, int maximum);
    void setMinimum(int minimum);
    void setMaximum(int maximum);
    void setValue(int value);
    void setOrientation(BobUI::Orientation);

Q_SIGNALS:
    void valueChanged(int value);

protected:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *) override;
    virtual void initStyleOption(QStyleOptionProgressBar *option) const;

private:
    Q_DECLARE_PRIVATE(QProgressBar)
    Q_DISABLE_COPY(QProgressBar)
};

BOBUI_END_NAMESPACE

#endif // QPROGRESSBAR_H
