// REUSE-IgnoreStart
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
// REUSE-IgnoreEnd

#include "cppgenerator.h"

#include "lalr.h"
#include "recognizer.h"

#include <BobUICore/qbitarray.h>
#include <BobUICore/bobuiextstream.h>
#include <BobUICore/qfile.h>
#include <BobUICore/qmap.h>
#include <BobUICore/private/qconfig_p.h>

#include <iterator>

using namespace BobUI::StringLiterals;

namespace {

void generateSeparator(int i, BOBUIextStream &out)
{
    if (!(i % 10)) {
        if (i)
            out << ",";
        out << BobUI::endl << "    ";
    } else {
        out << ", ";
    }
}

void generateList(const QList<int> &list, BOBUIextStream &out)
{
    for (int i = 0; i < list.size(); ++i) {
        generateSeparator(i, out);

        out << list[i];
    }
}

}
// REUSE-IgnoreStart
QString CppGenerator::copyrightHeader() const
{
  return
    "// " BOBUI_COPYRIGHT "\n"
    "// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0\n"
    "\n"_L1;
}
// REUSE-IgnoreEnd

QString CppGenerator::privateCopyrightHeader() const
{
  return
    "//\n"
    "//  W A R N I N G\n"
    "//  -------------\n"
    "//\n"
    "// This file is not part of the BobUI API.  It exists for the convenience\n"
    "// of other BobUI classes.  This header file may change from version to\n"
    "// version without notice, or even be removed.\n"
    "//\n"
    "// We mean it.\n"
    "//\n"_L1;
}

QString CppGenerator::startIncludeGuard(const QString &fileName)
{
    const QString normalized(QString(fileName).replace(u'.', u'_').toUpper());

    return QString::fromLatin1("#ifndef %1\n"
                               "#define %2\n").arg(normalized, normalized);
}

QString CppGenerator::endIncludeGuard(const QString &fileName)
{
    const QString normalized(QString(fileName).replace(u'.', u'_').toUpper());

    return QString::fromLatin1("#endif // %1\n").arg(normalized);
}

void CppGenerator::operator () ()
{
  // action table...
  state_count = static_cast<int>(aut.states.size());
  terminal_count = static_cast<int>(grammar.terminals.size());
  non_terminal_count = static_cast<int>(grammar.non_terminals.size());

#define ACTION(i, j) table [(i) * terminal_count + (j)]
#define GOTO(i, j) pgoto [(i) * non_terminal_count + (j)]

  int *table = new int [state_count * terminal_count];
  ::memset (table, 0, state_count * terminal_count * sizeof (int));

  int *pgoto = new int [state_count * non_terminal_count];
  ::memset (pgoto, 0, state_count * non_terminal_count * sizeof (int));

  accept_state = -1;
  int shift_reduce_conflict_count = 0;
  int reduce_reduce_conflict_count = 0;

  for (StatePointer state = aut.states.begin (); state != aut.states.end (); ++state)
    {
      int q = aut.id (state);

      for (Bundle::iterator a = state->bundle.begin (); a != state->bundle.end (); ++a)
        {
          int symbol = aut.id (a.key ());
          int r = aut.id (a.value ());

          Q_ASSERT (r < state_count);

          if (grammar.isNonTerminal (a.key ()))
            {
              Q_ASSERT(symbol >= terminal_count && symbol < static_cast<int>(grammar.names.size()));
              GOTO (q, symbol - terminal_count) = r;
            }

          else
            ACTION (q, symbol) = r;
        }

      for (ItemPointer item = state->closure.begin (); item != state->closure.end (); ++item)
        {
          if (item->dot != item->end_rhs ())
            continue;

          int r = aut.id (item->rule);

          const NameSet lookaheads = aut.lookaheads.value (item);

          if (item->rule == grammar.goal)
            accept_state = q;

          for (const Name &s : lookaheads)
            {
              int &u = ACTION (q, aut.id (s));

              if (u == 0)
                u = - r;

              else if (u < 0)
                {
                  if (verbose)
                    qout() << "*** Warning. Found a reduce/reduce conflict in state " << q << " on token ``" << s << "'' between rule "
                         << r << " and " << -u << BobUI::endl;

                  ++reduce_reduce_conflict_count;

                  u = qMax (u, -r);

                  if (verbose)
                    qout() << "\tresolved using rule " << -u << BobUI::endl;
                }

              else if (u > 0)
                {
                  if (item->rule->prec != grammar.names.end() && grammar.token_info.contains (s))
                    {
                      Grammar::TokenInfo info_r = grammar.token_info.value (item->rule->prec);
                      Grammar::TokenInfo info_s = grammar.token_info.value (s);

                      if (info_r.prec > info_s.prec)
                        u = -r;
                      else if (info_r.prec == info_s.prec)
                        {
                          switch (info_r.assoc) {
                          case Grammar::Left:
                            u = -r;
                            break;
                          case Grammar::Right:
                            // shift... nothing to do
                            break;
                          case Grammar::NonAssoc:
                            u = 0;
                            break;
                          } // switch
                        }
                    }

                  else
                    {
                      ++shift_reduce_conflict_count;

                      if (verbose)
                        qout() << "*** Warning. Found a shift/reduce conflict in state " << q << " on token ``" << s << "'' with rule " << r << BobUI::endl;
                    }
                }
            }
        }
    }

  if (shift_reduce_conflict_count || reduce_reduce_conflict_count)
    {
      if (shift_reduce_conflict_count != grammar.expected_shift_reduce
          || reduce_reduce_conflict_count != grammar.expected_reduce_reduce)
        {
          qerr() << "*** Conflicts: " << shift_reduce_conflict_count << " shift/reduce, " << reduce_reduce_conflict_count << " reduce/reduce" << BobUI::endl;
          if (warnings_are_errors)
            {
              qerr() << "qlalr: error: warning occurred, treating as error due to "
                        "--exit-on-warn." << BobUI::endl;
              exit(2);
            }
        }

      if (verbose)
        qout() << BobUI::endl << "*** Conflicts: " << shift_reduce_conflict_count << " shift/reduce, " << reduce_reduce_conflict_count << " reduce/reduce" << BobUI::endl
             << BobUI::endl;
    }

  QBitArray used_rules{static_cast<int>(grammar.rules.size())};

  int q = 0;
  for (StatePointer state = aut.states.begin (); state != aut.states.end (); ++state, ++q)
    {
      for (int j = 0; j < terminal_count; ++j)
        {
          int &u = ACTION (q, j);

          if (u < 0)
            used_rules.setBit (-u - 1);
        }
    }

  auto rule = grammar.rules.begin();
  for (int i = 0; i < used_rules.size(); ++i, ++rule)
    {
      if (! used_rules.testBit (i))
        {
          if (rule != grammar.goal)
            {
              qerr() << "*** Warning: Rule ``" << *rule << "'' is useless!" << BobUI::endl;
              if (warnings_are_errors)
                {
                  qerr() << "qlalr: error: warning occurred, treating as error due to "
                            "--exit-on-warn." << BobUI::endl;
                  exit(2);
                }
            }
        }
    }

  q = 0;
  for (StatePointer state = aut.states.begin (); state != aut.states.end (); ++state, ++q)
    {
      for (int j = 0; j < terminal_count; ++j)
        {
          int &u = ACTION (q, j);

          if (u >= 0)
            continue;

          RulePointer rule = std::next(grammar.rules.begin(), - u - 1);

          if (state->defaultReduce == rule)
            u = 0;
        }
    }

  // ... compress the goto table
  defgoto.resize (non_terminal_count);
  for (int j = 0; j < non_terminal_count; ++j)
    {
      count.fill (0, state_count);

      int &mx = defgoto [j];

      for (int i = 0; i < state_count; ++i)
        {
          int r = GOTO (i, j);

          if (! r)
            continue;

          ++count [r];

          if (count [r] > count [mx])
            mx = r;
        }
    }

  for (int i = 0; i < state_count; ++i)
    {
      for (int j = 0; j < non_terminal_count; ++j)
        {
          int &r = GOTO (i, j);

          if (r == defgoto [j])
            r = 0;
        }
    }

  compressed_action (table, state_count, terminal_count);
  compressed_goto (pgoto, state_count, non_terminal_count);

  delete[] table;
  table = nullptr;

  delete[] pgoto;
  pgoto = nullptr;

#undef ACTION
#undef GOTO

  if (! grammar.merged_output.isEmpty())
    {
      QFile f(grammar.merged_output);
      if (! f.open (QFile::WriteOnly))
        {
          fprintf (stderr, "*** cannot create %s\n", qPrintable(grammar.merged_output));
          return;
        }

      BOBUIextStream out (&f);

      // copyright headers must come first, otherwise the headers tests will fail
      if (copyright)
        {
          out << copyrightHeader()
              << privateCopyrightHeader()
              << BobUI::endl;
        }

      out << "// This file was generated by qlalr - DO NOT EDIT!\n";

      out << startIncludeGuard(grammar.merged_output) << BobUI::endl;

      if (copyright) {
          out << "#if defined(ERROR)" << BobUI::endl
              << "#  undef ERROR" << BobUI::endl
              << "#endif" << BobUI::endl << BobUI::endl;
      }

      generateDecl (out);
      generateImpl (out);
      out << p.decls();
      out << p.impls();
      out << BobUI::endl;

      out << endIncludeGuard(grammar.merged_output) << BobUI::endl;

      return;
    }

  // default behaviour
  QString declFileName = grammar.table_name.toLower () + "_p.h"_L1;
  QString bitsFileName = grammar.table_name.toLower () + ".cpp"_L1;

  { // decls...
    QFile f (declFileName);
    if (! f.open (QFile::WriteOnly))
      {
        fprintf (stderr, "*** cannot create %s: %s\n",
                 qPrintable(declFileName), qPrintable(f.errorString()));
        return;
      }
    BOBUIextStream out (&f);

    QString prot = declFileName.toUpper ().replace (QLatin1Char ('.'), QLatin1Char ('_'));

    // copyright headers must come first, otherwise the headers tests will fail
    if (copyright)
      {
        out << copyrightHeader()
            << privateCopyrightHeader()
            << BobUI::endl;
      }

    out << "// This file was generated by qlalr - DO NOT EDIT!\n";

    out << "#ifndef " << prot << BobUI::endl
        << "#define " << prot << BobUI::endl
        << BobUI::endl;

    if (copyright) {
        out << "#include <BobUICore/qglobal.h>" << BobUI::endl << BobUI::endl;
        out << "BOBUI_BEGIN_NAMESPACE" << BobUI::endl << BobUI::endl;
    }
    generateDecl (out);
    if (copyright)
        out << "BOBUI_END_NAMESPACE" << BobUI::endl;

    out << "#endif // " << prot << BobUI::endl << BobUI::endl;
  } // end decls

  { // bits...
    QFile f (bitsFileName);
    if (! f.open (QFile::WriteOnly))
      {
        fprintf (stderr, "*** cannot create %s: %s\n",
                 qPrintable(bitsFileName), qPrintable(f.errorString()));
        return;
      }
    BOBUIextStream out (&f);

    // copyright headers must come first, otherwise the headers tests will fail
    if (copyright)
      out << copyrightHeader();

    out << "// This file was generated by qlalr - DO NOT EDIT!\n";

    out << "#include \"" << declFileName << "\"" << BobUI::endl << BobUI::endl;
    if (copyright)
        out << "BOBUI_BEGIN_NAMESPACE" << BobUI::endl << BobUI::endl;
    generateImpl(out);
    if (copyright)
        out << "BOBUI_END_NAMESPACE" << BobUI::endl;

  } // end bits

  if (! grammar.decl_file_name.isEmpty ())
    {
      QFile f (grammar.decl_file_name);
      if (! f.open (QFile::WriteOnly))
        {
          fprintf (stderr, "*** cannot create %s: %s\n",
                   qPrintable(grammar.decl_file_name), qPrintable(f.errorString()));
          return;
        }
      BOBUIextStream out (&f);
      out << p.decls();
    }

  if (! grammar.impl_file_name.isEmpty ())
    {
      QFile f (grammar.impl_file_name);
      if (! f.open (QFile::WriteOnly))
        {
          fprintf (stderr, "*** cannot create %s: %s\n",
                   qPrintable(grammar.impl_file_name), qPrintable(f.errorString()));
          return;
        }
      BOBUIextStream out (&f);
      out << p.impls();
    }
}

QString CppGenerator::debugInfoProt() const
{
    QString prot = "QLALR_NO_"_L1;
    prot += grammar.table_name.toUpper();
    prot += "_DEBUG_INFO"_L1;
    return prot;
}

void CppGenerator::generateDecl (BOBUIextStream &out)
{
  out << "class " << grammar.table_name << BobUI::endl
      << "{" << BobUI::endl
      << "public:" << BobUI::endl
      << "    enum VariousConstants {" << BobUI::endl;

  for (const Name &t : std::as_const(grammar.terminals))
    {
      QString name = *t;
      int value = std::distance (grammar.names.begin (), t);

      if (name == "$end"_L1)
        name = "EOF_SYMBOL"_L1;

      else if (name == "$accept"_L1)
        name = "ACCEPT_SYMBOL"_L1;

      else
        name.prepend (grammar.token_prefix);

      out << "        " << name << " = " << value << "," << BobUI::endl;
    }

  out << BobUI::endl
      << "        ACCEPT_STATE = " << accept_state << "," << BobUI::endl
      << "        RULE_COUNT = " << grammar.rules.size () << "," << BobUI::endl
      << "        STATE_COUNT = " << state_count << "," << BobUI::endl
      << "        TERMINAL_COUNT = " << terminal_count << "," << BobUI::endl
      << "        NON_TERMINAL_COUNT = " << non_terminal_count << "," << BobUI::endl
      << BobUI::endl
      << "        GOTO_INDEX_OFFSET = " << compressed_action.index.size () << "," << BobUI::endl
      << "        GOTO_INFO_OFFSET = " << compressed_action.info.size () << "," << BobUI::endl
      << "        GOTO_CHECK_OFFSET = " << compressed_action.check.size () << BobUI::endl
      << "    };" << BobUI::endl
      << BobUI::endl
      << "    static const char *const     spell[];" << BobUI::endl
      << "    static const short             lhs[];" << BobUI::endl
      << "    static const short             rhs[];" << BobUI::endl;

  if (debug_info)
    {
      QString prot = debugInfoProt();

      out << BobUI::endl << "#ifndef " << prot << BobUI::endl
          << "    static const int     rule_index[];" << BobUI::endl
          << "    static const int      rule_info[];" << BobUI::endl
          << "#endif // " << prot << BobUI::endl << BobUI::endl;
    }

  out << "    static const short    goto_default[];" << BobUI::endl
      << "    static const short  action_default[];" << BobUI::endl
      << "    static const short    action_index[];" << BobUI::endl
      << "    static const short     action_info[];" << BobUI::endl
      << "    static const short    action_check[];" << BobUI::endl
      << BobUI::endl
      << "    static inline int nt_action (int state, int nt)" << BobUI::endl
      << "    {" << BobUI::endl
      << "        const int yyn = action_index [GOTO_INDEX_OFFSET + state] + nt;" << BobUI::endl
      << "        if (yyn < 0 || action_check [GOTO_CHECK_OFFSET + yyn] != nt)" << BobUI::endl
      << "            return goto_default [nt];" << BobUI::endl
      << BobUI::endl
      << "        return action_info [GOTO_INFO_OFFSET + yyn];" << BobUI::endl
      << "    }" << BobUI::endl
      << BobUI::endl
      << "    static inline int t_action (int state, int token)" << BobUI::endl
      << "    {" << BobUI::endl
      << "        const int yyn = action_index [state] + token;" << BobUI::endl
      << BobUI::endl
      << "        if (yyn < 0 || action_check [yyn] != token)" << BobUI::endl
      << "            return - action_default [state];" << BobUI::endl
      << BobUI::endl
      << "        return action_info [yyn];" << BobUI::endl
      << "    }" << BobUI::endl
      << "};" << BobUI::endl
      << BobUI::endl
      << BobUI::endl;
}

void CppGenerator::generateImpl (BOBUIextStream &out)
{
  int idx = 0;

  out << "const char *const " << grammar.table_name << "::spell [] = {";
  idx = 0;

  QMap<Name, int> name_ids;
  bool first_nt = true;

  for (Name t = grammar.names.begin (); t != grammar.names.end (); ++t, ++idx)
    {
      bool terminal = grammar.isTerminal (t);

      if (! (debug_info || terminal))
        break;

      name_ids.insert (t, idx);

      generateSeparator(idx, out);

      if (terminal)
        {
          QString spell = grammar.spells.value (t);

          if (spell.isEmpty ())
            out << "0";
          else
            out << "\"" << spell << "\"";
        }
      else
        {
          if (first_nt)
            {
              first_nt = false;
              QString prot = debugInfoProt();
              out << BobUI::endl << "#ifndef " << prot << BobUI::endl;
            }
          out << "\"" << *t << "\"";
        }
    }

  if (debug_info)
    out << BobUI::endl << "#endif // " << debugInfoProt() << BobUI::endl;

  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::lhs [] = {";
  idx = 0;
  for (RulePointer rule = grammar.rules.begin (); rule != grammar.rules.end (); ++rule, ++idx)
    {
      generateSeparator(idx, out);

      out << aut.id (rule->lhs);
    }
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::rhs [] = {";
  idx = 0;
  for (RulePointer rule = grammar.rules.begin (); rule != grammar.rules.end (); ++rule, ++idx)
    {
      generateSeparator(idx, out);

      out << rule->rhs.size ();
    }
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  if (debug_info)
    {
      QString prot = debugInfoProt();

      out << BobUI::endl << "#ifndef " << prot << BobUI::endl;
      out << "const int " << grammar.table_name << "::rule_info [] = {";
      idx = 0;
      for (auto rule = grammar.rules.cbegin (); rule != grammar.rules.cend (); ++rule, ++idx)
        {
          generateSeparator(idx, out);

          out << name_ids.value(rule->lhs);

          for (const Name &n : rule->rhs)
            out << ", " << name_ids.value (n);
        }
      out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

      out << "const int " << grammar.table_name << "::rule_index [] = {";
      idx = 0;
      size_t offset = 0;
      for (RulePointer rule = grammar.rules.begin (); rule != grammar.rules.end (); ++rule, ++idx)
        {
          generateSeparator(idx, out);

          out << offset;
          offset += rule->rhs.size () + 1;
        }
      out << BobUI::endl << "};" << BobUI::endl
          << "#endif // " << prot << BobUI::endl << BobUI::endl;
    }

  out << "const short " << grammar.table_name << "::action_default [] = {";
  idx = 0;
  for (StatePointer state = aut.states.begin (); state != aut.states.end (); ++state, ++idx)
    {
      generateSeparator(idx, out);

      if (state->defaultReduce != grammar.rules.end ())
        out << aut.id (state->defaultReduce);
      else
        out << "0";
    }
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::goto_default [] = {";
  generateList(defgoto, out);
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::action_index [] = {";
  generateList(compressed_action.index, out);
  out << "," << BobUI::endl;
  generateList(compressed_goto.index, out);
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::action_info [] = {";
  generateList(compressed_action.info, out);
  out << "," << BobUI::endl;
  generateList(compressed_goto.info, out);
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;

  out << "const short " << grammar.table_name << "::action_check [] = {";
  generateList(compressed_action.check, out);
  out << "," << BobUI::endl;
  generateList(compressed_goto.check, out);
  out << BobUI::endl << "};" << BobUI::endl << BobUI::endl;
}
