BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = iconpreviewarea.h \
                iconsizespinbox.h \
                imagedelegate.h \
                mainwindow.h
SOURCES       = iconpreviewarea.cpp \
                iconsizespinbox.cpp \
                imagedelegate.cpp \
                main.cpp \
                mainwindow.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

EXAMPLE_FILES = images/*

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/icons
INSTALLS += target
