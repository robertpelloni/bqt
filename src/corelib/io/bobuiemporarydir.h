// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEMPORARYDIR_H
#define BOBUIEMPORARYDIR_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qdir.h>
#include <BobUICore/qscopedpointer.h>

BOBUI_BEGIN_NAMESPACE


#if BOBUI_CONFIG(temporaryfile)

class BOBUIemporaryDirPrivate;

class Q_CORE_EXPORT BOBUIemporaryDir
{
public:
    BOBUIemporaryDir();
    explicit BOBUIemporaryDir(const QString &templateName);
    BOBUIemporaryDir(BOBUIemporaryDir &&other) noexcept
        : d_ptr{std::exchange(other.d_ptr, nullptr)}
    { }

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(BOBUIemporaryDir)

    ~BOBUIemporaryDir();

    void swap(BOBUIemporaryDir &other) noexcept
    { bobui_ptr_swap(d_ptr, other.d_ptr); }

    bool isValid() const;
    QString errorString() const;

    bool autoRemove() const;
    void setAutoRemove(bool b);
    bool remove();

    QString path() const;
    QString filePath(const QString &fileName) const;

private:
    BOBUIemporaryDirPrivate *d_ptr;

    Q_DISABLE_COPY(BOBUIemporaryDir)
};

inline void swap(BOBUIemporaryDir &lhs, BOBUIemporaryDir &rhs) noexcept
{
    lhs.swap(rhs);
}

#endif // BOBUI_CONFIG(temporaryfile)

BOBUI_END_NAMESPACE

#endif // BOBUIEMPORARYDIR_H
