BOBUI += widgets opengl openglwidgets

SOURCES += main.cpp \
           glwidget.cpp \
           mainwindow.cpp

HEADERS += glwidget.h \
           mainwindow.h

target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/stereoqopenglwidget
INSTALLS += target
