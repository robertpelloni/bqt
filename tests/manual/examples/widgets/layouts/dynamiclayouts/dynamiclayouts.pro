BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = dialog.h
SOURCES     = dialog.cpp \
              main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/layouts/dynamiclayouts
INSTALLS += target
