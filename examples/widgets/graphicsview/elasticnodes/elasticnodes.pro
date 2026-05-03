BOBUI += widgets

HEADERS += \
        edge.h \
        node.h \
        graphwidget.h

SOURCES += \
        edge.cpp \
        main.cpp \
        node.cpp \
        graphwidget.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target
