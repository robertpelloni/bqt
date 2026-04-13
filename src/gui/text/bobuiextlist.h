// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTLIST_H
#define BOBUIEXTLIST_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/bobuiextobject.h>
#include <BobUICore/qobject.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextListPrivate;
class BOBUIextCursor;

class Q_GUI_EXPORT BOBUIextList : public BOBUIextBlockGroup
{
    Q_OBJECT
public:
    explicit BOBUIextList(BOBUIextDocument *doc);
    ~BOBUIextList();

    int count() const;

    BOBUIextBlock item(int i) const;

    int itemNumber(const BOBUIextBlock &) const;
    QString itemText(const BOBUIextBlock &) const;

    void removeItem(int i);
    void remove(const BOBUIextBlock &);

    void add(const BOBUIextBlock &block);

    inline void setFormat(const BOBUIextListFormat &format);
    BOBUIextListFormat format() const { return BOBUIextObject::format().toListFormat(); }

private:
    Q_DISABLE_COPY(BOBUIextList)
    Q_DECLARE_PRIVATE(BOBUIextList)
};

inline void BOBUIextList::setFormat(const BOBUIextListFormat &aformat)
{ BOBUIextObject::setFormat(aformat); }

BOBUI_END_NAMESPACE

#endif // BOBUIEXTLIST_H
