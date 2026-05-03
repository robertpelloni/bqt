BOBUI += widgets

HEADERS       = digitalclock.h
SOURCES       = digitalclock.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/digitalclock
INSTALLS += target
