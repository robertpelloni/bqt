#This project can be used to verify BOBUIBUG-5111 case.
BOBUI       += core gui
BOBUI += widgets
TARGET = bobuiexteditlist
TEMPLATE = app
SOURCES  += main.cpp widget.cpp
HEADERS  += widget.h
FORMS    += widget.ui
