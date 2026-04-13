BOBUI += widgets
requires(bobuiConfig(filedialog))
bobuiHaveModule(printsupport): BOBUI += printsupport

HEADERS       = mainwindow.h \
                scribblearea.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                scribblearea.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/scribble
INSTALLS += target
