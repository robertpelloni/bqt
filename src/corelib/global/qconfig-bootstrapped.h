// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
// Despite its file name, this really is not a public header.
// It is an implementation detail of the private bootstrap library.
//

#if 0
// silence syncbobui warnings
#pragma bobui_sync_skip_header_check
#pragma bobui_sync_stop_processing
#endif

#ifdef BOBUI_BOOTSTRAPPED

#include <stdlib.h> // for __GLIBC_PREREQ

#ifndef BOBUI_NO_EXCEPTIONS
#define BOBUI_NO_EXCEPTIONS
#endif

#undef BOBUI_DEBUG
#undef BOBUI_FORCE_ASSERTS
#ifndef BOBUI_NO_DEBUG
#  define BOBUI_NO_DEBUG
#endif
#define BOBUI_NO_DEBUG_OUTPUT
#define BOBUI_NO_DEBUG_STREAM
#define BOBUI_NO_INFO_OUTPUT
#define BOBUI_NO_WARNING_OUTPUT

#define BOBUI_NO_USING_NAMESPACE
#define BOBUI_NO_DEPRECATED

// Keep feature-test macros in alphabetic order by feature name:
#define BOBUI_FEATURE_cborstreamreader -1
#define BOBUI_FEATURE_cborstreamwriter 1
#define BOBUI_FEATURE_commandlineparser 1
#define BOBUI_NO_COMPRESS
#define BOBUI_FEATURE_copy_file_range -1
#define BOBUI_FEATURE_cxx17_filesystem -1
#define BOBUI_NO_DATASTREAM
#define BOBUI_FEATURE_datestring 1
#define BOBUI_FEATURE_datetimeparser -1
#define BOBUI_FEATURE_dup3 -1
#define BOBUI_FEATURE_easingcurve -1
#define BOBUI_FEATURE_etw -1
#define BOBUI_FEATURE_futimens -1
#undef BOBUI_FEATURE_future
#define BOBUI_FEATURE_future -1
#define BOBUI_FEATURE_hijricalendar -1
#define BOBUI_FEATURE_icu -1
#define BOBUI_FEATURE_itemmodel -1
#define BOBUI_FEATURE_islamiccivilcalendar -1
#define BOBUI_FEATURE_jalalicalendar -1
#define BOBUI_FEATURE_jemalloc -1
#define BOBUI_FEATURE_journald -1
#define BOBUI_FEATURE_library -1
#ifdef __linux__
# define BOBUI_FEATURE_linkat 1
#else
# define BOBUI_FEATURE_linkat -1
#endif
#define BOBUI_FEATURE_lttng -1
#define BOBUI_FEATURE_memmem -1
#define BOBUI_FEATURE_memrchr -1
#define BOBUI_NO_QOBJECT
#define BOBUI_FEATURE_permissions -1
#define BOBUI_FEATURE_process -1
#define BOBUI_FEATURE_regularexpression 1
#ifdef __GLIBC_PREREQ
# define BOBUI_FEATURE_renameat2 (__GLIBC_PREREQ(2, 28) ? 1 : -1)
#else
# define BOBUI_FEATURE_renameat2 -1
#endif
#define BOBUI_FEATURE_shortcut -1
#define BOBUI_FEATURE_slog2 -1
#define BOBUI_FEATURE_syslog -1
#define BOBUI_NO_SYSTEMLOCALE
#define BOBUI_FEATURE_temporaryfile -1
#define BOBUI_FEATURE_textdate 1
#undef BOBUI_FEATURE_thread
#define BOBUI_FEATURE_thread -1
#define BOBUI_FEATURE_timezone -1
#define BOBUI_FEATURE_topleveldomain -1
#define BOBUI_NO_TRANSLATION
#define BOBUI_FEATURE_translation -1
#define BOBUI_NO_VARIANT -1
#define BOBUI_FEATURE_winsdkicu -1

#endif // BOBUI_BOOTSTRAPPED
