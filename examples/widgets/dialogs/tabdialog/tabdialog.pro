BOBUI += widgets
requires(bobuiConfig(listwidget))

HEADERS       = tabdialog.h
SOURCES       = main.cpp \
                tabdialog.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/dialogs/tabdialog
INSTALLS += target
