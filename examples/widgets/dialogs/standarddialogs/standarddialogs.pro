BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS       = dialog.h
SOURCES       = dialog.cpp \
                main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/dialogs/standarddialogs
INSTALLS += target
