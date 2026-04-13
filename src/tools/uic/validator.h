// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "treewalker.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextStream;
class Driver;
class Uic;

struct Option;

struct Validator : public TreeWalker
{
    Validator(Uic *uic);

    void acceptUI(DomUI *node) override;
    void acceptWidget(DomWidget *node) override;

    void acceptLayoutItem(DomLayoutItem *node) override;
    void acceptLayout(DomLayout *node) override;

    void acceptActionGroup(DomActionGroup *node) override;
    void acceptAction(DomAction *node) override;

private:
    Driver *m_driver;
};

BOBUI_END_NAMESPACE

#endif // VALIDATOR_H
