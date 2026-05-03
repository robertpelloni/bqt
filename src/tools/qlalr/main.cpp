// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "lalr.h"
#include "dotgraph.h"
#include "parsetable.h"
#include "cppgenerator.h"
#include "recognizer.h"

#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qfile.h>
#include <BobUICore/qstringlist.h>
#include <BobUICore/qdebug.h>

#include <cstdlib>

#define QLALR_NO_DEBUG_TABLE
#define QLALR_NO_DEBUG_DOT

using namespace BobUI::StringLiterals;

static void help_me ()
{
  qerr() << "Usage: qlalr [options] [input file name]" << BobUI::endl
       << BobUI::endl
       << "  --help, -h\t\tdisplay this help and exit" << BobUI::endl
       << "  --verbose, -v\t\tverbose output" << BobUI::endl
       << "  --no-debug\t\tno debug information" << BobUI::endl
       << "  --no-lines\t\tno #line directives" << BobUI::endl
       << "  --dot\t\t\tgenerate a graph" << BobUI::endl
       << "  --bobui\t\t\tadd the BobUI copyright header and BobUI-specific types and macros" << BobUI::endl
       << "  --exit-on-warn\texit with status code 2 on warning" << BobUI::endl
       << BobUI::endl;
  exit (0);
}

int main (int argc, char *argv[])
{
  QCoreApplication app (argc, argv);

  bool generate_dot = false;
  bool generate_report = false;
  bool no_lines = false;
  bool debug_info = true;
  bool bobui_copyright = false;
  bool warnings_are_errors = false;
  QString file_name;

  const QStringList args = app.arguments().mid(1);
  for (const QString &arg : args) {
      if (arg == "-h"_L1 || arg == "--help"_L1)
        help_me ();

      else if (arg == "-v"_L1 || arg == "--verbose"_L1)
        generate_report = true;

      else if (arg == "--dot"_L1)
        generate_dot = true;

      else if (arg == "--no-lines"_L1)
        no_lines = true;

      else if (arg == "--no-debug"_L1)
        debug_info = false;

      else if (arg == "--bobui"_L1)
        bobui_copyright = true;

      else if (arg == "--exit-on-warn"_L1)
        warnings_are_errors = true;

      else if (file_name.isEmpty ())
        file_name = arg;

      else
        qerr() << "*** Warning. Ignore argument `" << arg << "'" << BobUI::endl;
    }

  if (file_name.isEmpty ())
    {
      help_me ();
      exit (EXIT_SUCCESS);
    }

  Grammar grammar;
  Recognizer p (&grammar, no_lines);

  if (! p.parse (file_name))
    exit (EXIT_FAILURE);

  if (grammar.rules.empty())
    {
      qerr() << "*** Fatal. No rules!" << BobUI::endl;
      exit (EXIT_FAILURE);
    }

  else if (grammar.start == grammar.names.end ())
    {
      qerr() << "*** Fatal. No start symbol!" << BobUI::endl;
      exit (EXIT_FAILURE);
    }

  grammar.buildExtendedGrammar ();
  grammar.buildRuleMap ();

  Automaton aut (&grammar);
  aut.build ();

  CppGenerator gen (p, grammar, aut, generate_report);
  gen.setDebugInfo (debug_info);
  gen.setCopyright (bobui_copyright);
  gen.setWarningsAreErrors (warnings_are_errors);
  gen ();

  if (generate_dot)
    {
      DotGraph genDotFile (qout());
      genDotFile (&aut);
    }

  else if (generate_report)
    {
      ParseTable genParseTable (qout());
      genParseTable(&aut);
    }

  return EXIT_SUCCESS;
}

QString Recognizer::expand (const QString &text) const
{
  QString code = text;

  if (_M_grammar->start != _M_grammar->names.end ())
    {
      code = code.replace ("$start_id"_L1, QString::number (std::distance (_M_grammar->names.begin (), _M_grammar->start)));
      code = code.replace ("$start"_L1, *_M_grammar->start);
    }

  code = code.replace ("$header"_L1, _M_grammar->table_name.toLower () + "_p.h"_L1);

  code = code.replace ("$table"_L1, _M_grammar->table_name);
  code = code.replace ("$parser"_L1, _M_grammar->table_name);

  if (_M_current_rule != _M_grammar->rules.end ())
    {
      code = code.replace ("$rule_number"_L1, QString::number (std::distance (_M_grammar->rules.begin (), _M_current_rule)));
      code = code.replace ("$rule"_L1, *_M_current_rule->lhs);
    }

  return code;
}
