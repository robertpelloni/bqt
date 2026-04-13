// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef COLORDIALOGPANEL_H
#define COLORDIALOGPANEL_H

#include <QPointer>
#include <QColorDialog>

BOBUI_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QPushButton;
BOBUI_END_NAMESPACE

class ColorDialogPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorDialogPanel(QWidget *parent = nullptr);

public slots:
    void execModal();
    void showModal(BobUI::WindowModality modality);
    void showNonModal();
    void deleteNonModalDialog();
    void deleteModalDialog();
    void accepted();
    void rejected();
    void currentColorChanged(const QColor & color);
    void showAcceptedResult();
    void restoreDefaults();

private slots:
    void enableDeleteNonModalDialogButton();
    void enableDeleteModalDialogButton();

private:
    void applySettings(QColorDialog *d) const;

    QComboBox *m_colorComboBox;
    QCheckBox *m_showAlphaChannel;
    QCheckBox *m_noButtons;
    QCheckBox *m_dontUseNativeDialog;
    QPushButton *m_deleteNonModalDialogButton;
    QPushButton *m_deleteModalDialogButton;
    QString m_result;
    QPointer<QColorDialog> m_modalDialog;
    QPointer<QColorDialog> m_nonModalDialog;
};

#endif // COLORDIALOGPANEL_H
