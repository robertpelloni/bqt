BOBUI += widgets
requires(bobuiConfig(fontcombobox))

SOURCES += main.cpp
SOURCES += customproxy.cpp embeddeddialog.cpp
HEADERS += customproxy.h embeddeddialog.h

FORMS += embeddeddialog.ui
RESOURCES += embeddeddialogs.qrc

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/graphicsview/embeddeddialogs
INSTALLS += target
