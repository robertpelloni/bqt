BOBUI += widgets

HEADERS       = mainwindow.h
SOURCES       = mainwindow.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/mainwindows/menus
INSTALLS += target
