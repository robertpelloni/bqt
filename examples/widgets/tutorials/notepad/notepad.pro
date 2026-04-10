TEMPLATE = app
TARGET = notepad

BOBUI += widgets

bobuiHaveModule(printsupport): BOBUI += printsupport
requires(bobuiConfig(fontdialog))

SOURCES += \
    main.cpp\
    notepad.cpp

HEADERS += notepad.h

FORMS += notepad.ui

RESOURCES += \
    notepad.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/notepad
INSTALLS += target

