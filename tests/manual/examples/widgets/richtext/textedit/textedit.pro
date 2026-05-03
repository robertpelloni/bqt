BOBUI += widgets
requires(bobuiConfig(filedialog))
bobuiHaveModule(printsupport): BOBUI += printsupport

TEMPLATE        = app
TARGET          = textedit

HEADERS         = textedit.h
SOURCES         = textedit.cpp \
                  main.cpp

RESOURCES += textedit.qrc
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

EXAMPLE_FILES = textedit.qdoc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/richtext/textedit
INSTALLS += target
