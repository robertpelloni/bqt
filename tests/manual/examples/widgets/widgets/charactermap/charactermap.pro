BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = characterwidget.h \
              mainwindow.h
SOURCES     = characterwidget.cpp \
              mainwindow.cpp \
              main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/charactermap
INSTALLS += target
