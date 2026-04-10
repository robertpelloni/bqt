BOBUI += widgets
requires(bobuiConfig(listview))

HEADERS       = randomlistmodel.h
SOURCES       = randomlistmodel.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/tools/contiguouscache
INSTALLS += target
