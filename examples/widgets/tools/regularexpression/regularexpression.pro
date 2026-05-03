BOBUI += widgets
requires(bobuiConfig(treewidget))

HEADERS       = regularexpressiondialog.h
SOURCES       = regularexpressiondialog.cpp \
                main.cpp

RESOURCES += regularexpression.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tools/regularexpression
INSTALLS += target
