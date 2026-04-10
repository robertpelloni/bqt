// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "../tst_qmimedatabase.cpp"

void tst_QMimeDatabase::initTestCaseInternal()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    const QString mimeDirName = m_globalXdgDir + QStringLiteral("/mime");
    runUpdateMimeDatabase(mimeDirName);
    QVERIFY(QFile::exists(mimeDirName + QStringLiteral("/mime.cache")));
#endif
}

bool tst_QMimeDatabase::useCacheProvider() const
{
    return true;
}

bool tst_QMimeDatabase::useFreeDesktopOrgXml() const
{
    return true;
}
