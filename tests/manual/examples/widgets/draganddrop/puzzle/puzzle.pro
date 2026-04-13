BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS     = mainwindow.h \
              pieceslist.h \
              puzzlewidget.h
RESOURCES   = puzzle.qrc
SOURCES     = main.cpp \
              mainwindow.cpp \
              pieceslist.cpp \
              puzzlewidget.cpp

QMAKE_PROJECT_NAME = dndpuzzle

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/draganddrop/puzzle
INSTALLS += target
