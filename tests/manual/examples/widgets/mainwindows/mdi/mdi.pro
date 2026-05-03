BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS       = mainwindow.h \
                mdichild.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp
RESOURCES     = mdi.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/mainwindows/mdi
INSTALLS += target
