HEADERS       = mainwindow.h \
                xbeltree.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                xbeltree.cpp
BOBUI           += xml widgets
requires(bobuiConfig(filedialog))

EXAMPLE_FILES = jennifer.xbel

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/xml/dombookmarks
INSTALLS += target
