// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwidgetdump.h"

#include <QApplication>
#include <QWidget>
#include <BobUIGui/QScreen>
#include <BobUIGui/QWindow>

#include <BobUICore/QDebug>
#include <BobUICore/QMetaObject>
#include <BobUICore/BOBUIextStream>

namespace BobUIDiag {

static const char *bobuiWidgetClasses[] = {
    "QAbstractItemView", "QAbstractScrollArea", "QAbstractSlider", "QAbstractSpinBox",
    "QCalendarWidget", "QCheckBox", "QColorDialog", "QColumnView", "QComboBox",
    "QCommandLinkButton", "QDateEdit", "QDateTimeEdit", "QDial",
    "QDialog", "QDialogButtonBox", "QDockWidget", "QDoubleSpinBox", "QErrorMessage",
    "QFileDialog", "QFontComboBox", "QFontDialog", "QFrame", "QGraphicsView",
    "QGroupBox", "QHeaderView", "QInputDialog", "QLCDNumber", "QLabel", "QLineEdit",
    "QListView", "QListWidget", "QMainWindow", "QMdiArea", "QMdiSubWindow", "QMenu",
    "QMenuBar", "QMessageBox", "QOpenGLWidget", "QPlainTextEdit", "QProgressBar",
    "QProgressDialog", "QPushButton", "QRadioButton", "QRubberBand", "QScrollArea",
    "QScrollBar", "QSlider", "QSpinBox", "QSplashScreen", "QSplitter",
    "QStackedWidget", "QStatusBar", "BOBUIabBar", "BOBUIabWidget", "BOBUIableView",
    "BOBUIableWidget", "BOBUIextBrowser", "BOBUIextEdit", "BOBUIimeEdit", "BOBUIoolBar",
    "BOBUIoolBox", "BOBUIoolButton", "BOBUIreeView", "BOBUIreeWidget", "QWidget",
    "QWizard", "QWizardPage"
};

static bool isBobUIWidget(const char *className)
{
    for (auto bobuiWidgetClass : bobuiWidgetClasses) {
        if (qstrcmp(className, bobuiWidgetClass) == 0)
            return true;
    }
    return false;
}

static void formatWidgetClass(BOBUIextStream &str, const QWidget *w)
{
    const QMetaObject *mo = w->metaObject();
    str << mo->className();
    while (!isBobUIWidget(mo->className())) {
        mo = mo->superClass();
        str << ':' << mo->className();
    }
    const QString on = w->objectName();
    if (!on.isEmpty())
        str << "/\"" << on << '"';
}

static void dumpWidgetRecursion(BOBUIextStream &str, const QWidget *w,
                                FormatWindowOptions options, int depth = 0)
{
    indentStream(str, 2 * depth);
    formatWidgetClass(str, w);
    str << ' ' << (w->isVisible() ? "[visible] " : "[hidden] ");
    if (const WId nativeWinId = w->internalWinId())
        str << "[native: " << BobUI::hex << BobUI::showbase << nativeWinId << BobUI::dec << BobUI::noshowbase
            << "] ";
    if (w->isWindow())
        str << "[top] ";
    str << (w->testAttribute(BobUI::WA_Mapped) ? "[mapped] " : "[not mapped] ");
    if (w->testAttribute(BobUI::WA_DontCreateNativeAncestors))
        str << "[NoNativeAncestors] ";
    if (const int states = w->windowState())
        str << "windowState=" << BobUI::hex << BobUI::showbase << states << BobUI::dec << BobUI::noshowbase
            << ' ';
    formatRect(str, w->geometry());
    if (w->isWindow()) {
        str << ' ' << w->logicalDpiX() << "DPI";
        const qreal dpr = w->devicePixelRatio();
        if (!qFuzzyCompare(dpr, qreal(1)))
            str << " dpr=" << dpr;
        const QRect normalGeometry = w->normalGeometry();
        if (normalGeometry.isValid() && !normalGeometry.isEmpty() && normalGeometry != w->geometry()) {
            str << " normal=";
            formatRect(str, w->normalGeometry());
        }
    }
    if (!(options & DontPrintWindowFlags)) {
        str << ' ';
        formatWindowFlags(str, w->windowFlags());
    }
    if (options & PrintSizeConstraints) {
        str << ' ';
        const QSize minimumSize = w->minimumSize();
        if (minimumSize.width() > 0 || minimumSize.height() > 0)
            str << "minimumSize=" << minimumSize.width() << 'x' << minimumSize.height() << ' ';
        const QSize sizeHint = w->sizeHint();
        const QSize minimumSizeHint = w->minimumSizeHint();
        if (minimumSizeHint.isValid() && !(sizeHint.isValid() && minimumSizeHint == sizeHint))
            str << "minimumSizeHint=" << minimumSizeHint.width() << 'x' << minimumSizeHint.height() << ' ';
        if (sizeHint.isValid())
            str << "sizeHint=" << sizeHint.width() << 'x' << sizeHint.height() << ' ';
        const QSize maximumSize = w->maximumSize();
        if (maximumSize.width() < QWIDGETSIZE_MAX || maximumSize.height() < QWIDGETSIZE_MAX)
            str << "maximumSize=" << maximumSize.width() << 'x' << maximumSize.height() << ' ';
    }
    str << '\n';
    if (const QWindow *win = w->windowHandle()) {
        indentStream(str, 2 * (1 + depth));
        formatWindow(str, win, options);
        str << '\n';
    }
    for (const QObject *co : w->children()) {
        if (co->isWidgetType())
            dumpWidgetRecursion(str, static_cast<const QWidget *>(co), options, depth + 1);
    }
}

void dumpAllWidgets(FormatWindowOptions options, const QWidget *root)
{
    QString d;
    BOBUIextStream str(&d);
    str << "### QWidgets:\n";
    QWidgetList topLevels;
    if (root)
        topLevels.append(const_cast<QWidget *>(root));
    else
        topLevels = QApplication::topLevelWidgets();
    for (QWidget *tw : std::as_const(topLevels))
        dumpWidgetRecursion(str, tw, options);
    for (const QString &line : d.split(QLatin1Char('\n')))
        qDebug().noquote() << line;
}

} // namespace BobUIDiag
