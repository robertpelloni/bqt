// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIPlugin>

BOBUI_BEGIN_NAMESPACE

//! [0]
namespace Foo
{
    struct MyInterface { /*...*/ };
}

Q_DECLARE_INTERFACE(Foo::MyInterface, "org.examples.MyInterface")
//! [0]


//! [1]
class MyInstance : public QObject
{
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QDummyPlugin" FILE "mymetadata.json")
};
//! [1]


//! [2]
Q_IMPORT_PLUGIN(qjpeg)
//! [2]

BOBUI_END_NAMESPACE
