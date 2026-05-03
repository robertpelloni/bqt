TEMPLATE=subdirs
BOBUI_FOR_CONFIG += network-private gui-private

TEMPLATE = subdirs
BOBUI_FOR_CONFIG += widgets-private

SUBDIRS = \
bobui-client-raster \
bobui-client-widget

bobuiHaveModule(widgets):bobuiConfig(gtk3): SUBDIRS += gtk-container
