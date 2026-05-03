BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = widgetgallery.h
SOURCES       = main.cpp \
                widgetgallery.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/gallery
INSTALLS += target
