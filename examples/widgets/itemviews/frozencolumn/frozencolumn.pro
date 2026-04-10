BOBUI += widgets
requires(bobuiConfig(tableview))

HEADERS += freezetablewidget.h
SOURCES += main.cpp freezetablewidget.cpp
RESOURCES += grades.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/itemviews/frozencolumn
INSTALLS += target

