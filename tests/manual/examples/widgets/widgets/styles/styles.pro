BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = norwegianwoodstyle.h \
                widgetgallery.h
SOURCES       = main.cpp \
                norwegianwoodstyle.cpp \
                widgetgallery.cpp
RESOURCES     = styles.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/styles
INSTALLS += target
