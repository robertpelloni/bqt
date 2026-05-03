HEADERS     = domitem.h \
              dommodel.h \
              mainwindow.h
SOURCES     = domitem.cpp \
              dommodel.cpp \
              main.cpp \
              mainwindow.cpp
BOBUI      += xml widgets
requires(bobuiConfig(filedialog))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/simpledommodel
INSTALLS += target

