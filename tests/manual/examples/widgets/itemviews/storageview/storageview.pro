BOBUI += core gui widgets
requires(bobuiConfig(treeview))
TARGET = storageview
TEMPLATE = app
SOURCES += storagemodel.cpp \
    main.cpp
HEADERS += \
    storagemodel.h

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/storageview
INSTALLS += target
