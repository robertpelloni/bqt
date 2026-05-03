BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS	    =	mainwindow.h \
        tabletcanvas.h \
        tabletapplication.h
SOURCES	    =	mainwindow.cpp \
        main.cpp \
        tabletcanvas.cpp \
        tabletapplication.cpp
RESOURCES += images.qrc

# Avoid naming the target "tablet", as it would create an executable
# named "tablet.exe" on Windows and trigger a bug (in the Wacom drivers, apparently)
# preventing tablet messages from being received.
TARGET = bobuitablet

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/tablet
INSTALLS += target
