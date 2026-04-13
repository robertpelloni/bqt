BOBUI_FOR_CONFIG += widgets
requires(bobuiConfig(inputdialog))

TEMPLATE = subdirs
SUBDIRS = plugins app

app.depends = plugins
