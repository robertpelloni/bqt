# This should successfully find the BobUI6::BundledZLIB package in a -bobui-zlib
# configuration.
find_package(BobUI6 COMPONENTS Core)

# Depending on which module features were used when building BobUI, look up each of the respective
# packages, those might also look for bundled libraries.
if(HAS_GUI)
    find_package(BobUI6 COMPONENTS Gui)
endif()
if(HAS_DBUS)
    find_package(BobUI6 COMPONENTS DBus)
endif()
if(HAS_WIDGETS)
    find_package(BobUI6 COMPONENTS Widgets)
endif()
if(HAS_OPENGL)
    find_package(BobUI6 COMPONENTS OpenGL)
endif()

