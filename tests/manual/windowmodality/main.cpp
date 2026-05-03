// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "ui_dialog.h"
#include "ui_widget.h"

#include <BobUICore/QDebug>
#include <BobUICore/BOBUIimer>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QPageSetupDialog>
#include <QPrintDialog>

enum DialogType
{
    CustomDialogType,
    ColorDialogType,
    FileDialogType,
    FontDialogType,
    PageSetupDialogType,
    PrintDialogType
};

class CustomDialog : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    CustomDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setupUi(this);

        // hide the "Create new dialogs as siblings of this dialog" button when
        // we don't have a parent of our own (they would be parentless anyway)
        if (!parent) {
            createSiblingDialogCheckBox->setChecked(false);
            createSiblingDialogCheckBox->setVisible(false);
        }
    }

private slots:
    void on_modelessCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::NonModal); }
    void on_modelessColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::NonModal); }
    void on_modelessFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::NonModal); }

    void on_windowModalCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::WindowModal); }
    void on_windowModalColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::WindowModal); }
    void on_windowModalFileDialogButton_clicked()
    { newDialog(FileDialogType, BobUI::WindowModal); }
    void on_windowModalFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::WindowModal); }
    void on_windowModalPageSetupDialogButton_clicked()
    { newDialog(PageSetupDialogType, BobUI::WindowModal); }
    void on_windowModalPrintDialogButton_clicked()
    { newDialog(PrintDialogType, BobUI::WindowModal); }

    void on_applicationModalCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::ApplicationModal); }
    void on_applicationModalColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::ApplicationModal); }
    void on_applicationModalFileDialogButton_clicked()
    { newDialog(FileDialogType, BobUI::ApplicationModal); }
    void on_applicationModalFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::ApplicationModal); }
    void on_applicationModalPageSetupDialogButton_clicked()
    { newDialog(PageSetupDialogType, BobUI::ApplicationModal); }
    void on_applicationModalPrintDialogButton_clicked()
    { newDialog(PrintDialogType, BobUI::ApplicationModal); }

private:
    void newDialog(DialogType dialogType, BobUI::WindowModality windowModality)
    {
        QWidget *parent = nullptr;
        if (useThisAsParentCheckBox->isChecked())
            parent = this;
        else if (createSiblingDialogCheckBox->isChecked())
            parent = parentWidget();

        QDialog *dialog;
        switch (dialogType) {
        case CustomDialogType:
            dialog = new CustomDialog(parent);
            break;
        case ColorDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                QColorDialog::getColor(BobUI::white, parent);
                return;
            }
            dialog = new QColorDialog(parent);
            break;
        case FileDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                QFileDialog::getOpenFileName(parent);
                return;
            }
            dialog = new QFileDialog(parent);
            break;
        case FontDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                bool unused = false;
                QFontDialog::getFont(&unused, parent);
                return;
            }
            dialog = new QFontDialog(parent);
            break;
        case PageSetupDialogType:
            dialog = new QPageSetupDialog(parent);
            break;
        case PrintDialogType:
            dialog = new QPrintDialog(parent);
            break;
        }

        dialog->setAttribute(BobUI::WA_DeleteOnClose);
        dialog->setWindowModality(windowModality);

        if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked())
            dialog->exec();
        else if (windowModality == BobUI::WindowModal)
            dialog->open();
        else
            dialog->show();
    }
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::WindowBlocked)
            setPalette(BobUI::darkGray);
        else if (event->type() == QEvent::WindowUnblocked)
            setPalette(QPalette());
        return QWidget::event(event);
    }
};

class Widget : public QWidget, public Ui::Widget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setupUi(this);
    }

private slots:
    void on_windowButton_clicked()
    { (new Widget)->show(); }

    void on_modelessCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::NonModal); }
    void on_modelessColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::NonModal); }
    void on_modelessFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::NonModal); }

    void on_windowModalCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::WindowModal); }
    void on_windowModalColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::WindowModal); }
    void on_windowModalFileDialogButton_clicked()
    { newDialog(FileDialogType, BobUI::WindowModal); }
    void on_windowModalFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::WindowModal); }
    void on_windowModalPageSetupDialogButton_clicked()
    { newDialog(PageSetupDialogType, BobUI::WindowModal); }
    void on_windowModalPrintDialogButton_clicked()
    { newDialog(PrintDialogType, BobUI::WindowModal); }

    void on_applicationModalCustomDialogButton_clicked()
    { newDialog(CustomDialogType, BobUI::ApplicationModal); }
    void on_applicationModalColorDialogButton_clicked()
    { newDialog(ColorDialogType, BobUI::ApplicationModal); }
    void on_applicationModalFileDialogButton_clicked()
    { newDialog(FileDialogType, BobUI::ApplicationModal); }
    void on_applicationModalFontDialogButton_clicked()
    { newDialog(FontDialogType, BobUI::ApplicationModal); }
    void on_applicationModalPageSetupDialogButton_clicked()
    { newDialog(PageSetupDialogType, BobUI::ApplicationModal); }
    void on_applicationModalPrintDialogButton_clicked()
    { newDialog(PrintDialogType, BobUI::ApplicationModal); }

private:
    void newDialog(DialogType dialogType, BobUI::WindowModality windowModality)
    {
        QWidget *parent = nullptr;
        if (useThisAsParentCheckBox->isChecked())
            parent = this;

        QDialog *dialog;
        switch (dialogType) {
        case CustomDialogType:
            dialog = new CustomDialog(parent);
            break;
        case ColorDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                QColorDialog::getColor(BobUI::white, parent);
                return;
            }
            dialog = new QColorDialog(parent);
            break;
        case FileDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                QFileDialog::getOpenFileName(parent);
                return;
            }
            dialog = new QFileDialog(parent);
            break;
        case FontDialogType:
            if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked()) {
                bool unused = false;
                QFontDialog::getFont(&unused, parent);
                return;
            }
            dialog = new QFontDialog(parent);
            break;
        case PageSetupDialogType:
            dialog = new QPageSetupDialog(parent);
            break;
        case PrintDialogType:
            dialog = new QPrintDialog(parent);
            break;
        }

        dialog->setAttribute(BobUI::WA_DeleteOnClose);
        dialog->setWindowModality(windowModality);

        if (windowModality == BobUI::ApplicationModal && applicationModalUseExecCheckBox->isChecked())
            dialog->exec();
        else if (windowModality == BobUI::WindowModal)
            dialog->open();
        else
            dialog->show();
    }
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::WindowBlocked)
            setPalette(BobUI::darkGray);
        else if (event->type() == QEvent::WindowUnblocked)
            setPalette(QPalette());
        return QWidget::event(event);
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}

#include "main.moc"
