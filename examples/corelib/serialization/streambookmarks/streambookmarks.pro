HEADERS       = mainwindow.h \
                xbelreader.h \
                xbelwriter.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                xbelreader.cpp \
                xbelwriter.cpp
BOBUI           += widgets
requires(bobuiConfig(filedialog))

EXAMPLE_FILES = jennifer.xbel

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/serialization/streambookmarks
INSTALLS += target
