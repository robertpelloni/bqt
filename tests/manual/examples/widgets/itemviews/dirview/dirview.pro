BOBUI += widgets
requires(bobuiConfig(treeview))

SOURCES       = main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/dirview
INSTALLS += target
