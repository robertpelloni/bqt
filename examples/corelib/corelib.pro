TEMPLATE = subdirs
CONFIG += no_docs_target

SUBDIRS = \
    ipc \
    mimetypes \
    serialization \
    tools \
    platform \
    time

bobuiConfig(thread): SUBDIRS += threads
