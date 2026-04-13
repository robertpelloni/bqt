BOBUI += opengl

HEADERS = $$PWD/glwindow.h

SOURCES = $$PWD/glwindow.cpp \
          $$PWD/main.cpp

RESOURCES += computegles31.qrc

target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/computegles31
INSTALLS += target
