SOURCES     = windowcontainer.cpp

BOBUI += widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/windowcontainer
INSTALLS += target

include(../../opengl/openglwindow/openglwindow.pri)
