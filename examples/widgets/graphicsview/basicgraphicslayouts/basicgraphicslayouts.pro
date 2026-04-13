BOBUI += widgets

HEADERS   = layoutitem.h \
            window.h
SOURCES   = layoutitem.cpp \
            main.cpp \
            window.cpp
RESOURCES = basicgraphicslayouts.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/graphicsview/basicgraphicslayouts
INSTALLS += target
