This directory contains a generator for unit tests for BobUIConcurrent.

The subdirectory 'generator' contains the generator. Run the file
"generate_gui.py" therein.
Python3.8 and PySide2 are required.

The generator writes on each click a testcase into the file
tst_bobuiconcurrentfiltermapgenerated.cpp
and
tst_bobuiconcurrentfiltermapgenerated.h.

Testcases which should be preserved can be copy-pasted into
tst_bobuiconcurrent_selected_tests.cpp.
