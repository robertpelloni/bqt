// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WIDGETGALLERY_H
#define WIDGETGALLERY_H

#include <QDialog>

BOBUI_BEGIN_NAMESPACE
class QGroupBox;
class QProgressBar;
class BOBUIabWidget;
class BOBUIextBrowser;
class BOBUIoolBox;
BOBUI_END_NAMESPACE

class WidgetGallery : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetGallery(QWidget *parent = nullptr);

    void setVisible(bool visible) override;

private slots:
    void changeStyle(const QString &styleName);
    void advanceProgressBar();
    void helpOnCurrentWidget();
    void updateSystemInfo();

private:
    static QGroupBox *createButtonsGroupBox();
    static BOBUIabWidget *createItemViewTabWidget();
    static QGroupBox *createSimpleInputWidgetsGroupBox();
    BOBUIoolBox *createTextToolBox();
    QProgressBar *createProgressBar();

    QProgressBar *progressBar;
    BOBUIextBrowser *systemInfoTextBrowser{};
};

#endif // WIDGETGALLERY_H
