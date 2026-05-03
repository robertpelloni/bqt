BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS     = movieplayer.h
SOURCES     = main.cpp \
              movieplayer.cpp

EXAMPLE_FILES = animation.gif

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/movie
INSTALLS += target
