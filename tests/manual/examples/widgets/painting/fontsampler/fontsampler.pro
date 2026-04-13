BOBUI += widgets
requires(bobuiConfig(combobox))
bobuiHaveModule(printsupport): BOBUI += printsupport

FORMS     = mainwindowbase.ui
HEADERS   = mainwindow.h
SOURCES   = main.cpp \
            mainwindow.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/fontsampler
INSTALLS += target
