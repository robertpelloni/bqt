// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#define CATCH_CONFIG_RUNNER
#define CATCH_CLARA_CONFIG_CONSOLE_WIDTH 1000

#if defined(BOBUI_NO_EXCEPTIONS)
#define CATCH_CONFIG_DISABLE_EXCEPTIONS
#endif

#include "catch_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {

int catchMain(int argc, char **argv)
{
    Catch::Session session;

    if (int returnCode = session.applyCommandLine(argc, argv))
        return returnCode; // Command line error

    return session.run();
}

} // namespace BOBUIestPrivate

BOBUI_END_NAMESPACE

