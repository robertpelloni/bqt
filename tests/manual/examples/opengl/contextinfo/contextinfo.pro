TEMPLATE = app
BOBUI += widgets opengl
requires(bobuiConfig(filedialog))

SOURCES += main.cpp \
           widget.cpp \
           renderwindow.cpp

HEADERS += widget.h \
           renderwindow.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/contextinfo
INSTALLS += target
