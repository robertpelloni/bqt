// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PYTHONWRITEDECLARATION_H
#define PYTHONWRITEDECLARATION_H

#include <treewalker.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIextStream;
class Driver;
class Uic;

struct Option;

namespace Python {

struct WriteDeclaration : public TreeWalker
{
    WriteDeclaration(Uic *uic);

    void acceptUI(DomUI *node) override;
    void acceptButtonGroup(const DomButtonGroup *buttonGroup) override;

private:
    Uic *m_uic;
    Driver *m_driver;
    BOBUIextStream &m_output;
    const Option &m_option;
};

} // namespace Python

BOBUI_END_NAMESPACE

#endif // PYTHONWRITEDECLARATION_H
