BOBUI += widgets
requires(bobuiConfig(filedialog))

SOURCES   = addressbook.cpp \
            finddialog.cpp \
            main.cpp
HEADERS   = addressbook.h \
            finddialog.h

QMAKE_PROJECT_NAME = ab_part6

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/addressbook/part6
INSTALLS += target
