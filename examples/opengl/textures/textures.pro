HEADERS       = glwidget.h \
                window.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp

RESOURCES     = textures.qrc

BOBUI += widgets opengl openglwidgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/textures
INSTALLS += target
