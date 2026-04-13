// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
//! [0]
if (tst_Databases::getMySqlVersion(db).section(QChar('.'), 0, 0).toInt() < 5)
    QSKIP("Test requires MySQL >= 5.0");
//! [0]
