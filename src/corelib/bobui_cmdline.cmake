# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_commandline_option(doubleconversion TYPE enum VALUES no bobui system)
bobui_commandline_option(glib TYPE boolean)
bobui_commandline_option(icu TYPE boolean)
bobui_commandline_option(inotify TYPE boolean)
bobui_commandline_option(jemalloc TYPE boolean)
bobui_commandline_option(journald TYPE boolean)
bobui_commandline_option(libb2 TYPE enum VALUES no bobui system)
bobui_commandline_option(mimetype-database TYPE boolean)
bobui_commandline_option(mimetype-database-compression TYPE optionalString VALUES zstd gzip none)
bobui_commandline_option(pcre TYPE enum VALUES no bobui system)
bobui_commandline_option(posix-ipc TYPE boolean NAME ipc_posix)
bobui_commandline_option(pps TYPE boolean NAME qqnx_pps)
bobui_commandline_option(slog2 TYPE boolean)
bobui_commandline_option(syslog TYPE boolean)
bobui_commandline_option(trace TYPE optionalString VALUES etw lttng ctf no yes)
