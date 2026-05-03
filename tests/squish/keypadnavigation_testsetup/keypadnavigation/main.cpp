// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QMainWindow>
#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "ui_keypadnavigation.h"

class KeypadNavigation : public QMainWindow
{
    Q_OBJECT

public:
    KeypadNavigation(QWidget *parent = nullptr)
        : QMainWindow(parent)
        , ui(new Ui_KeypadNavigation)
    {
        ui->setupUi(this);

        const struct {
            QAction *action;
            QWidget *page;
        } layoutMappings[] = {
            {ui->m_actionLayoutVerticalSimple,  ui->m_pageVerticalSimple},
            {ui->m_actionLayoutVerticalComplex, ui->m_pageVerticalComplex},
            {ui->m_actionLayoutTwoDimensional,  ui->m_pageTwoDimensional},
            {ui->m_actionLayoutSliderMagic,     ui->m_pageSliderMagic},
            {ui->m_actionLayoutChaos,           ui->m_pageChaos},
            {ui->m_actionLayoutDialogs,         ui->m_pageDialogs}
        };
        for (auto layoutMapping : layoutMappings) {
            const auto page = layoutMapping.page;
            connect(layoutMapping.action, &QAction::triggered, ui->m_stackWidget,
                    [this, page]()
                        { ui->m_stackWidget->setCurrentWidget(page); });
        }

#ifdef BOBUI_KEYPAD_NAVIGATION
        const struct {
            QAction *action;
            BobUI::NavigationMode mode;
        } modeMappings[] = {
            {ui->m_actionModeNone,                  BobUI::NavigationModeNone},
            {ui->m_actionModeKeypadTabOrder,        BobUI::NavigationModeKeypadTabOrder},
            {ui->m_actionModeKeypadDirectional,     BobUI::NavigationModeKeypadDirectional},
            {ui->m_actionModeCursorAuto,            BobUI::NavigationModeCursorAuto},
            {ui->m_actionModeCursorForceVisible,    BobUI::NavigationModeCursorForceVisible}
        };
        for (auto modeMapping : modeMappings) {
            const auto mode = modeMapping.mode;
            connect(modeMapping.action, &QAction::triggered, this,
                    [this, mode]() { setNavigationMode(mode); });
        }
#else // BOBUI_KEYPAD_NAVIGATION
        ui->m_menuNavigation_mode->deleteLater();
#endif // BOBUI_KEYPAD_NAVIGATION

        const struct {
            QPushButton *button;
            Dialog dialog;
        } openDialogMappings[] = {
            {ui->m_buttonGetOpenFileName,       DialogGetOpenFileName},
            {ui->m_buttonGetSaveFileName,       DialogGetSaveFileName},
            {ui->m_buttonGetExistingDirectory,  DialogGetExistingDirectory},
            {ui->m_buttonGetColor,              DialogGetColor},
            {ui->m_buttonGetFont,               DialogGetFont},
            {ui->m_buttonQuestion,              DialogQuestion},
            {ui->m_buttonAboutBobUI,               DialogAboutBobUI},
            {ui->m_buttonGetItem,               DialogGetItem}
        };
        for (auto openDialogMapping : openDialogMappings) {
            const auto dialog = openDialogMapping.dialog;
            connect(openDialogMapping.button, &QPushButton::clicked, this,
                    [this, dialog]() { openDialog(dialog); });
        }
    }

    ~KeypadNavigation()
    {
        delete ui;
    }

protected slots:
#ifdef BOBUI_KEYPAD_NAVIGATION
    void setNavigationMode(int mode)
    {
        QApplication::setNavigationMode(BobUI::NavigationMode(mode));
    }
#endif // BOBUI_KEYPAD_NAVIGATION

    void openDialog(int dialog)
    {
        switch (Dialog(dialog)) {
            case DialogGetOpenFileName:
                QFileDialog::getOpenFileName(this, QLatin1String("getOpenFileName"));
                break;
            case DialogGetSaveFileName:
                QFileDialog::getSaveFileName(this, QLatin1String("getSaveFileName"));
                break;
            case DialogGetExistingDirectory:
                QFileDialog::getExistingDirectory(this, QLatin1String("getExistingDirectory"));
                break;
            case DialogGetColor:
                QColorDialog::getColor(QColor(BobUI::green), this, QLatin1String("getColor"));
                break;
            case DialogGetFont:
                QFontDialog::getFont(0, this);
                break;
            case DialogQuestion:
                QMessageBox::question(this, QLatin1String("question"), QLatin1String("\xbfHola, que tal?"));
                break;
            case DialogAboutBobUI:
                QMessageBox::aboutBobUI(this);
                break;
            case DialogGetItem:
                QInputDialog::getItem(this, QLatin1String("getItem"), QLatin1String("Choose a color"), QColor::colorNames());
                break;
            default:
            break;
        }
    }

private:
    enum Dialog {
        DialogGetOpenFileName,
        DialogGetSaveFileName,
        DialogGetExistingDirectory,
        DialogGetColor,
        DialogGetFont,
        DialogQuestion,
        DialogAboutBobUI,
        DialogGetItem
    };

    Ui_KeypadNavigation *ui;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeypadNavigation w;
    w.show();
    return a.exec();
}

#include "main.moc"
