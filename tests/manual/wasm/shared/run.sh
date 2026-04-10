#! /bin/bash

# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -m

function removeServer()
{
    kill $cleanupPid
}

if [ -z "$1"]
then
    echo "Usage: $0 testname, where testname is a test in the tests/manual/wasm directory" >&2
    exit 1
fi

trap removeServer EXIT

script_dir=`dirname ${BASH_SOURCE[0]}`
cd "$script_dir/../../../../"
python3 util/wasm/bobuiwasmserver/bobuiwasmserver.py -p 8001 &
cleanupPid=$!
cd -

python3 -m webbrowser "http://localhost:8001/tests/manual/wasm/$1/tst_$1.html"

echo 'Press any key to continue...' >&2
read -n 1
