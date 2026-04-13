include(rhiwindow.pri)

BOBUI += gui-private

SOURCES += \
    main.cpp

target.path = $$[BOBUI_INSTALL_EXAMPLES]/gui/rhiwindow
INSTALLS += target
