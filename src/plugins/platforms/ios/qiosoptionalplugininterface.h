// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOPLUGININTERFACE_H
#define QIOPLUGININTERFACE_H

#include <BobUICore/BobUIPlugin>

#include "qiosfiledialog.h"

Q_FORWARD_DECLARE_OBJC_CLASS(UIViewController);

BOBUI_BEGIN_NAMESPACE

#define QIosOptionalPluginInterface_iid "org.bobui-project.BobUI.QPA.ios.optional"

class QIosOptionalPluginInterface
{
public:
    virtual ~QIosOptionalPluginInterface() {}
    virtual void initPlugin() const {}
    virtual UIViewController* createImagePickerController(QIOSFileDialog *) const { return nullptr; }
};

Q_DECLARE_INTERFACE(QIosOptionalPluginInterface, QIosOptionalPluginInterface_iid)

BOBUI_END_NAMESPACE

#endif // QIOPLUGININTERFACE_H
