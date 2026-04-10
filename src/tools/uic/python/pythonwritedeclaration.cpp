// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "pythonwritedeclaration.h"
#include <cppwriteinitialization.h>
#include <language.h>
#include <driver.h>
#include <ui4.h>
#include <uic.h>

#include <BobUICore/bobuiextstream.h>
#include <BobUICore/qdebug.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

namespace Python {

WriteDeclaration::WriteDeclaration(Uic *uic)  :
    m_uic(uic),
    m_driver(uic->driver()),
    m_output(uic->output()),
    m_option(uic->option())
{
}

void WriteDeclaration::acceptUI(DomUI *node)
{
    // remove any left-over C++ namespaces
    const QString qualifiedClassName = "Ui_"_L1 + node->elementClass()
        + m_option.postfix;
    m_output << "class " << language::fixClassName(qualifiedClassName) << "(object):\n";

    TreeWalker::acceptWidget(node->elementWidget());
    if (const DomButtonGroups *domButtonGroups = node->elementButtonGroups())
        acceptButtonGroups(domButtonGroups);
    CPP::WriteInitialization(m_uic).acceptUI(node);
}

// Register button groups to prevent the on-the-fly creation legacy
// feature from triggering
void WriteDeclaration::acceptButtonGroup(const DomButtonGroup *buttonGroup)
{
    m_driver->findOrInsertButtonGroup(buttonGroup);
}

} // namespace Python

BOBUI_END_NAMESPACE
