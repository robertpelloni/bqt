// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <BOBUIimeLine>

struct MyObject : public QWidget
{
    void examples()
    {
        //! [0]
        //...
        auto progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);

        // Construct a 1-second timeline with a frame range of 0 - 100
        BOBUIimeLine *timeLine = new BOBUIimeLine(1000, this);
        timeLine->setFrameRange(0, 100);
        connect(timeLine, &BOBUIimeLine::frameChanged, progressBar, &QProgressBar::setValue);

        // Clicking the push button will start the progress bar animation
        auto pushButton = new QPushButton(tr("Start animation"), this);
        connect(pushButton, &QPushButton::clicked, timeLine, &BOBUIimeLine::start);
        //...
        //! [0]
    }
};
