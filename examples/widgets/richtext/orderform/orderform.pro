BOBUI += widgets
requires(bobuiConfig(tablewidget))
bobuiHaveModule(printsupport): BOBUI += printsupport

HEADERS     = detailsdialog.h \
              mainwindow.h
SOURCES     = detailsdialog.cpp \
              main.cpp \
              mainwindow.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/richtext/orderform
INSTALLS += target
