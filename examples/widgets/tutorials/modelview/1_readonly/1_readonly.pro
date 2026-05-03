TARGET =   mv_readonly

TEMPLATE = app

BOBUI += widgets
requires(bobuiConfig(tableview))

SOURCES += main.cpp \
           mymodel.cpp

HEADERS += mymodel.h


# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/modelview/1_readonly
INSTALLS += target
