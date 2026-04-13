BOBUI += widgets
requires(bobuiConfig(tablewidget))

HEADERS = droparea.h \
          dropsitewindow.h
SOURCES = droparea.cpp \
          dropsitewindow.cpp \
          main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/draganddrop/dropsite
INSTALLS += target
