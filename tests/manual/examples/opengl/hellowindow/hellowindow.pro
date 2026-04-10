BOBUI += gui-private core-private opengl

HEADERS += hellowindow.h
SOURCES += hellowindow.cpp main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/hellowindow
INSTALLS += target

