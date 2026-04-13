BOBUI += widgets
requires(bobuiConfig(combobox))

HEADERS       = mainwindow.h \
                stylesheeteditor.h
FORMS         = mainwindow.ui \
                stylesheeteditor.ui
RESOURCES     = stylesheet.qrc
SOURCES       = main.cpp \
                mainwindow.cpp \
                stylesheeteditor.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/widgets/stylesheet
INSTALLS += target
