BOBUI += widgets

HEADERS       = controllerwindow.h \
                previewwindow.h
SOURCES       = controllerwindow.cpp \
                previewwindow.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/windowflags
INSTALLS += target
