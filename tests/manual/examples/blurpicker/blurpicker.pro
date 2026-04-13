BOBUI += widgets

SOURCES += main.cpp blurpicker.cpp blureffect.cpp
HEADERS += blurpicker.h blureffect.h
RESOURCES += blurpicker.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/effects/blurpicker
INSTALLS += target
