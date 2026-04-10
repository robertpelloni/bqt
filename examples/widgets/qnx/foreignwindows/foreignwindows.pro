TEMPLATE = app

HEADERS += collector.h
SOURCES += main.cpp collector.cpp
LIBS += -lscreen

BOBUI += widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/qnx/foreignwindows
INSTALLS += target
