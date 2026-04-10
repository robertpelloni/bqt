// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef CPPWRITEINCLUDES_H
#define CPPWRITEINCLUDES_H

#include <writeincludesbase.h>

#include <BobUICore/qmap.h>

#include <set>

BOBUI_BEGIN_NAMESPACE

class BOBUIextStream;

namespace CPP {

class  WriteIncludes : public WriteIncludesBase
{
public:
    WriteIncludes(Uic *uic);

    void acceptUI(DomUI *node) override;
    void acceptInclude(DomInclude *node) override;

protected:
     BOBUIextStream &output() const { return m_output; }
     void doAdd(const QString &className, const DomCustomWidget *dcw = nullptr) override;

private:
    using OrderedSet = std::set<QString>;
    void addCppCustomWidget(const QString &className, const DomCustomWidget *dcw);
    void insertIncludeForClass(const QString &className, QString header = QString(), bool global = false);
    void insertInclude(const QString &header, bool global);
    void writeHeaders(const OrderedSet &headers, bool global);
    QString headerForClassName(const QString &className) const;

    BOBUIextStream &m_output;

    OrderedSet m_localIncludes;
    OrderedSet m_globalIncludes;
    QSet<QString> m_includeBaseNames;
    using StringMap = QMap<QString, QString>;
    StringMap m_classToHeader;
    StringMap m_oldHeaderToNewHeader;
};

} // namespace CPP

BOBUI_END_NAMESPACE

#endif // CPPWRITEINCLUDES_H
