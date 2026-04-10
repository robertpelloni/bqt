// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WIZARDPANEL_H
#define WIZARDPANEL_H

#include <QWidget>

class WizardStyleControl;
class WizardOptionsControl;
BOBUI_BEGIN_NAMESPACE
class QWizard;
BOBUI_END_NAMESPACE

class WizardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit WizardPanel(QWidget *parent = nullptr);

public slots:
    void execModal();
    void showModal(BobUI::WindowModality modality);
    void showNonModal();
    void showEmbedded();

private:
    void applyParameters(QWizard *wizard) const;

    WizardStyleControl *m_styleControl;
    WizardOptionsControl *m_optionsControl;
};

#endif // WIZARDPANEL_H
