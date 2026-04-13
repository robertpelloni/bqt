BOBUI += widgets
requires(bobuiConfig(tableview))
bobuiHaveModule(printsupport): BOBUI += printsupport

HEADERS     = imagemodel.h \
              mainwindow.h \
              pixeldelegate.h
SOURCES     = imagemodel.cpp \
              main.cpp \
              mainwindow.cpp \
              pixeldelegate.cpp
RESOURCES   += images.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/pixelator
INSTALLS += target
