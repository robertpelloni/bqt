TARGET = mv_edit

TEMPLATE = app

BOBUI += widgets
requires(bobuiConfig(tableview))

SOURCES += main.cpp \
           mainwindow.cpp \
           mymodel.cpp

HEADERS += mainwindow.h \
           mymodel.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/modelview/5_edit
INSTALLS += target
