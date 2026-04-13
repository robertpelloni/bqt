BOBUI += concurrent widgets

SOURCES += main.cpp primecounter.cpp
HEADERS += primecounter.h

target.path = $$[BOBUI_INSTALL_EXAMPLES]/bobuiconcurrent/primecounter
INSTALLS += target

FORMS += primecounter.ui
