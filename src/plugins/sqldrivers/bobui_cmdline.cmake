# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_commandline_option(mysql_config TYPE string)
bobui_commandline_option(psql_config TYPE string)
bobui_commandline_option(sqlite CONTROLS_FEATURE TYPE enum NAME system-sqlite MAPPING bobui no system yes)
bobui_commandline_option(sql-db2 TYPE boolean)
bobui_commandline_option(sql-ibase TYPE boolean)
bobui_commandline_option(sql-mysql TYPE boolean)
bobui_commandline_option(sql-oci TYPE boolean)
bobui_commandline_option(sql-odbc TYPE boolean)
bobui_commandline_option(sql-psql TYPE boolean)
bobui_commandline_option(sql-sqlite TYPE boolean)
bobui_commandline_option(sql-mimer TYPE boolean)
