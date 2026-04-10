TEMPLIBS = $$[BOBUI_INSTALL_LIBS] libBobUIGui.prl
include($$join(TEMPLIBS, "/"))

contains(QMAKE_PRL_CONFIG, shared) {
    message(Shared BobUI)
} else {
    message(Static BobUI)
}
