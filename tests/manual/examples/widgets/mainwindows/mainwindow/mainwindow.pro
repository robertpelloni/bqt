TEMPLATE = app
BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS += colorswatch.h mainwindow.h toolbar.h
SOURCES += colorswatch.cpp mainwindow.cpp toolbar.cpp main.cpp
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

#! [qrc]
RESOURCES += mainwindow.qrc
#! [qrc]

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/mainwindows/mainwindow
INSTALLS += target
