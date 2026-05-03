# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs



#### Libraries

if(NOT APPLE AND BUILD_SHARED_LIBS)
    set(mark_cups_optional MARK_OPTIONAL)
else()
    set(mark_cups_optional "")
endif()
bobui_find_package(Cups PROVIDED_TARGETS Cups::Cups
    MODULE_NAME printsupport QMAKE_LIB cups ${mark_cups_optional})

#### Tests



#### Features

bobui_feature("cups" PUBLIC PRIVATE
    SECTION "Painting"
    LABEL "CUPS"
    PURPOSE "Provides support for the Common Unix Printing System."
    CONDITION Cups_FOUND AND BOBUI_FEATURE_printer AND BOBUI_FEATURE_datestring
)
bobui_feature_definition("cups" "BOBUI_NO_CUPS" NEGATE VALUE "1")
bobui_feature("cupsjobwidget" PUBLIC PRIVATE
    SECTION "Widgets"
    LABEL "CUPS job control widget"
    CONDITION ( BOBUI_FEATURE_buttongroup ) AND ( BOBUI_FEATURE_calendarwidget ) AND ( BOBUI_FEATURE_checkbox ) AND ( BOBUI_FEATURE_combobox ) AND ( BOBUI_FEATURE_cups ) AND ( BOBUI_FEATURE_datetimeedit ) AND ( BOBUI_FEATURE_groupbox ) AND ( BOBUI_FEATURE_tablewidget )
)
bobui_feature_definition("cupsjobwidget" "BOBUI_NO_CUPSJOBWIDGET" NEGATE VALUE "1")
bobui_feature("cupspassworddialog" PRIVATE
    SECTION "Widgets"
    LABEL "CUPS password dialog"
    CONDITION ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_formlayout ) AND ( BOBUI_FEATURE_lineedit )
)
bobui_feature("printer" PUBLIC
    SECTION "Painting"
    LABEL "QPrinter"
    PURPOSE "Provides a printer backend of QPainter."
    CONDITION BOBUI_FEATURE_picture AND BOBUI_FEATURE_temporaryfile AND BOBUI_FEATURE_pdf
)
bobui_feature_definition("printer" "BOBUI_NO_PRINTER" NEGATE VALUE "1")
bobui_feature("printpreviewwidget" PUBLIC
    SECTION "Widgets"
    LABEL "QPrintPreviewWidget"
    PURPOSE "Provides a widget for previewing page layouts for printer output."
    CONDITION BOBUI_FEATURE_graphicsview AND BOBUI_FEATURE_printer AND BOBUI_FEATURE_mainwindow
)
bobui_feature_definition("printpreviewwidget" "BOBUI_NO_PRINTPREVIEWWIDGET" NEGATE VALUE "1")
bobui_feature("printdialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QPrintDialog"
    PURPOSE "Provides a dialog widget for specifying printer configuration."
    CONDITION ( BOBUI_FEATURE_buttongroup ) AND ( BOBUI_FEATURE_checkbox ) AND ( BOBUI_FEATURE_combobox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_datetimeedit ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_formlayout ) AND ( BOBUI_FEATURE_printer ) AND ( BOBUI_FEATURE_radiobutton ) AND ( BOBUI_FEATURE_spinbox ) AND ( BOBUI_FEATURE_tabwidget ) AND ( BOBUI_FEATURE_treeview )
)
bobui_feature_definition("printdialog" "BOBUI_NO_PRINTDIALOG" NEGATE VALUE "1")
bobui_feature("printpreviewdialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QPrintPreviewDialog"
    PURPOSE "Provides a dialog for previewing and configuring page layouts for printer output."
    CONDITION BOBUI_FEATURE_printpreviewwidget AND BOBUI_FEATURE_printdialog AND BOBUI_FEATURE_toolbar AND BOBUI_FEATURE_formlayout
)
bobui_feature_definition("printpreviewdialog" "BOBUI_NO_PRINTPREVIEWDIALOG" NEGATE VALUE "1")
bobui_configure_add_summary_section(NAME "BobUI PrintSupport")
bobui_configure_add_summary_entry(ARGS "cups")
bobui_configure_end_summary_section() # end of "BobUI PrintSupport" section
