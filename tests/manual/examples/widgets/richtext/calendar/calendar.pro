BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = mainwindow.h
SOURCES     = main.cpp \
              mainwindow.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/richtext/calendar
INSTALLS += target
