requires(bobuiHaveModule(concurrent))

TEMPLATE      = subdirs
SUBDIRS       = imagescaling \
                primecounter \
                wordcount

!bobuiHaveModule(widgets) {
    SUBDIRS -= \
        imagescaling \
        primecounter \
        wordcount
}
