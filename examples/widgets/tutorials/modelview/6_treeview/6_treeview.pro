TARGET = mv_tree
TEMPLATE = app
BOBUI += widgets
requires(bobuiConfig(treeview))
SOURCES += main.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/modelview/6_treeview
INSTALLS += target
