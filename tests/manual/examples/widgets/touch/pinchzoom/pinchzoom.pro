BOBUI += widgets

HEADERS += \
        mouse.h \
        graphicsview.h
SOURCES += \
	main.cpp \
        mouse.cpp \
        graphicsview.cpp

RESOURCES += \
	mice.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/touch/pinchzoom
INSTALLS += target
