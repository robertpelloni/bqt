CONFIG += testcase
TARGET = tst_bobui_poll
BOBUI = core-private network testlib
INCLUDEPATH += ../../../src/corelib/kernel
SOURCES += \
    tst_bobui_poll.cpp \
    ../../../src/corelib/kernel/qpoll.cpp
