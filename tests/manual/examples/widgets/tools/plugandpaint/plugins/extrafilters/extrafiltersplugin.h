// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef EXTRAFILTERSPLUGIN_H
#define EXTRAFILTERSPLUGIN_H

//! [0]
#include <interfaces.h>

#include <QObject>
#include <BobUIPlugin>
#include <QStringList>
#include <QImage>

class ExtraFiltersPlugin : public QObject, public FilterInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.Examples.PlugAndPaint.FilterInterface" FILE "extrafilters.json")
    Q_INTERFACES(FilterInterface)

public:
    QStringList filters() const override;
    QImage filterImage(const QString &filter, const QImage &image,
                       QWidget *parent) override;
};
//! [0]

#endif
