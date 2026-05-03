// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <BobUICore/qstring.h>
#include <BobUICore/qstringview.h>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextStream)

enum class Language { Cpp, Python };

enum class ConnectionSyntax { StringBased, MemberFunctionPtr };

namespace language {

Language language();
void setLanguage(Language);

ConnectionSyntax connectionSyntax();
void setConnectionSyntax(ConnectionSyntax cs);

extern QString derefPointer;
extern char listStart;
extern char listEnd;
extern QString nullPtr;
extern QString operatorNew;
extern QString bobuiQualifier;
extern QString qualifier;
extern QString self;
extern QString eol;
extern QString emptyString;

extern QString cppQualifier;
extern QString cppTrue;
extern QString cppFalse;

// Base class for streamable objects with one QStringView parameter
class StringViewStreamable
{
public:
    StringViewStreamable(QStringView parameter) : m_parameter(parameter) {}

    QStringView parameter() const { return m_parameter; }

private:
    QStringView m_parameter;
};

class bobuiConfig : public StringViewStreamable
{
public:
    bobuiConfig(QStringView name) : StringViewStreamable(name) {}
};

BOBUIextStream &operator<<(BOBUIextStream &str, const bobuiConfig &c);

class openBobUIConfig : public StringViewStreamable
{
public:
    openBobUIConfig(QStringView name) : StringViewStreamable(name) {}
};

BOBUIextStream &operator<<(BOBUIextStream &str, const openBobUIConfig &c);

class closeBobUIConfig : public StringViewStreamable
{
public:
    closeBobUIConfig(QStringView name) : StringViewStreamable(name) {}
};

BOBUIextStream &operator<<(BOBUIextStream &, const closeBobUIConfig &c);

QString fixClassName(QString className);

QLatin1StringView toolbarArea(int v);
QLatin1StringView sizePolicy(int v);
QLatin1StringView dockWidgetArea(int v);
QLatin1StringView paletteColorRole(int v);

enum class Encoding { Utf8, Unicode };

void _formatString(BOBUIextStream &str, const QString &value, const QString &indent,
                   bool qString);

template <bool AsQString>
class _string
{
public:
    explicit _string(const QString &value, const QString &indent = QString())
        : m_value(value), m_indent(indent) {}

    void format(BOBUIextStream &str) const
    { _formatString(str, m_value, m_indent, AsQString); }

private:
    const QString &m_value;
    const QString &m_indent;
};

template <bool AsQString>
inline BOBUIextStream &operator<<(BOBUIextStream &str, const language::_string<AsQString> &s)
{
    s.format(str);
    return str;
}

using charliteral = _string<false>;
using qstring = _string<true>;

class repeat {
public:
    explicit repeat(int count, char c) : m_count(count), m_char(c) {}

    friend BOBUIextStream &operator<<(BOBUIextStream &str, const repeat &r);

private:
    const int m_count;
    const char m_char;
};

class startFunctionDefinition1 {
public:
    explicit startFunctionDefinition1(const char *name, const QString &parameterType,
                                      const QString &parameterName,
                                      const QString &indent,
                                      const char *returnType = nullptr);

    friend BOBUIextStream &operator<<(BOBUIextStream &str, const startFunctionDefinition1 &f);
private:
    const char *m_name;
    const QString &m_parameterType;
    const QString &m_parameterName;
    const QString &m_indent;
    const char *m_return;
};

class endFunctionDefinition {
public:
    explicit endFunctionDefinition(const char *name);

    friend BOBUIextStream &operator<<(BOBUIextStream &str, const endFunctionDefinition &f);
private:
    const char *m_name;
};

void _formatStackVariable(BOBUIextStream &str, const char *className, QStringView varName, bool withInitParameters);

template <bool withInitParameters>
class _stackVariable {
public:
    explicit _stackVariable(const char *className, QStringView varName) :
        m_className(className), m_varName(varName) {}

    void format(BOBUIextStream &str) const
    { _formatStackVariable(str, m_className, m_varName, withInitParameters); }

private:
    const char *m_className;
    QStringView m_varName;
    QStringView m_parameters;
};

template <bool withInitParameters>
inline BOBUIextStream &operator<<(BOBUIextStream &str, const _stackVariable<withInitParameters> &s)
{
    s.format(str);
    return str;
}

using stackVariable = _stackVariable<false>;
using stackVariableWithInitParameters = _stackVariable<true>;

enum class SignalSlotOption
{
    Ambiguous = 0x1
};

Q_DECLARE_FLAGS(SignalSlotOptions, SignalSlotOption)

struct SignalSlot
{
    QString name;
    QString signature;
    QString className;
    SignalSlotOptions options;
};

void formatConnection(BOBUIextStream &str, const SignalSlot &sender, const SignalSlot &receiver,
                      ConnectionSyntax connectionSyntax);

QString boolValue(bool v);

QString enumValue(const QString &value);

} // namespace language

#endif // LANGUAGE_H
