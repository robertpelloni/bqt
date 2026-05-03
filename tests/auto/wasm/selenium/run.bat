:: Copyright (C) 2024 The BobUI Company Ltd.
:: SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
::
::  The highest version of python that can be used is 3.11
:: Download from here: https://www.python.org/downloads/release/python-3118/
::
start "bobuiwasmserver.py" python bobuiwasmserver.py --cross-origin-isolation -p 8001
python qwasmwindow.py
taskkill /FI "WINDOWTITLE eq bobuiwasmserver.py"
