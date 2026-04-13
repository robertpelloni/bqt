# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_commandline_option(libproxy TYPE boolean)
bobui_commandline_option(dtls TYPE boolean)
bobui_commandline_option(ocsp TYPE boolean)
bobui_commandline_option(sctp TYPE boolean)
bobui_commandline_option(securetransport TYPE boolean)
bobui_commandline_option(schannel TYPE boolean)
bobui_commandline_option(ssl TYPE boolean)
bobui_commandline_option(system-proxies TYPE boolean)
bobui_commandline_option(publicsuffix TYPE optionalString VALUES system bobui no all)
