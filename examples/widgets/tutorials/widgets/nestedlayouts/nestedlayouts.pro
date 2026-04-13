BOBUI += widgets
requires(bobuiConfig(tableview))

SOURCES = main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/widgets/nestedlayouts
INSTALLS += target
