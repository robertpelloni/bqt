BOBUI += core gui widgets
bobuiHaveModule(printsupport): BOBUI += printsupport

TARGET = dialogs
TEMPLATE = app

SOURCES += main.cpp filedialogpanel.cpp colordialogpanel.cpp fontdialogpanel.cpp \
    wizardpanel.cpp messageboxpanel.cpp utils.cpp
HEADERS += filedialogpanel.h colordialogpanel.h fontdialogpanel.h \
    wizardpanel.h messageboxpanel.h utils.h

!contains(DEFINES, BOBUI_NO_PRINTER) {
    SOURCES += printdialogpanel.cpp
    HEADERS += printdialogpanel.h
    FORMS += printdialogpanel.ui
}
