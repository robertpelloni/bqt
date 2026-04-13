BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS   = imagewidget.h \
            mainwidget.h
SOURCES   = imagewidget.cpp \
            main.cpp \
            mainwidget.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/gestures/imagegestures
INSTALLS += target

