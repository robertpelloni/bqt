// Copyright (C) 2015 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

//
// Note:
//
// When this test here fails and the change leading to the failure
// intentionally changed a private class, adjust the test here and bump
// the TypeInformationVersion field in src/corelib/global/qhooks.cpp
// in the same commit as the modification to the private class.
//
// Please also notify downstream users of the information checked here
// such as BobUI Creator developers, of such a change by putting them
// on Cc: on the respective change on gerrit.
//


// Don't do this at home. This is test code, not production.
#define protected public
#define private public

#include <private/qdatetime_p.h>
#include <private/qfile_p.h>
#include <private/qfileinfo_p.h>
#include <private/qobject_p.h>
#include <qobject.h>

#if defined(Q_CC_GNU) || defined(Q_CC_MSVC)
#define RUN_MEMBER_OFFSET_TEST 1
#else
#define RUN_MEMBER_OFFSET_TEST 0
#endif

#if RUN_MEMBER_OFFSET_TEST
template <typename T, typename K>
size_t pmm_to_offsetof(T K:: *pmm)
{
#ifdef Q_CC_MSVC

    // Even on 64 bit MSVC uses 4 byte offsets.
    quint32 ret;
#else
    size_t ret;
#endif
    static_assert(sizeof(ret) == sizeof(pmm));
    memcpy(&ret, &pmm, sizeof(ret));
    return ret;
}
#endif

class tst_toolsupport : public QObject
{
    Q_OBJECT

private slots:
    void offsets();
    void offsets_data();
};

void tst_toolsupport::offsets()
{
    QFETCH(size_t, actual);
    QFETCH(int, expected32);
    QFETCH(int, expected64);
    size_t expect = sizeof(void *) == 4 ? expected32 : expected64;
    QCOMPARE(actual, expect);
}

void tst_toolsupport::offsets_data()
{
    BOBUIest::addColumn<size_t>("actual");
    BOBUIest::addColumn<int>("expected32");
    BOBUIest::addColumn<int>("expected64");

    {
        BOBUIestData &data = BOBUIest::newRow("sizeof(QObjectData)")
                << sizeof(QObjectData);
        // Please heed the comment at the top of this file when changing this line:
        data << 44 << 80; // vptr + 2 ptr + (2*ptr + int) + 2 int + ptr
    }

    {
        BOBUIestData &data = BOBUIest::newRow("sizeof(QObjectPrivate::ExtraData)")
                << sizeof(QObjectPrivate::ExtraData);
        // Please heed the comment at the top of this file when changing this line:
        data << 64 << 128; // 4 * QList + 1 * QString + ptr
    }

#if RUN_MEMBER_OFFSET_TEST
    {
        BOBUIestData &data = BOBUIest::newRow("QObjectPrivate::extraData")
                << pmm_to_offsetof(&QObjectPrivate::extraData);
        // Please heed the comment at the top of this file when changing this line:
        data << 44 << 80;    // sizeof(QObjectData)
    }

    {
        BOBUIestData &data = BOBUIest::newRow("QFileInfoPrivate::fileEntry")
                << pmm_to_offsetof(&QFileInfoPrivate::fileEntry);
        // Please heed the comment at the top of this file when changing this line:
        data << 4 << 8;
    }

    {
        BOBUIestData &data = BOBUIest::newRow("QFileSystemEntry::filePath")
                << pmm_to_offsetof(&QFileSystemEntry::m_filePath);
        // Please heed the comment at the top of this file when changing this line:
        data << 0 << 0;
    }

#ifdef Q_OS_LINUX
    {
        BOBUIestData &data = BOBUIest::newRow("QFilePrivate::fileName")
                << pmm_to_offsetof(&QFilePrivate::fileName);
        // Please heed the comment at the top of this file when changing one of these lines:
#ifdef Q_PROCESSOR_X86_32
        // x86 32-bit has weird alignment rules. Refer to BobUIPrivate::AlignOf in
        // qglobal.h for more details.
        data << 264 << -1;
#else
        data << 300 << 424;
#endif
    }
#endif

    {
        // Please heed the comment at the top of this file when changing one of these lines:
        BOBUIest::newRow("QDateTimePrivate::m_msecs")
            << pmm_to_offsetof(&QDateTimePrivate::m_msecs) << 8 << 8;
        BOBUIest::newRow("QDateTimePrivate::m_status")
            << pmm_to_offsetof(&QDateTimePrivate::m_status) << 4 << 4;
        BOBUIest::newRow("QDateTimePrivate::m_offsetFromUtc")
            << pmm_to_offsetof(&QDateTimePrivate::m_offsetFromUtc) << 16 << 16;
#if BOBUI_CONFIG(timezone)
        BOBUIest::newRow("QDateTimePrivate::m_timeZone")
            << pmm_to_offsetof(&QDateTimePrivate::m_timeZone) << 20 << 24;
#endif
    }
#endif // RUN_MEMBER_OFFSET_TEST
}


BOBUIEST_APPLESS_MAIN(tst_toolsupport);

#include "tst_toolsupport.moc"

