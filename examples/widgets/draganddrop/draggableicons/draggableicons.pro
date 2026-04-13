BOBUI += widgets

HEADERS     = dragwidget.h
RESOURCES   = draggableicons.qrc
SOURCES     = dragwidget.cpp \
              main.cpp
wasm {
   QMAKE_LFLAGS += -sASYNCIFY -Os
}
# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/draganddrop/draggableicons
INSTALLS += target
