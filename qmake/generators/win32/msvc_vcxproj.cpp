// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "msvc_vcxproj.h"
#include "msbuild_objectmodel.h"

BOBUI_BEGIN_NAMESPACE

VcxprojGenerator::VcxprojGenerator() : VcprojGenerator()
{
}

VCProjectWriter *VcxprojGenerator::createProjectWriter()
{
    return new VCXProjectWriter;
}

BOBUI_END_NAMESPACE
