TEMPLATE        = app
TARGET          = sqlbrowser

BOBUI              += sql widgets
requires(bobuiConfig(tableview))

HEADERS         = browser.h connectionwidget.h qsqlconnectiondialog.h
SOURCES         = main.cpp browser.cpp connectionwidget.cpp qsqlconnectiondialog.cpp

FORMS           = browserwidget.ui qsqlconnectiondialog.ui
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/sqlbrowser
INSTALLS += target
