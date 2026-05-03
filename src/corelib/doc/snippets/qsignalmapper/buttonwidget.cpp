// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "buttonwidget.h"

#include <BobUIWidgets>

    //! [OpenCtor]
    ButtonWidget::ButtonWidget(const QStringList &texts, QWidget *parent)
        : QWidget(parent)
    {
    //! [OpenCtor]
    {
        //![OldNotation]
            signalMapper = new QSignalMapper(this);

            QGridLayout *gridLayout = new QGridLayout(this);
            for (int i = 0; i < texts.size(); ++i) {
                QPushButton *button = new QPushButton(texts[i]);
                connect(button, &QPushButton::clicked, signalMapper, qOverload<>(&QSignalMapper::map));
                signalMapper->setMapping(button, texts[i]);
                gridLayout->addWidget(button, i / 3, i % 3);
            }

            connect(signalMapper, &QSignalMapper::mappedString,
                    this, &ButtonWidget::clicked);
        //![OldNotation]
    }

    {
        //![ModernNotation]
            QGridLayout *gridLayout = new QGridLayout(this);
            for (int i = 0; i < texts.size(); ++i) {
                QString text = texts[i];
                QPushButton *button = new QPushButton(text);
                connect(button, &QPushButton::clicked, [this, text] { clicked(text); });
                gridLayout->addWidget(button, i / 3, i % 3);
            }
        //![ModernNotation]
    }

    //! [CloseBrackets]
    }
    //! [CloseBrackets]
