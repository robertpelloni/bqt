BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = window.h
SOURCES     = main.cpp \
              window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/lineedits
INSTALLS += target
