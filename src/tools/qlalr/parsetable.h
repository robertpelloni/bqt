// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PARSETABLE_H
#define PARSETABLE_H

#include <BobUICore/qglobal.h>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextStream);
class Automaton;

class ParseTable
{
public:
  ParseTable (BOBUIextStream &out);

  void operator () (Automaton *a);

private:
  BOBUIextStream &out;
};

#endif // PARSETABLE_H
