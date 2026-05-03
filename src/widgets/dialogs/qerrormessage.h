// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QERRORMESSAGE_H
#define QERRORMESSAGE_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>

#include <BobUIWidgets/qdialog.h>

BOBUI_REQUIRE_CONFIG(errormessage);

BOBUI_BEGIN_NAMESPACE

class QErrorMessagePrivate;

class Q_WIDGETS_EXPORT QErrorMessage: public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QErrorMessage)
public:
    explicit QErrorMessage(QWidget* parent = nullptr);
    ~QErrorMessage();

    static QErrorMessage * bobuiHandler();

public Q_SLOTS:
    void showMessage(const QString &message);
    void showMessage(const QString &message, const QString &type);

protected:
    void done(int) override;
    void changeEvent(QEvent *e) override;

private:
    Q_DISABLE_COPY(QErrorMessage)
};

BOBUI_END_NAMESPACE

#endif // QERRORMESSAGE_H
