BOBUI += widgets

HEADERS       = shapedclock.h
SOURCES       = shapedclock.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/shapedclock
INSTALLS += target
