BOBUI += widgets
requires(bobuiConfig(datetimeedit))

HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/spinboxes
INSTALLS += target
