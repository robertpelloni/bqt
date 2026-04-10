SOURCES += tst_lexgen.cpp
TARGET = tst_lexgen
include(../lexgen.pri)
BOBUI = core testlib
DEFINES += SRCDIR=\\\"$$PWD\\\"
