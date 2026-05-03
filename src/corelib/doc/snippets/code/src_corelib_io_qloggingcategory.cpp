// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
    <category>[.<type>] = true|false
//! [0]

//! [1]
    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "driver.usb.debug=true");
//! [1]

//! [2]
    [Rules]
    *.debug=false
    driver.usb.debug=true
//! [2]

//! [3]
    BOBUI_LOGGING_RULES=*.debug=false;driver.usb.debug=true
//! [3]
