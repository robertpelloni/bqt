// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef UIC_H
#define UIC_H

#include "databaseinfo.h"
#include "customwidgetsinfo.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qstack.h>
#include <qxmlstream.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIextStream;
class QIODevice;

class Driver;
class DomUI;
class DomWidget;
class DomSpacer;
class DomLayout;
class DomLayoutItem;
class DomItem;

struct Option;

class Uic
{
    Q_DISABLE_COPY_MOVE(Uic)
public:
    Uic(Driver *driver);
    ~Uic();

    bool printDependencies();

    inline Driver *driver() const
    { return drv; }

    inline BOBUIextStream &output()
    { return out; }

    inline const Option &option() const
    { return opt; }

    inline QString pixmapFunction() const
    { return pixFunction; }

    inline void setPixmapFunction(const QString &f)
    { pixFunction = f; }

    inline const DatabaseInfo *databaseInfo() const
    { return &info; }

    inline const CustomWidgetsInfo *customWidgetsInfo() const
    { return &cWidgetsInfo; }

    bool write(QIODevice *in);

    bool write(DomUI *ui);

    bool isButton(const QString &className) const;
    bool isContainer(const QString &className) const;
    bool isMenu(const QString &className) const;

private:
    // copyright header
    void writeCopyrightHeaderCpp(const DomUI *ui) const;
    void writeCopyrightHeaderPython(const DomUI *ui) const;
    static DomUI *parseUiFile(QXmlStreamReader &reader);

    // header protection
    void writeHeaderProtectionStart();
    void writeHeaderProtectionEnd();

    Driver *drv;
    BOBUIextStream &out;
    Option &opt;
    DatabaseInfo info;
    CustomWidgetsInfo cWidgetsInfo;
    QString pixFunction;
};

BOBUI_END_NAMESPACE

#endif // UIC_H
