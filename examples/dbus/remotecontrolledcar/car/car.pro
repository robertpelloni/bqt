BOBUI += dbus widgets

DBUS_ADAPTORS += ../common/car.xml
HEADERS += car.h
SOURCES += car.cpp main.cpp

CONFIG += no_batch  # work around BOBUIBUG-96513

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/dbus/remotecontrolledcar/car
INSTALLS += target
