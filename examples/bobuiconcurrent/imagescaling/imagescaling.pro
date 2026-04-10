BOBUI += concurrent widgets network
CONFIG += exceptions
requires(bobuiConfig(filedialog))

SOURCES += main.cpp imagescaling.cpp \
    downloaddialog.cpp
HEADERS += imagescaling.h \
    downloaddialog.h

target.path = $$[BOBUI_INSTALL_EXAMPLES]/bobuiconcurrent/imagescaling
INSTALLS += target

FORMS += \
    downloaddialog.ui
