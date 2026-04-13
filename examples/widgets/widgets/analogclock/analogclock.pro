BOBUI += widgets

HEADERS       = analogclock.h
SOURCES       = analogclock.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target
