TEMPLATE      = subdirs
CONFIG += no_docs_target

SUBDIRS       = semaphores \
                waitconditions

bobuiHaveModule(widgets): SUBDIRS += \
    mandelbrot \
    queuedcustomtype
