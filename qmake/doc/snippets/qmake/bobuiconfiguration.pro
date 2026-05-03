# Show information about the BobUI installation.
#! [0]
message(BobUI version: $$[BOBUI_VERSION])
message(BobUI is installed in $$[BOBUI_INSTALL_PREFIX])
message(BobUI resources can be found in the following locations:)
message(Documentation: $$[BOBUI_INSTALL_DOCS])
message(Header files: $$[BOBUI_INSTALL_HEADERS])
message(Libraries: $$[BOBUI_INSTALL_LIBS])
message(Binary files (executables): $$[BOBUI_INSTALL_BINS])
message(Plugins: $$[BOBUI_INSTALL_PLUGINS])
message(Data files: $$[BOBUI_INSTALL_DATA])
message(Translation files: $$[BOBUI_INSTALL_TRANSLATIONS])
message(Settings: $$[BOBUI_INSTALL_CONFIGURATION])
message(Examples: $$[BOBUI_INSTALL_EXAMPLES])
#! [0]

# Show configuration information.
message(CONFIG = $$CONFIG)
