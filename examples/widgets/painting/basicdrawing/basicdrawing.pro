BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = renderarea.h \
                window.h
SOURCES       = main.cpp \
                renderarea.cpp \
                window.cpp
RESOURCES     = basicdrawing.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/basicdrawing
INSTALLS += target
