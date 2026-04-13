BOBUI += opengl

HEADERS += paintedwindow.h
SOURCES += paintedwindow.cpp main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/paintedwindow
INSTALLS += target
