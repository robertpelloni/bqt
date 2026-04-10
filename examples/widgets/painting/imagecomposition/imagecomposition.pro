BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = imagecomposer.h
SOURCES       = imagecomposer.cpp \
                main.cpp
RESOURCES     = imagecomposition.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/imagecomposition
INSTALLS += target
