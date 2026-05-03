BOBUI += widgets

HEADERS     = flowlayout.h \
              window.h
SOURCES     = flowlayout.cpp \
              main.cpp \
              window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/layouts/flowlayout
INSTALLS += target
