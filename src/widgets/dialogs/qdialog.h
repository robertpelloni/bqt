// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDIALOG_H
#define QDIALOG_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(dialog);

BOBUI_BEGIN_NAMESPACE


class QPushButton;
class QDialogPrivate;

class Q_WIDGETS_EXPORT QDialog : public QWidget
{
    Q_OBJECT
    friend class QPushButton;

    Q_PROPERTY(bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled)
    Q_PROPERTY(bool modal READ isModal WRITE setModal)

public:
    explicit QDialog(QWidget *parent = nullptr, BobUI::WindowFlags f = BobUI::WindowFlags());
    ~QDialog();

    enum DialogCode { Rejected, Accepted };
    Q_ENUM(DialogCode)

    int result() const;

    void setVisible(bool visible) override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setSizeGripEnabled(bool);
    bool isSizeGripEnabled() const;

    void setModal(bool modal);
    void setResult(int r);

Q_SIGNALS:
    void finished(int result);
    void accepted();
    void rejected();

public Q_SLOTS:
    virtual void open();
    virtual int exec();
    virtual void done(int);
    virtual void accept();
    virtual void reject();

protected:
    QDialog(QDialogPrivate &, QWidget *parent, BobUI::WindowFlags f = BobUI::WindowFlags());

    void keyPressEvent(QKeyEvent *) override;
    void closeEvent(QCloseEvent *) override;
    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *) override;
#endif
    bool eventFilter(QObject *, QEvent *) override;
    void adjustPosition(QWidget*);
private:
    Q_DECLARE_PRIVATE(QDialog)
    Q_DISABLE_COPY(QDialog)
};

BOBUI_END_NAMESPACE

#endif // QDIALOG_H
