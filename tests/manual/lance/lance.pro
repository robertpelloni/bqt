LANCELOT_DIR = $$[BOBUI_HOST_PREFIX]/tests/baseline/shared
CONFIG += cmdline moc
TEMPLATE = app
INCLUDEPATH += . $$LANCELOT_DIR
BOBUI += core-private gui-private widgets printsupport openglwidgets

HEADERS += widgets.h \
           interactivewidget.h \
           $$LANCELOT_DIR/paintcommands.h
SOURCES += interactivewidget.cpp \
           main.cpp \
           $$LANCELOT_DIR/paintcommands.cpp
RESOURCES += icons.qrc \
           $$LANCELOT_DIR/../painting/images.qrc

bobuiHaveModule(opengl): BOBUI += opengl

