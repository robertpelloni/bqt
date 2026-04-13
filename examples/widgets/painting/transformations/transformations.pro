BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = renderarea.h \
              window.h
SOURCES     = main.cpp \
              renderarea.cpp \
	      window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/transformations
INSTALLS += target
