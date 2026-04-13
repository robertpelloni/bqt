// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef MSVC_VCXPROJ_H
#define MSVC_VCXPROJ_H

#include "msvc_vcproj.h"

BOBUI_BEGIN_NAMESPACE

class VcxprojGenerator : public VcprojGenerator
{
public:
    VcxprojGenerator();

protected:
    VCProjectWriter *createProjectWriter() override;
};

BOBUI_END_NAMESPACE

#endif // MSVC_VCXPROJ_H
