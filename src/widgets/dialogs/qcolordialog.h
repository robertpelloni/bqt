// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOLORDIALOG_H
#define QCOLORDIALOG_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>

#include <BobUIWidgets/qdialog.h>

BOBUI_REQUIRE_CONFIG(colordialog);

BOBUI_BEGIN_NAMESPACE

class QColorDialogPrivate;

class Q_WIDGETS_EXPORT QColorDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QColorDialog)
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor
               NOTIFY currentColorChanged)
    Q_PROPERTY(ColorDialogOptions options READ options WRITE setOptions)

public:
    enum ColorDialogOption {
        ShowAlphaChannel    = 0x00000001,
        NoButtons           = 0x00000002,
        DontUseNativeDialog = 0x00000004,
        NoEyeDropperButton  = 0x00000008,
    };
    Q_ENUM(ColorDialogOption)

    Q_DECLARE_FLAGS(ColorDialogOptions, ColorDialogOption)

    explicit QColorDialog(QWidget *parent = nullptr);
    explicit QColorDialog(const QColor &initial, QWidget *parent = nullptr);
    ~QColorDialog();

    void setCurrentColor(const QColor &color);
    QColor currentColor() const;

    QColor selectedColor() const;

    void setOption(ColorDialogOption option, bool on = true);
    bool testOption(ColorDialogOption option) const;
    void setOptions(ColorDialogOptions options);
    ColorDialogOptions options() const;

    using QDialog::open;
    void open(QObject *receiver, const char *member);

    void setVisible(bool visible) override;

    static QColor getColor(const QColor &initial = BobUI::white,
                           QWidget *parent = nullptr,
                           const QString &title = QString(),
                           ColorDialogOptions options = ColorDialogOptions());

    static int customCount();
    static QColor customColor(int index);
    static void setCustomColor(int index, QColor color);
    static QColor standardColor(int index);
    static void setStandardColor(int index, QColor color);

Q_SIGNALS:
    void currentColorChanged(const QColor &color);
    void colorSelected(const QColor &color);

protected:
    void changeEvent(QEvent *event) override;
    void done(int result) override;

private:
    Q_DISABLE_COPY(QColorDialog)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QColorDialog::ColorDialogOptions)

BOBUI_END_NAMESPACE

#endif // QCOLORDIALOG_H
