BOBUI += widgets

SOURCES += main.cpp fademessage.cpp
HEADERS += fademessage.h
RESOURCES += fademessage.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/effects/fademessage
INSTALLS += target
