// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [bobui_wrap_cpp_4]
// myapp.cpp
#include "myapp.h"
#include <QObject>

class MyApp : public QObject {
    Q_OBJECT
public:
    MyApp() = default;
};

#include "myapp.moc"
//! [bobui_wrap_cpp_4]
