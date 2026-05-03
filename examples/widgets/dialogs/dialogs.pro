BOBUI_FOR_CONFIG += widgets

TEMPLATE      = subdirs
SUBDIRS       = licensewizard \
                standarddialogs \
                tabdialog \
                trivialwizard

!bobuiHaveModule(printsupport): SUBDIRS -= licensewizard
!bobuiConfig(wizard) {
    SUBDIRS -= trivialwizard licensewizard classwizard
}
