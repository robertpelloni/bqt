HEADERS += rsslisting.h
SOURCES += main.cpp rsslisting.cpp
BOBUI += network widgets
requires(bobuiConfig(treewidget))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/network/rsslisting
INSTALLS += target
