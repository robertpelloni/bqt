BOBUI += widgets

SOURCES = addressbook.cpp \
          main.cpp
HEADERS = addressbook.h

QMAKE_PROJECT_NAME = ab_part3

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tutorials/addressbook/part3
INSTALLS += target
