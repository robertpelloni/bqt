BOBUI += widgets
requires(bobuiConfig(listview))

SOURCES   = adddialog.cpp \
            addresswidget.cpp \
            main.cpp \
            mainwindow.cpp \
            newaddresstab.cpp \
            tablemodel.cpp
HEADERS   = adddialog.h \
            addresswidget.h \
            mainwindow.h \
            newaddresstab.h \
            tablemodel.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/addressbook
INSTALLS += target
