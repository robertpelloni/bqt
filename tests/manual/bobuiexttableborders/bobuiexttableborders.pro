#This project can be used to verify BOBUIBUG-36152 case.
BOBUI       += core gui printsupport widgets
CONFIG   += c++11
TARGET = bobuiexttableborders
TEMPLATE = app
SOURCES  += main.cpp widget.cpp
HEADERS  += widget.h
FORMS    += widget.ui
RESOURCES += resources.qrc
