// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "settings.h"

bool readSettingsFromCommandLine(int argc,
                                 char *argv[],
                                 Settings& settings);


#endif // COMMANDLINE_H
