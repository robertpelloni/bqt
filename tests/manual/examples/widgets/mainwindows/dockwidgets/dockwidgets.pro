BOBUI += widgets
requires(bobuiConfig(listwidget))
bobuiHaveModule(printsupport): BOBUI += printsupport

HEADERS         = mainwindow.h
SOURCES         = main.cpp \
                  mainwindow.cpp
RESOURCES       = dockwidgets.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/mainwindows/dockwidgets
INSTALLS += target
