BOBUI += widgets
requires(bobuiConfig(treeview))
bobuiHaveModule(printsupport): BOBUI += printsupport
#unix:bobuiHaveModule(dbus): BOBUI += dbus widgets

HEADERS += printview.h spreadsheet.h spreadsheetdelegate.h spreadsheetitem.h
SOURCES += main.cpp \
           printview.cpp \
           spreadsheet.cpp \
           spreadsheetdelegate.cpp \
           spreadsheetitem.cpp
RESOURCES += spreadsheet.qrc

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/spreadsheet
INSTALLS += target

