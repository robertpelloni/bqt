BOBUI += widgets
requires(bobuiConfig(listview))

HEADERS     = mainwindow.h \
              piecesmodel.h \
              puzzlewidget.h
RESOURCES   = puzzle.qrc
SOURCES     = main.cpp \
              mainwindow.cpp \
              piecesmodel.cpp \
              puzzlewidget.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/puzzle
INSTALLS += target
