TEMPLATE = subdirs
SUBDIRS = \
    cbordump \
    convert \
    savegame

bobuiHaveModule(widgets) {
    SUBDIRS +=  streambookmarks
}
