requires(bobuiHaveModule(widgets))

TEMPLATE      = subdirs

SUBDIRS             =   books \
                        drilldown \
                        cachedtable \
                        querymodel \
                        relationaltablemodel \
                        sqlwidgetmapper \
                        tablemodel

bobuiHaveModule(xml): SUBDIRS += masterdetail

!cross_compile:{
    contains(BOBUI_BUILD_PARTS, tools):{
        SUBDIRS += sqlbrowser
    }
}

EXAMPLE_FILES = connection.h
