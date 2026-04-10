// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <BobUIWidgets/qmainwindow.h>
#include <BobUIWidgets/qlabel.h>
#include <BobUICore/qmetaobject.h>

#include <BobUINetwork/qnetworkinformation.h>

template<typename QEnum>
QString enumToString(const QEnum value)
{
    return QString::fromUtf8(QMetaEnum::fromType<QEnum>().valueToKey(int(value)));
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    using Reachability = QNetworkInformation::Reachability;
    using TransportMedium = QNetworkInformation::TransportMedium;

public:
    MainWindow() : QMainWindow(nullptr)
    {
        label->setText("hello");
        setCentralWidget(label);
    }

public slots:
    void updateReachability(Reachability newValue)
    {
        reachability = newValue;
        updateText();
    }

    void updateCaptiveState(bool newValue)
    {
        captive = newValue;
        updateText();
    }

    void updateTransportMedium(TransportMedium newValue)
    {
        transportMedium = newValue;
        updateText();
    }

    void updateMetered(bool newValue)
    {
        metered = newValue;
        updateText();
    }

private:
    void updateText()
    {
        QString str =
                QLatin1String("Reachability: %1\nBehind captive portal: %2\nTransport medium: %3"
                              "\nMetered: %4")
                        .arg(enumToString(reachability), captive ? u"true" : u"false",
                             enumToString(transportMedium), metered ? u"true" : u"false");
        label->setText(str);
    }

    QLabel *const label = new QLabel(this);
    Reachability reachability = Reachability::Unknown;
    TransportMedium transportMedium = TransportMedium::Unknown;
    bool captive = false;
    bool metered = false;
};

#endif
