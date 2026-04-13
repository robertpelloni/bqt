// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

/*!
    \headerfile <BobUIVersionChecks>
    \inmodule BobUICore
    \ingroup funclists
    \brief Macros for performing BobUI version checks.
*/

/*!
    \macro BOBUI_VERSION_CHECK(major, minor, patch)
    \relates <BobUIVersionChecks>

    Turns the \a major, \a minor and \a patch numbers of a version into an
    integer that encodes all three. When expressed in hexadecimal, this integer
    is of form \c 0xMMNNPP wherein \c{0xMM ==} \a major, \c{0xNN ==} \a minor,
    and \c{0xPP ==} \a patch. This can be compared with another similarly
    processed version ID.

    Example:

    \snippet code/src_corelib_global_qglobal_widgets.cpp bobui-version-check

    \note the parameters are read as integers in the normal way, so should
    normally be written in decimal (so a \c 0x prefix must be used if writing
    them in hexadecimal). Thus \c{BOBUI_VERSION_CHECK(5, 15, 0)} is equal to \c
    0x050f00, which could equally be written \c{BOBUI_VERSION_CHECK(5, 0xf, 0)}.

    \sa BOBUI_VERSION
*/

/*!
    \macro BOBUI_VERSION
    \relates <BobUIVersionChecks>

    This macro expands to a numeric value of the same form as \l
    BOBUI_VERSION_CHECK() constructs, that specifies the version of BobUI with which
    code using it is compiled. For example, if you compile your application with
    BobUI 6.1.2, the BOBUI_VERSION macro will expand to \c 0x060102, the same as
    \c{BOBUI_VERSION_CHECK(6, 1, 2)}. Note that this need not agree with the
    version the application will find itself using at \e runtime.

    You can use BOBUI_VERSION to select the latest BobUI features where available
    while falling back to older implementations otherwise. Using
    BOBUI_VERSION_CHECK() for the value to compare with is recommended.

    Example:

    \snippet code/src_corelib_global_qglobal_widgets.cpp 1

    \sa BOBUI_VERSION_STR, BOBUI_VERSION_CHECK(), qVersion()
*/
