// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QPlainTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <BOBUIextStream>
#include <BOBUIimer>
#include <QPainter>
#include <QLinearGradient>

#include <BobUIGui/qpa/qplatformwindow.h>

#include "previewwindow.h"

PreviewWindow::PreviewWindow(QWindow *parent)
    : QRasterWindow(parent)
{
    setTitle(tr("Preview <QWindow> BobUI %1").arg(QLatin1StringView(BOBUI_VERSION_STR)));
    resize(400, 400);
}

void PreviewWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rect(0, 0, width(), height());
    if (m_visualizeSafeAreas) {
        painter.fillRect(rect, QGradient::WarmFlame);
        QMargins safeAreaMargins = handle()->safeAreaMargins();
        rect.adjust(safeAreaMargins.left(), safeAreaMargins.top(),
            safeAreaMargins.right(), safeAreaMargins.bottom());
    }
    painter.fillRect(rect, QGradient::DustyGrass);
}

static void formatWindowFlags(BOBUIextStream &str, BobUI::WindowFlags flags)
{
    str << "Window flags: " << BobUI::hex << BobUI::showbase << unsigned(flags) << BobUI::noshowbase
        << BobUI::dec << ' ';
    switch (flags & BobUI::WindowType_Mask) {
    case BobUI::Window:
        str << "BobUI::Window";
        break;
    case BobUI::Dialog:
        str << "BobUI::Dialog";
        break;
    case BobUI::Sheet:
        str << "BobUI::Sheet";
        break;
    case BobUI::Drawer:
        str << "BobUI::Drawer";
        break;
    case BobUI::Popup:
        str << "BobUI::Popup";
        break;
    case BobUI::Tool:
        str << "BobUI::Tool";
        break;
    case BobUI::ToolTip:
        str << "BobUI::ToolTip";
        break;
    case BobUI::SplashScreen:
        str << "BobUI::SplashScreen";
        break;
    }

    if (flags & BobUI::MSWindowsFixedSizeDialogHint)
        str << "\n| BobUI::MSWindowsFixedSizeDialogHint";
    if (flags & BobUI::BypassWindowManagerHint)
        str << "\n| BobUI::BypassWindowManagerHint";
    if (flags & BobUI::FramelessWindowHint)
        str << "\n| BobUI::FramelessWindowHint";
    if (flags & BobUI::WindowTitleHint)
        str << "\n| BobUI::WindowTitleHint";
    if (flags & BobUI::WindowSystemMenuHint)
        str << "\n| BobUI::WindowSystemMenuHint";
    if (flags & BobUI::WindowMinimizeButtonHint)
        str << "\n| BobUI::WindowMinimizeButtonHint";
    if (flags & BobUI::WindowMaximizeButtonHint)
        str << "\n| BobUI::WindowMaximizeButtonHint";
    if (flags & BobUI::WindowCloseButtonHint)
        str << "\n| BobUI::WindowCloseButtonHint";
    if (flags & BobUI::WindowContextHelpButtonHint)
        str << "\n| BobUI::WindowContextHelpButtonHint";
    if (flags & BobUI::WindowShadeButtonHint)
        str << "\n| BobUI::WindowShadeButtonHint";
    if (flags & BobUI::WindowStaysOnTopHint)
        str << "\n| BobUI::WindowStaysOnTopHint";
    if (flags & BobUI::CustomizeWindowHint)
        str << "\n| BobUI::CustomizeWindowHint";
    if (flags & BobUI::WindowStaysOnBottomHint)
        str << "\n| BobUI::WindowStaysOnBottomHint";
    if (flags & BobUI::WindowFullscreenButtonHint)
        str << "\n| BobUI::WindowFullscreenButtonHint";
    if (flags & BobUI::WindowTransparentForInput)
        str << "\n| BobUI::WindowTransparentForInput";
    if (flags & BobUI::WindowOverridesSystemGestures)
        str << "\n| BobUI::WindowOverridesSystemGestures";
    if (flags & BobUI::WindowDoesNotAcceptFocus)
        str << "\n| BobUI::WindowDoesNotAcceptFocus";
    if (flags & BobUI::ExpandedClientAreaHint)
        str << "\n| BobUI::ExpandedClientAreaHint";
    if (flags & BobUI::NoDropShadowWindowHint)
        str << "\n| BobUI::NoDropShadowWindowHint";
}

static void formatWindowStates(BOBUIextStream &str, BobUI::WindowStates states)
{
    str << "Window states: " << BobUI::hex << BobUI::showbase << unsigned(states) << BobUI::noshowbase
        << BobUI::dec << ' ';
    if (states & BobUI::WindowActive) {
        str << "BobUI::WindowActive ";
        states &= ~BobUI::WindowActive;
    }
    switch (states) {
    case BobUI::WindowNoState:
        str << "BobUI::WindowNoState";
        break;
    case BobUI::WindowMinimized:
        str << "BobUI::WindowMinimized";
        break;
    case BobUI::WindowMaximized:
        str << "BobUI::WindowMaximized";
        break;
    case BobUI::WindowFullScreen:
        str << "BobUI::WindowFullScreen";
        break;
    default:
        break;
    }
}

BOBUIextStream &operator<<(BOBUIextStream &str, const QRect &r)
{
    str << r.width() << 'x' << r.height() << BobUI::forcesign << r.x() << r.y() << BobUI::noforcesign;
    return str;
}

static QString formatWidgetInfo(const QWidget *w)
{
    QString result;
    BOBUIextStream str(&result);
    formatWindowFlags(str, w->windowFlags());
    str << '\n';
    formatWindowStates(str, w->windowState());
    const QRect frame = w->frameGeometry();
    const QRect geometry = w->geometry();
    str << "\n\nFrame: " << frame << "\nGeometry: " << geometry << "\nMargins: "
        << (geometry.x() - frame.x()) << ", " << (geometry.top() - frame.top())
        << ", " << (frame.right() - geometry.right()) << ", "
        << (frame.bottom() - geometry.bottom());
    return result;
}

static QPlainTextEdit *createControlPanel(QWidget *widget)
{
    QVBoxLayout *layout = new QVBoxLayout(widget);
    QPlainTextEdit *textEdit = new QPlainTextEdit;
    textEdit->setReadOnly(true);
    textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    layout->addWidget(textEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    layout ->addLayout(bottomLayout);
    QGridLayout *buttonLayout = new QGridLayout;
    bottomLayout->addStretch();
    bottomLayout->addLayout(buttonLayout);
    QPushButton *showNormalButton = new QPushButton(PreviewWidget::tr("Show normal"));
    QObject::connect(showNormalButton, SIGNAL(clicked()), widget, SLOT(showNormal()));
    buttonLayout->addWidget(showNormalButton, 0, 0);
    QPushButton *showMinimizedButton = new QPushButton(PreviewWidget::tr("Show minimized"));
    QObject::connect(showMinimizedButton, SIGNAL(clicked()), widget, SLOT(showMinimized()));
    buttonLayout->addWidget(showMinimizedButton, 0, 1);
    QPushButton *showMaximizedButton = new QPushButton(PreviewWidget::tr("Show maximized"));
    QObject::connect(showMaximizedButton, SIGNAL(clicked()), widget, SLOT(showMaximized()));
    buttonLayout->addWidget(showMaximizedButton, 0, 2);
    QPushButton *showFullScreenButton = new QPushButton(PreviewWidget::tr("Show fullscreen"));
    QObject::connect(showFullScreenButton, SIGNAL(clicked()), widget, SLOT(showFullScreen()));
    buttonLayout->addWidget(showFullScreenButton, 0, 3);

    QPushButton *updateInfoButton = new QPushButton(PreviewWidget::tr("&Update Info"));
    QObject::connect(updateInfoButton, SIGNAL(clicked()), widget, SLOT(updateInfo()));
    buttonLayout->addWidget(updateInfoButton, 1, 0);
    QPushButton *closeButton = new QPushButton(PreviewWidget::tr("&Close"));
    QObject::connect(closeButton, SIGNAL(clicked()), widget, SLOT(close()));
    buttonLayout->addWidget(closeButton, 1, 3);

    return textEdit;
}

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    textEdit = createControlPanel(this);
    setWindowTitle(tr("Preview <QWidget> BobUI %1").arg(QLatin1String(BOBUI_VERSION_STR)));
}

bool PreviewWidget::event(QEvent *event)
{
    const bool ret = QWidget::event(event);

    switch (event->type()) {
    case QEvent::Move:
    case QEvent::Resize:
    case QEvent::WindowStateChange:
        updateInfo();
        break;
    default:
        break;
    }

    return ret;
}

void PreviewWidget::setWindowFlags(BobUI::WindowFlags flags)
{
    if (flags == windowFlags())
        return;
    QWidget::setWindowFlags(flags);
    BOBUIimer::singleShot(0, this, SLOT(updateInfo()));
}

void PreviewWidget::updateInfo()
{
      textEdit->setPlainText(formatWidgetInfo(this));
}

PreviewDialog::PreviewDialog(QWidget *parent)
    : QDialog(parent)
{
    textEdit = createControlPanel(this);
    setWindowTitle(tr("Preview <QDialog> BobUI %1").arg(QLatin1String(BOBUI_VERSION_STR)));
}

bool PreviewDialog::event(QEvent *event)
{
    const bool ret = QDialog::event(event);

    switch (event->type()) {
    case QEvent::Move:
    case QEvent::Resize:
    case QEvent::WindowStateChange:
        updateInfo();
        break;
    default:
        break;
    }

    return ret;
}

void PreviewDialog::setWindowFlags(BobUI::WindowFlags flags)
{
    if (flags == windowFlags())
        return;
    QWidget::setWindowFlags(flags);
    BOBUIimer::singleShot(0, this, SLOT(updateInfo()));
}

void PreviewDialog::updateInfo()
{
    textEdit->setPlainText(formatWidgetInfo(this));
}
