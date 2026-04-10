BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS     = slidersgroup.h \
              window.h
SOURCES     = main.cpp \
              slidersgroup.cpp \
              window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/sliders
INSTALLS += target
