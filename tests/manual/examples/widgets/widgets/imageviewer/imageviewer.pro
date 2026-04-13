BOBUI += widgets
requires(bobuiConfig(filedialog))
bobuiHaveModule(printsupport): BOBUI += printsupport

HEADERS       = imageviewer.h
SOURCES       = imageviewer.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/imageviewer
INSTALLS += target
