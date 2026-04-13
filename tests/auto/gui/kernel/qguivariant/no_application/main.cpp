// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <qvariant.h>



class tst_QGuiVariantNoApplication : public QObject
{
    Q_OBJECT

public:
    tst_QGuiVariantNoApplication();

private slots:
    void variantWithoutApplication();
};

tst_QGuiVariantNoApplication::tst_QGuiVariantNoApplication()
{}

void tst_QGuiVariantNoApplication::variantWithoutApplication()
{
    QVariant v = QString("red");

    QCOMPARE(qvariant_cast<QColor>(v), QColor(BobUI::red));
}


BOBUIEST_APPLESS_MAIN(tst_QGuiVariantNoApplication)
#include "main.moc"
