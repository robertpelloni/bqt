BOBUI += widgets

HEADERS     = draglabel.h \
              dragwidget.h
RESOURCES   = fridgemagnets.qrc
SOURCES     = draglabel.cpp \
              dragwidget.cpp \
              main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/draganddrop/fridgemagnets
INSTALLS += target
