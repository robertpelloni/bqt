RESOURCES += images.qrc

HEADERS += mainwindow.h view.h chip.h
SOURCES += main.cpp
SOURCES += mainwindow.cpp view.cpp chip.cpp

BOBUI += widgets
bobuiHaveModule(printsupport): BOBUI += printsupport

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/graphicsview/chip
INSTALLS += target

