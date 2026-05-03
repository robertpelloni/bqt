BOBUI += widgets
requires(bobuiConfig(completer))

HEADERS   = treemodelcompleter.h \
            mainwindow.h
SOURCES   = treemodelcompleter.cpp \
            main.cpp \
            mainwindow.cpp
RESOURCES = treemodelcompleter.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tools/treemodelcompleter
INSTALLS += target
