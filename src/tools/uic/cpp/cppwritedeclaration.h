// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef CPPWRITEDECLARATION_H
#define CPPWRITEDECLARATION_H

#include "treewalker.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextStream;
class Driver;
class Uic;

struct Option;

namespace CPP {

struct WriteDeclaration : public TreeWalker
{
    WriteDeclaration(Uic *uic);

    void acceptUI(DomUI *node) override;
    void acceptWidget(DomWidget *node) override;
    void acceptSpacer(DomSpacer *node) override;
    void acceptLayout(DomLayout *node) override;
    void acceptActionGroup(DomActionGroup *node) override;
    void acceptAction(DomAction *node) override;
    void acceptButtonGroup(const DomButtonGroup *buttonGroup) override;

private:
    Uic *m_uic;
    Driver *m_driver;
    BOBUIextStream &m_output;
    const Option &m_option;
};

} // namespace CPP

BOBUI_END_NAMESPACE

#endif // CPPWRITEDECLARATION_H
