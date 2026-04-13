BOBUI += widgets

HEADERS     = dragwidget.h
RESOURCES   = draggabletext.qrc
SOURCES     = dragwidget.cpp \
              main.cpp
wasm {
   QMAKE_LFLAGS += -sASYNCIFY -Os
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/draganddrop/draggabletext
INSTALLS += target
