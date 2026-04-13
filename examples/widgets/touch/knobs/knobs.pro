BOBUI += widgets

HEADERS = knob.h
SOURCES = main.cpp knob.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/touch/knobs
INSTALLS += target
