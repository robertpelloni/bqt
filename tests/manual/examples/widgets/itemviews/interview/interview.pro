TEMPLATE = app
BOBUI += widgets
requires(bobuiConfig(treeview))

HEADERS += model.h
SOURCES += model.cpp main.cpp
RESOURCES += interview.qrc

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/interview
INSTALLS += target
