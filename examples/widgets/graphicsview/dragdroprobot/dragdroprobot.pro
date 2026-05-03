BOBUI += widgets

HEADERS += \
	coloritem.h \
	robot.h

SOURCES += \
	coloritem.cpp \
	main.cpp \
	robot.cpp

RESOURCES += \
	robot.qrc


# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/graphicsview/dragdroprobot
INSTALLS += target
