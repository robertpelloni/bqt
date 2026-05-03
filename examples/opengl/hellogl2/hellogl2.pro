HEADERS       = glwidget.h \
                window.h \
                mainwindow.h \
                logo.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp \
                logo.cpp

BOBUI += widgets opengl openglwidgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/hellogl2
INSTALLS += target
