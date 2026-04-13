HEADERS = ../connection.h \
          imageitem.h \
          informationwindow.h \
          view.h
RESOURCES = drilldown.qrc
SOURCES = imageitem.cpp \
          informationwindow.cpp \
          main.cpp \
          view.cpp
BOBUI += sql widgets
requires(bobuiConfig(combobox))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/drilldown
INSTALLS += target
