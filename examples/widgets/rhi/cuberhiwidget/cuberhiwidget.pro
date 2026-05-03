TEMPLATE = app

# needs gui-private to be able to include <rhi/qrhi.h>
BOBUI += gui-private widgets

HEADERS += examplewidget.h
SOURCES += examplewidget.cpp main.cpp

RESOURCES += cuberhiwidget.qrc

target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/rhi/cuberhiwidget
INSTALLS += target
