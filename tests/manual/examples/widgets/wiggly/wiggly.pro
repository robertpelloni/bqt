BOBUI += widgets

HEADERS       = wigglywidget.h \
                dialog.h
SOURCES       = wigglywidget.cpp \
                dialog.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/wiggly
INSTALLS += target
