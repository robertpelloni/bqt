// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/bobuiversion.h>
#include <BobUIGui/qpa/qplatformscreen.h>
#include <BobUIGui/qpa/qplatformwindow.h>
#include <BobUIGui/private/qhighdpiscaling_p.h>
#include <BobUIWidgets/BobUIWidgets>
#include <iostream>

/*
    DprGadget: The focused High-DPI settings debug utility

    DprGadget displays the device pixel ratio ("DPR") for the screen
    it's on in a large font, as well as the inputs (from the platform
    plugin or environment) currently in use for determinging the DPR.

    Non-relevant inputs are not displayed. See bobuitools/src/bobuidiag for
    an utility which displays all inputs.
*/

bool g_bobuiScaleFactor = false;
bool g_bobuiUsePhysicalDpi = false;
bool g_bobuiFontDpi = false;
bool g_bobuiScaleFactorRoundingPolicy = false;
bool g_bobuiHighDpiDownscale = false;
bool g_displayEvents = false;


class DprGadget : public QWidget
{
public:
    std::function<void(void)> m_clearFn;
    std::function<void(void)> m_updateFn;
    qreal m_currentDpr = -1;
    QString m_eventsText;

    DprGadget() {
        setWindowTitle(QString("DprGadget - BobUI %1").arg(qVersion()));

        QFont tinyFont;
        tinyFont.setPointSize(8);
        QFont smallFont;
        smallFont.setPointSize(12);
        QFont bigFont;
        bigFont.setPointSize(42);
        QFont biggerFont;
        biggerFont.setPointSize(80);

        QLabel *dprLabel = new QLabel("Device Pixel Ratio");
        dprLabel->setFont(bigFont);
        dprLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *dprValue = new QLabel();
        dprValue->setFont(biggerFont);
        dprValue->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *screenLabel = new QLabel("Current Screen:");
        screenLabel->setFont(smallFont);

        QLabel *sizeLabel = new QLabel("Window size:");
        sizeLabel->setFont(smallFont);
        sizeLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *nativeSizeLabel = new QLabel("Native:");
        nativeSizeLabel->setFont(smallFont);
        nativeSizeLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *dpiLabel = new QLabel("Logical DPI:");
        dpiLabel->setFont(smallFont);
        dpiLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *windowDpiLabel = new QLabel("Window DPI:");
        windowDpiLabel->setFont(smallFont);
        windowDpiLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *platformDpiLabel = new QLabel("Native Device Pixel Ratio:");
        platformDpiLabel->setFont(smallFont);
        platformDpiLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *windowDprLabel = new QLabel("Window DPR:");
        windowDprLabel->setFont(smallFont);
        windowDprLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *plarformDprLabel = new QLabel("Native DPI:");
        plarformDprLabel->setFont(smallFont);
        plarformDprLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QLabel *bobuiScaleFactorLabel = new QLabel("BobUI Internal Scale Factor:");
        bobuiScaleFactorLabel->setFont(smallFont);

        QLabel *eventsLabel = new QLabel(m_eventsText);
        eventsLabel->setFont(tinyFont);
        eventsLabel->setTextInteractionFlags(BobUI::TextSelectableByMouse);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(dprLabel);
        layout->setAlignment(dprLabel, BobUI::AlignHCenter);
        layout->addWidget(dprValue);
        layout->setAlignment(dprValue, BobUI::AlignHCenter);

        bool displayLogicalDpi = false;
        if (displayLogicalDpi)
            layout->addWidget(dpiLabel);

        layout->addStretch();

        QHBoxLayout *screenLabelLayout = new QHBoxLayout();
        screenLabelLayout->addStretch();
        screenLabelLayout->addWidget(screenLabel);
        screenLabelLayout->addStretch();
        layout->addLayout(screenLabelLayout);

        QHBoxLayout *windowSizeLayout = new QHBoxLayout();
        windowSizeLayout->addWidget(sizeLabel);
        windowSizeLayout->addStretch();
        windowSizeLayout->addWidget(nativeSizeLabel);
        layout->addLayout(windowSizeLayout);

        QHBoxLayout *dpiLayout = new QHBoxLayout();
        dpiLayout->addWidget(windowDpiLabel);
        dpiLayout->addStretch();
        dpiLayout->addWidget(platformDpiLabel);
        layout->addLayout(dpiLayout);

        QHBoxLayout *dprLayout = new QHBoxLayout();
        dprLayout->addWidget(windowDprLabel);
        dprLayout->addStretch();
        dprLayout->addWidget(plarformDprLabel);
        layout->addLayout(dprLayout);

        QHBoxLayout *bobuiScaleFactorLabelLayout = new QHBoxLayout();
        bobuiScaleFactorLabelLayout->addStretch();
        bobuiScaleFactorLabelLayout->addWidget(bobuiScaleFactorLabel);
        bobuiScaleFactorLabelLayout->addStretch();
        layout->addLayout(bobuiScaleFactorLabelLayout);

        if (g_displayEvents)
            layout->addWidget(eventsLabel);

        bool activeEnvironment = g_bobuiScaleFactor || g_bobuiUsePhysicalDpi || g_bobuiFontDpi || g_bobuiScaleFactorRoundingPolicy || g_bobuiHighDpiDownscale;
        if (activeEnvironment) {
            layout->addWidget(new QLabel("Active Environment:"));
            if (g_bobuiScaleFactor) {
                QString text = QString("BOBUI_SCALE_FACTOR=") + qgetenv("BOBUI_SCALE_FACTOR");
                layout->addWidget(new QLabel(text));
            }
            if (g_bobuiUsePhysicalDpi) {
                QString text = QString("BOBUI_USE_PHYSICAL_DPI=") + qgetenv("BOBUI_USE_PHYSICAL_DPI");
                layout->addWidget(new QLabel(text));
            }
            if (g_bobuiFontDpi) {
                QString text = QString("BOBUI_FONT_DPI=") + qgetenv("BOBUI_FONT_DPI");
                layout->addWidget(new QLabel(text));
            }
            if (g_bobuiScaleFactorRoundingPolicy) {
                QString text = QString("BOBUI_SCALE_FACTOR_ROUNDING_POLICY=") + qgetenv("BOBUI_SCALE_FACTOR_ROUNDING_POLICY");
                layout->addWidget(new QLabel(text));
            }
            if (g_bobuiHighDpiDownscale) {
                QString text = QString("BOBUI_WIDGETS_HIGHDPI_DOWNSCALE=") + qgetenv("BOBUI_WIDGETS_HIGHDPI_DOWNSCALE");
                layout->addWidget(new QLabel(text));
            }
        }

        auto updateValues = [=]() {
            dprValue->setText(QString("%1").arg(devicePixelRatioF()));
            windowDpiLabel->setText(QString("QWindow DPI: %1").arg(logicalDpiX()));
            dpiLabel->setText(QString("Logical DPI: %1").arg(logicalDpiX()));
            sizeLabel->setText(QString("QWindow size: %1 %2").arg(width()).arg(height()));

            QPlatformWindow *platformWindow = windowHandle()->handle();
            nativeSizeLabel->setText(QString("native size %1 %2").arg(platformWindow->geometry().width())
                                                            .arg(platformWindow->geometry().height()));
            QPlatformScreen *pscreen = screen()->handle();
            if (g_bobuiUsePhysicalDpi) {
                int physicalDpi = qRound(pscreen->geometry().width() / pscreen->physicalSize().width() * qreal(25.4));
                platformDpiLabel->setText(QString("Native Physical DPI: %1").arg(physicalDpi));
            } else {
                platformDpiLabel->setText(QString("native logical DPI: %1").arg(pscreen->logicalDpi().first));
            }

            windowDprLabel->setText(QString("QWindow DPR: %1").arg(windowHandle()->devicePixelRatio()));
            plarformDprLabel->setText(QString("native DPR: %1").arg(pscreen->devicePixelRatio()));

            screenLabel->setText(QString("Current Screen: %1").arg(screen()->name()));
            bobuiScaleFactorLabel->setText(QString("BobUI Internal Scale Factor: %1").arg(QHighDpiScaling::factor(windowHandle())));
            eventsLabel->setText(QString(m_eventsText));
        };
        m_updateFn = updateValues;

        m_clearFn = [=]() {
            dprValue->setText(QString(""));
            m_eventsText.clear();
        };

        create();

        QObject::connect(this->windowHandle(), &QWindow::screenChanged, [updateValues, this](QScreen *screen){
            Q_UNUSED(screen);
            this->m_eventsText.prepend(QString("ScreenChange "));
            this->m_eventsText.truncate(80);
            updateValues();
        });

        setLayout(layout);

        updateValues();
    }

    void paintEvent(QPaintEvent *) override  {

        // Update the UI in the paint event - normally not good
        // practice but it looks like we can get away with it here
        this->m_eventsText.prepend(QString("Paint "));
        this->m_eventsText.truncate(80);

        // Dpr change should trigger a repaint, update display values here
        if (m_currentDpr == devicePixelRatioF())
            return;
        m_currentDpr = devicePixelRatioF();

        m_updateFn();
    }

    void resizeEvent(QResizeEvent *event) override {
        QSize size = event->size();
        m_eventsText.prepend(QString("Resize(%1 %2) ").arg(size.width()).arg(size.height()));
        m_eventsText.truncate(80);
        m_updateFn();
    }

    void mousePressEvent(QMouseEvent *) override {
        m_clearFn();
        BOBUIimer::singleShot(500, this, [this](){
            m_updateFn();
        });
    }
};

int main(int argc, char **argv) {

    // Set sensible defaults
    QCoreApplication::setAttribute(BobUI::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(BobUI::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Display any set high-dpi eviornment variable.
    g_bobuiScaleFactor = qEnvironmentVariableIsSet("BOBUI_SCALE_FACTOR");
    g_bobuiUsePhysicalDpi = qgetenv("BOBUI_USE_PHYSICAL_DPI") == QByteArray("1");
    g_bobuiFontDpi = qEnvironmentVariableIsSet("BOBUI_FONT_DPI");
    g_bobuiScaleFactorRoundingPolicy = qEnvironmentVariableIsSet("BOBUI_SCALE_FACTOR_ROUNDING_POLICY");
    g_bobuiHighDpiDownscale = qEnvironmentVariableIsSet("BOBUI_WIDGETS_HIGHDPI_DOWNSCALE");

    QApplication app(argc, argv);

    DprGadget dprGadget;
    dprGadget.show();

    return app.exec();
}
