BOBUI += widgets

HEADERS       = mandelbrotwidget.h \
                renderthread.h
SOURCES       = main.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/threads/mandelbrot
INSTALLS += target
