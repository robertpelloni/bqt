// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "../tst_qmimedatabase.cpp"

void tst_QMimeDatabase::initTestCaseInternal()
{
    qputenv("BOBUI_NO_MIME_CACHE", "1");
}

bool tst_QMimeDatabase::useCacheProvider() const
{
    return false;
}

bool tst_QMimeDatabase::useFreeDesktopOrgXml() const
{
    return true;
}
