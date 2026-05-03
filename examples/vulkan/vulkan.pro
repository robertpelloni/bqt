TEMPLATE = subdirs

SUBDIRS = hellovulkantriangle

bobuiHaveModule(widgets) {
    SUBDIRS += hellovulkanwidget
    bobuiHaveModule(concurrent): SUBDIRS += hellovulkancubes
}
