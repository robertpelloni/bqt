requires(bobuiHaveModule(widgets))

TEMPLATE      = subdirs
CONFIG += no_docs_target

SUBDIRS       = \
                animation \
                desktop \
                dialogs \
                draganddrop \
                gallery \
                gestures \
                graphicsview \
                itemviews \
                layouts \
                mainwindows \
                painting \
                rhi \
                richtext \
                tools \
                touch \
                tutorials \
                widgets

contains(DEFINES, BOBUI_NO_CURSOR): SUBDIRS -= mainwindows
!bobuiConfig(draganddrop): SUBDIRS -= draganddrop
!bobuiConfig(animation): SUBDIRS -= animation
