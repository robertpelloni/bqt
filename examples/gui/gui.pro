requires(bobuiHaveModule(gui))

TEMPLATE     = subdirs
BOBUI_FOR_CONFIG += gui
CONFIG += no_docs_target

SUBDIRS += rasterwindow \
           rhiwindow
