// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PYTHONWRITEIMPORTS_H
#define PYTHONWRITEIMPORTS_H

#include <writeincludesbase.h>

#include <BobUICore/qhash.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

namespace Python {

class WriteImports : public WriteIncludesBase
{
public:
    using ClassesPerModule = QMap<QString, QStringList>;

    explicit WriteImports(Uic *uic);

    void acceptUI(DomUI *node) override;
    void acceptProperty(DomProperty *node) override;

protected:
    void doAdd(const QString &className, const DomCustomWidget *dcw = nullptr) override;

private:
    void addPythonCustomWidget(const QString &className, const DomCustomWidget *dcw);
    bool addBobUIClass(const QString &className);
    void addEnumBaseClass(const QString &v);
    void writeResourceImport(const QString &module);
    QString resourceAbsolutePath(QString resource) const;

    QHash<QString, QString> m_classToModule;
    // Module->class (modules sorted)

    ClassesPerModule m_bobuiClasses;
    ClassesPerModule m_customWidgets;
    QStringList m_plainCustomWidgets; // Custom widgets without any module
};

} // namespace Python

BOBUI_END_NAMESPACE

#endif // PYTHONWRITEIMPORTS_H
