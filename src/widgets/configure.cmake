# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs



#### Libraries

bobui_find_package(GTK3 3.6 MODULE
    PROVIDED_TARGETS PkgConfig::GTK3 MODULE_NAME widgets QMAKE_LIB gtk3)


#### Tests



#### Features

bobui_feature("gtk3" PRIVATE
    LABEL "GTK+"
    AUTODETECT NOT APPLE
    CONDITION BOBUI_FEATURE_glib AND GTK3_FOUND
)
bobui_feature("style-fusion" PRIVATE
    LABEL "Fusion"
)
bobui_feature("style-mac" PRIVATE
    LABEL "macOS"
    CONDITION MACOS AND BOBUI_FEATURE_animation
)
bobui_feature("style-windows" PRIVATE
    LABEL "Windows"
)
bobui_feature("style-windowsvista" PRIVATE
    LABEL "WindowsVista"
    CONDITION BOBUI_FEATURE_style_windows AND BOBUI_FEATURE_animation AND WIN32
)
bobui_feature("style-windows11" PRIVATE
    LABEL "Windows11"
    CONDITION BOBUI_FEATURE_style_windows AND BOBUI_FEATURE_animation AND WIN32
)
bobui_feature("style-android" PRIVATE
    LABEL "Android"
    AUTODETECT ANDROID
)
bobui_feature("style-stylesheet" PUBLIC
    SECTION "Styles"
    LABEL "QStyleSheetStyle"
    PURPOSE "Provides a widget style which is configurable via CSS."
    CONDITION BOBUI_FEATURE_style_windows AND BOBUI_FEATURE_cssparser
)
bobui_feature_definition("style-stylesheet" "BOBUI_NO_STYLE_STYLESHEET" NEGATE VALUE "1")
bobui_feature("effects" PRIVATE
    SECTION "Kernel"
    LABEL "Effects"
    PURPOSE "Provides special widget effects (e.g. fading and scrolling)."
)
bobui_feature("itemviews" PUBLIC
    SECTION "ItemViews"
    LABEL "The Model/View Framework"
    PURPOSE "Provides the model/view architecture managing the relationship between data and the way it is presented to the user."
    CONDITION BOBUI_FEATURE_itemmodel AND BOBUI_FEATURE_scrollarea
)
bobui_feature_definition("itemviews" "BOBUI_NO_ITEMVIEWS" NEGATE VALUE "1")
bobui_feature("treewidget" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIreeWidget"
    PURPOSE "Provides views using tree models."
    CONDITION BOBUI_FEATURE_treeview
)
bobui_feature_definition("treewidget" "BOBUI_NO_TREEWIDGET" NEGATE VALUE "1")
bobui_feature("listwidget" PUBLIC
    SECTION "Widgets"
    LABEL "QListWidget"
    PURPOSE "Provides item-based list widgets."
    CONDITION BOBUI_FEATURE_listview
)
bobui_feature_definition("listwidget" "BOBUI_NO_LISTWIDGET" NEGATE VALUE "1")
bobui_feature("tablewidget" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIableWidget"
    PURPOSE "Provides item-based table views."
    CONDITION BOBUI_FEATURE_tableview
)
bobui_feature_definition("tablewidget" "BOBUI_NO_TABLEWIDGET" NEGATE VALUE "1")
bobui_feature("abstractbutton" PUBLIC
    SECTION "Widgets"
    LABEL "QAbstractButton"
    PURPOSE "Abstract base class of button widgets, providing functionality common to buttons."
)
bobui_feature("commandlinkbutton" PUBLIC
    SECTION "Widgets"
    LABEL "QCommandLinkButton"
    PURPOSE "Provides a Vista style command link button."
    CONDITION BOBUI_FEATURE_pushbutton
)
bobui_feature("datetimeedit" PUBLIC
    SECTION "Widgets"
    LABEL "QDateTimeEdit"
    PURPOSE "Supports editing dates and times."
    CONDITION BOBUI_FEATURE_calendarwidget AND BOBUI_FEATURE_datetimeparser
)
bobui_feature_definition("datetimeedit" "BOBUI_NO_DATETIMEEDIT" NEGATE VALUE "1")
bobui_feature("stackedwidget" PUBLIC
    SECTION "Widgets"
    LABEL "QStackedWidget"
    PURPOSE "Provides stacked widgets."
)
bobui_feature_definition("stackedwidget" "BOBUI_NO_STACKEDWIDGET" NEGATE VALUE "1")
bobui_feature("textbrowser" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIextBrowser"
    PURPOSE "Supports HTML document browsing."
    CONDITION BOBUI_FEATURE_textedit
)
bobui_feature_definition("textbrowser" "BOBUI_NO_TEXTBROWSER" NEGATE VALUE "1")
bobui_feature("splashscreen" PUBLIC
    SECTION "Widgets"
    LABEL "QSplashScreen"
    PURPOSE "Supports splash screens that can be shown during application startup."
)
bobui_feature_definition("splashscreen" "BOBUI_NO_SPLASHSCREEN" NEGATE VALUE "1")
bobui_feature("splitter" PUBLIC
    SECTION "Widgets"
    LABEL "QSplitter"
    PURPOSE "Provides user controlled splitter widgets."
)
bobui_feature_definition("splitter" "BOBUI_NO_SPLITTER" NEGATE VALUE "1")
bobui_feature("widgettextcontrol" PRIVATE
    SECTION "Widgets"
    LABEL "QWidgetTextControl"
    PURPOSE "Provides text control functionality to other widgets."
)
bobui_feature("label" PUBLIC
    SECTION "Widgets"
    LABEL "QLabel"
    PURPOSE "Provides a text or image display."
    CONDITION BOBUI_FEATURE_widgettextcontrol
)
bobui_feature("formlayout" PUBLIC
    SECTION "Widgets"
    LABEL "QFormLayout"
    PURPOSE "Manages forms of input widgets and their associated labels."
    CONDITION BOBUI_FEATURE_label
)
bobui_feature("lcdnumber" PUBLIC
    SECTION "Widgets"
    LABEL "QLCDNumber"
    PURPOSE "Provides LCD-like digits."
)
bobui_feature_definition("lcdnumber" "BOBUI_NO_LCDNUMBER" NEGATE VALUE "1")
bobui_feature("menu" PUBLIC
    SECTION "Widgets"
    LABEL "QMenu"
    PURPOSE "Provides popup-menus."
    CONDITION BOBUI_FEATURE_action AND BOBUI_FEATURE_pushbutton
)
bobui_feature_definition("menu" "BOBUI_NO_MENU" NEGATE VALUE "1")
bobui_feature("lineedit" PUBLIC
    SECTION "Widgets"
    LABEL "QLineEdit"
    PURPOSE "Provides single-line edits."
    CONDITION BOBUI_FEATURE_widgettextcontrol
)
bobui_feature_definition("lineedit" "BOBUI_NO_LINEEDIT" NEGATE VALUE "1")
bobui_feature("radiobutton" PUBLIC
    SECTION "Widgets"
    LABEL "QRadioButton"
    PURPOSE "Provides a radio button with a text label."
    CONDITION BOBUI_FEATURE_abstractbutton
)
bobui_feature("spinbox" PUBLIC
    SECTION "Widgets"
    LABEL "QSpinBox"
    PURPOSE "Provides spin boxes handling integers and discrete sets of values."
    CONDITION BOBUI_FEATURE_lineedit AND BOBUI_FEATURE_validator
)
bobui_feature_definition("spinbox" "BOBUI_NO_SPINBOX" NEGATE VALUE "1")
bobui_feature("tabbar" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIabBar"
    PURPOSE "Provides tab bars, e.g., for use in tabbed dialogs."
    CONDITION BOBUI_FEATURE_toolbutton
)
bobui_feature_definition("tabbar" "BOBUI_NO_TABBAR" NEGATE VALUE "1")
bobui_feature("tabwidget" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIabWidget"
    PURPOSE "Supports stacking tabbed widgets."
    CONDITION BOBUI_FEATURE_tabbar AND BOBUI_FEATURE_stackedwidget
)
bobui_feature_definition("tabwidget" "BOBUI_NO_TABWIDGET" NEGATE VALUE "1")
bobui_feature("combobox" PUBLIC
    SECTION "Widgets"
    LABEL "QComboBox"
    PURPOSE "Provides drop-down boxes presenting a list of options to the user."
    CONDITION BOBUI_FEATURE_lineedit AND BOBUI_FEATURE_standarditemmodel AND BOBUI_FEATURE_listview
)
bobui_feature_definition("combobox" "BOBUI_NO_COMBOBOX" NEGATE VALUE "1")
bobui_feature("fontcombobox" PUBLIC
    SECTION "Widgets"
    LABEL "QFontComboBox"
    PURPOSE "Provides a combobox that lets the user select a font family."
    CONDITION BOBUI_FEATURE_combobox AND BOBUI_FEATURE_stringlistmodel
)
bobui_feature_definition("fontcombobox" "BOBUI_NO_FONTCOMBOBOX" NEGATE VALUE "1")
bobui_feature("checkbox" PUBLIC
    SECTION "Widgets"
    LABEL "QCheckBox("
    PURPOSE "Provides a checkbox with a text label."
    CONDITION BOBUI_FEATURE_abstractbutton
)
bobui_feature("pushbutton" PUBLIC
    SECTION "Widgets"
    LABEL "QPushButton"
    PURPOSE "Provides a command button."
    CONDITION BOBUI_FEATURE_abstractbutton AND BOBUI_FEATURE_action
)
bobui_feature("toolbutton" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIoolButton"
    PURPOSE "Provides quick-access buttons to commands and options."
    CONDITION BOBUI_FEATURE_abstractbutton AND BOBUI_FEATURE_action
)
bobui_feature_definition("toolbutton" "BOBUI_NO_TOOLBUTTON" NEGATE VALUE "1")
bobui_feature("toolbar" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIoolBar"
    PURPOSE "Provides movable panels containing a set of controls."
    CONDITION BOBUI_FEATURE_mainwindow
)
bobui_feature_definition("toolbar" "BOBUI_NO_TOOLBAR" NEGATE VALUE "1")
bobui_feature("toolbox" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIoolBox"
    PURPOSE "Provides columns of tabbed widget items."
    CONDITION BOBUI_FEATURE_toolbutton AND BOBUI_FEATURE_scrollarea
)
bobui_feature_definition("toolbox" "BOBUI_NO_TOOLBOX" NEGATE VALUE "1")
bobui_feature("groupbox" PUBLIC
    SECTION "Widgets"
    LABEL "QGroupBox"
    PURPOSE "Provides widget grouping boxes with frames."
)
bobui_feature_definition("groupbox" "BOBUI_NO_GROUPBOX" NEGATE VALUE "1")
bobui_feature("buttongroup" PUBLIC
    SECTION "Widgets"
    LABEL "QButtonGroup"
    PURPOSE "Supports organizing groups of button widgets."
    CONDITION BOBUI_FEATURE_abstractbutton AND BOBUI_FEATURE_groupbox
)
bobui_feature_definition("buttongroup" "BOBUI_NO_BUTTONGROUP" NEGATE VALUE "1")
bobui_feature("mainwindow" PUBLIC
    SECTION "Widgets"
    LABEL "QMainWindow"
    PURPOSE "Provides main application windows."
    CONDITION BOBUI_FEATURE_menu AND BOBUI_FEATURE_resizehandler AND BOBUI_FEATURE_toolbutton
)
bobui_feature_definition("mainwindow" "BOBUI_NO_MAINWINDOW" NEGATE VALUE "1")
bobui_feature("dockwidget" PUBLIC
    SECTION "Widgets"
    LABEL "QDockwidget"
    PURPOSE "Supports docking widgets inside a QMainWindow or floated as a top-level window on the desktop."
    CONDITION BOBUI_FEATURE_mainwindow
)
bobui_feature_definition("dockwidget" "BOBUI_NO_DOCKWIDGET" NEGATE VALUE "1")
bobui_feature("mdiarea" PUBLIC
    SECTION "Widgets"
    LABEL "QMdiArea"
    PURPOSE "Provides an area in which MDI windows are displayed."
    CONDITION BOBUI_FEATURE_scrollarea
)
bobui_feature_definition("mdiarea" "BOBUI_NO_MDIAREA" NEGATE VALUE "1")
bobui_feature("resizehandler" PUBLIC
    SECTION "Widgets"
    LABEL "QWidgetResizeHandler"
    PURPOSE "Provides an internal resize handler for dock widgets."
)
bobui_feature_definition("resizehandler" "BOBUI_NO_RESIZEHANDLER" NEGATE VALUE "1")
bobui_feature("statusbar" PUBLIC
    SECTION "Widgets"
    LABEL "QStatusBar"
    PURPOSE "Supports presentation of status information."
)
bobui_feature_definition("statusbar" "BOBUI_NO_STATUSBAR" NEGATE VALUE "1")
bobui_feature("menubar" PUBLIC
    SECTION "Widgets"
    LABEL "QMenuBar"
    PURPOSE "Provides pull-down menu items."
    CONDITION BOBUI_FEATURE_menu AND BOBUI_FEATURE_toolbutton
)
bobui_feature_definition("menubar" "BOBUI_NO_MENUBAR" NEGATE VALUE "1")
bobui_feature("contextmenu" PUBLIC
    SECTION "Widgets"
    LABEL "Context menus"
    PURPOSE "Adds pop-up menus on right mouse click to numerous widgets."
    CONDITION BOBUI_FEATURE_menu
)
bobui_feature_definition("contextmenu" "BOBUI_NO_CONTEXTMENU" NEGATE VALUE "1")
bobui_feature("progressbar" PUBLIC
    SECTION "Widgets"
    LABEL "QProgressBar"
    PURPOSE "Supports presentation of operation progress."
)
bobui_feature_definition("progressbar" "BOBUI_NO_PROGRESSBAR" NEGATE VALUE "1")
bobui_feature("abstractslider" PUBLIC
    SECTION "Widgets"
    LABEL "QAbstractSlider"
    PURPOSE "Common super class for widgets like QScrollBar, QSlider and QDial."
)
bobui_feature("slider" PUBLIC
    SECTION "Widgets"
    LABEL "QSlider"
    PURPOSE "Provides sliders controlling a bounded value."
    CONDITION BOBUI_FEATURE_abstractslider
)
bobui_feature_definition("slider" "BOBUI_NO_SLIDER" NEGATE VALUE "1")
bobui_feature("scrollbar" PUBLIC
    SECTION "Widgets"
    LABEL "QScrollBar"
    PURPOSE "Provides scrollbars allowing the user access parts of a document that is larger than the widget used to display it."
    CONDITION BOBUI_FEATURE_slider
)
bobui_feature_definition("scrollbar" "BOBUI_NO_SCROLLBAR" NEGATE VALUE "1")
bobui_feature("dial" PUBLIC
    SECTION "Widgets"
    LABEL "QDial"
    PURPOSE "Provides a rounded range control, e.g., like a speedometer."
    CONDITION BOBUI_FEATURE_slider
)
bobui_feature_definition("dial" "BOBUI_NO_DIAL" NEGATE VALUE "1")
bobui_feature("scrollarea" PUBLIC
    SECTION "Widgets"
    LABEL "QScrollArea"
    PURPOSE "Supports scrolling views onto widgets."
    CONDITION BOBUI_FEATURE_scrollbar
)
bobui_feature_definition("scrollarea" "BOBUI_NO_SCROLLAREA" NEGATE VALUE "1")
bobui_feature("scroller" PUBLIC
    SECTION "Widgets"
    LABEL "QScroller"
    PURPOSE "Enables kinetic scrolling for any scrolling widget or graphics item."
    CONDITION BOBUI_FEATURE_easingcurve
)
bobui_feature("graphicsview" PUBLIC
    SECTION "Widgets"
    LABEL "QGraphicsView"
    PURPOSE "Provides a canvas/sprite framework."
    CONDITION BOBUI_FEATURE_scrollarea AND BOBUI_FEATURE_widgettextcontrol
)
bobui_feature_definition("graphicsview" "BOBUI_NO_GRAPHICSVIEW" NEGATE VALUE "1")
bobui_feature("graphicseffect" PUBLIC
    SECTION "Widgets"
    LABEL "QGraphicsEffect"
    PURPOSE "Provides various graphics effects."
    CONDITION BOBUI_FEATURE_graphicsview
)
bobui_feature_definition("graphicseffect" "BOBUI_NO_GRAPHICSEFFECT" NEGATE VALUE "1")
bobui_feature("textedit" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIextEdit"
    PURPOSE "Supports rich text editing."
    CONDITION BOBUI_FEATURE_scrollarea AND BOBUI_FEATURE_widgettextcontrol
)
bobui_feature_definition("textedit" "BOBUI_NO_TEXTEDIT" NEGATE VALUE "1")
bobui_feature("syntaxhighlighter" PUBLIC
    SECTION "Widgets"
    LABEL "QSyntaxHighlighter"
    PURPOSE "Supports custom syntax highlighting."
    CONDITION BOBUI_FEATURE_textedit
)
bobui_feature_definition("syntaxhighlighter" "BOBUI_NO_SYNTAXHIGHLIGHTER" NEGATE VALUE "1")
bobui_feature("rubberband" PUBLIC
    SECTION "Widgets"
    LABEL "QRubberBand"
    PURPOSE "Supports using rubberbands to indicate selections and boundaries."
)
bobui_feature_definition("rubberband" "BOBUI_NO_RUBBERBAND" NEGATE VALUE "1")
bobui_feature("tooltip" PUBLIC
    SECTION "Widgets"
    LABEL "BOBUIoolTip"
    PURPOSE "Supports presentation of tooltips."
    CONDITION BOBUI_FEATURE_label
)
bobui_feature_definition("tooltip" "BOBUI_NO_TOOLTIP" NEGATE VALUE "1")
bobui_feature("statustip" PUBLIC
    SECTION "Widgets"
    LABEL "Status Tip"
    PURPOSE "Supports status tip functionality and events."
)
bobui_feature_definition("statustip" "BOBUI_NO_STATUSTIP" NEGATE VALUE "1")
bobui_feature("sizegrip" PUBLIC
    SECTION "Widgets"
    LABEL "QSizeGrip"
    PURPOSE "Provides corner-grips for resizing top-level windows."
)
bobui_feature_definition("sizegrip" "BOBUI_NO_SIZEGRIP" NEGATE VALUE "1")
bobui_feature("calendarwidget" PUBLIC
    SECTION "Widgets"
    LABEL "QCalendarWidget"
    PURPOSE "Provides a monthly based calendar widget allowing the user to select a date."
    CONDITION ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_menu ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_spinbox ) AND ( BOBUI_FEATURE_tableview ) AND ( BOBUI_FEATURE_textdate ) AND ( BOBUI_FEATURE_toolbutton )
)
bobui_feature_definition("calendarwidget" "BOBUI_NO_CALENDARWIDGET" NEGATE VALUE "1")
bobui_feature("keysequenceedit" PUBLIC
    SECTION "Widgets"
    LABEL "QKeySequenceEdit"
    PURPOSE "Provides a widget for editing QKeySequences."
    CONDITION BOBUI_FEATURE_lineedit AND BOBUI_FEATURE_shortcut
)
bobui_feature_definition("keysequenceedit" "BOBUI_NO_KEYSEQUENCEEDIT" NEGATE VALUE "1")
bobui_feature("dialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QDialog"
    PURPOSE "Base class of dialog windows."
)
bobui_feature("dialogbuttonbox" PUBLIC
    SECTION "Dialogs"
    LABEL "QDialogButtonBox"
    PURPOSE "Presents buttons in a layout that is appropriate for the current widget style."
    CONDITION BOBUI_FEATURE_dialog AND BOBUI_FEATURE_pushbutton
)
bobui_feature("messagebox" PUBLIC
    SECTION "Dialogs"
    LABEL "QMessageBox"
    PURPOSE "Provides message boxes displaying informative messages and simple questions."
    CONDITION ( BOBUI_FEATURE_checkbox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton )
)
bobui_feature_definition("messagebox" "BOBUI_NO_MESSAGEBOX" NEGATE VALUE "1")
bobui_feature("colordialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QColorDialog"
    PURPOSE "Provides a dialog widget for specifying colors."
    CONDITION ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_spinbox )
)
bobui_feature_definition("colordialog" "BOBUI_NO_COLORDIALOG" NEGATE VALUE "1")
bobui_feature("filedialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QFileDialog"
    PURPOSE "Provides a dialog widget for selecting files or directories."
    CONDITION ( BOBUI_FEATURE_buttongroup ) AND ( BOBUI_FEATURE_combobox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_filesystemmodel ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_proxymodel ) AND ( BOBUI_FEATURE_splitter ) AND ( BOBUI_FEATURE_stackedwidget ) AND ( BOBUI_FEATURE_treeview ) AND ( BOBUI_FEATURE_toolbutton )
)
bobui_feature_definition("filedialog" "BOBUI_NO_FILEDIALOG" NEGATE VALUE "1")
bobui_feature("fontdialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QFontDialog"
    PURPOSE "Provides a dialog widget for selecting fonts."
    CONDITION ( BOBUI_FEATURE_checkbox ) AND ( BOBUI_FEATURE_combobox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_groupbox ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_stringlistmodel ) AND ( BOBUI_FEATURE_validator )
)
bobui_feature_definition("fontdialog" "BOBUI_NO_FONTDIALOG" NEGATE VALUE "1")
bobui_feature("progressdialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QProgressDialog"
    PURPOSE "Provides feedback on the progress of a slow operation."
    CONDITION ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_progressbar )
)
bobui_feature_definition("progressdialog" "BOBUI_NO_PROGRESSDIALOG" NEGATE VALUE "1")
bobui_feature("inputdialog" PUBLIC
    SECTION "Dialogs"
    LABEL "QInputDialog"
    PURPOSE "Provides a simple convenience dialog to get a single value from the user."
    CONDITION ( BOBUI_FEATURE_combobox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_dialogbuttonbox ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_spinbox ) AND ( BOBUI_FEATURE_stackedwidget ) AND ( BOBUI_FEATURE_textedit )
)
bobui_feature_definition("inputdialog" "BOBUI_NO_INPUTDIALOG" NEGATE VALUE "1")
bobui_feature("errormessage" PUBLIC
    SECTION "Dialogs"
    LABEL "QErrorMessage"
    PURPOSE "Provides an error message display dialog."
    CONDITION ( BOBUI_FEATURE_checkbox ) AND ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_textedit ) AND ( BOBUI_FEATURE_label ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_textedit )
)
bobui_feature_definition("errormessage" "BOBUI_NO_ERRORMESSAGE" NEGATE VALUE "1")
bobui_feature("wizard" PUBLIC
    SECTION "Dialogs"
    LABEL "QWizard"
    PURPOSE "Provides a framework for multi-page click-through dialogs."
    CONDITION ( BOBUI_FEATURE_dialog ) AND ( BOBUI_FEATURE_pushbutton ) AND ( BOBUI_FEATURE_label )
)
bobui_feature_definition("wizard" "BOBUI_NO_WIZARD" NEGATE VALUE "1")
bobui_feature("listview" PUBLIC
    SECTION "ItemViews"
    LABEL "QListView"
    PURPOSE "Provides a list or icon view onto a model."
    CONDITION BOBUI_FEATURE_itemviews
)
bobui_feature_definition("listview" "BOBUI_NO_LISTVIEW" NEGATE VALUE "1")
bobui_feature("tableview" PUBLIC
    SECTION "ItemViews"
    LABEL "BOBUIableView"
    PURPOSE "Provides a default model/view implementation of a table view."
    CONDITION BOBUI_FEATURE_itemviews
)
bobui_feature_definition("tableview" "BOBUI_NO_TABLEVIEW" NEGATE VALUE "1")
bobui_feature("treeview" PUBLIC
    SECTION "ItemViews"
    LABEL "BOBUIreeView"
    PURPOSE "Provides a default model/view implementation of a tree view."
    CONDITION BOBUI_FEATURE_itemviews
)
bobui_feature_definition("treeview" "BOBUI_NO_TREEVIEW" NEGATE VALUE "1")
bobui_feature("datawidgetmapper" PUBLIC
    SECTION "ItemViews"
    LABEL "QDataWidgetMapper"
    PURPOSE "Provides mapping between a section of a data model to widgets."
    CONDITION BOBUI_FEATURE_itemviews
)
bobui_feature_definition("datawidgetmapper" "BOBUI_NO_DATAWIDGETMAPPER" NEGATE VALUE "1")
bobui_feature("columnview" PUBLIC
    SECTION "ItemViews"
    LABEL "QColumnView"
    PURPOSE "Provides a model/view implementation of a column view."
    CONDITION BOBUI_FEATURE_listview
)
bobui_feature_definition("columnview" "BOBUI_NO_COLUMNVIEW" NEGATE VALUE "1")
bobui_feature("completer" PUBLIC
    SECTION "Utilities"
    LABEL "QCompleter"
    PURPOSE "Provides completions based on an item model."
    CONDITION BOBUI_FEATURE_proxymodel AND BOBUI_FEATURE_itemviews
)
bobui_feature_definition("completer" "BOBUI_NO_COMPLETER" NEGATE VALUE "1")
bobui_feature("fscompleter" PUBLIC
    SECTION "Utilities"
    LABEL "QFSCompleter"
    PURPOSE "Provides file name completion in QFileDialog."
    CONDITION BOBUI_FEATURE_filesystemmodel AND BOBUI_FEATURE_completer
)
bobui_feature_definition("fscompleter" "BOBUI_NO_FSCOMPLETER" NEGATE VALUE "1")
bobui_feature("undoview" PUBLIC
    SECTION "Utilities"
    LABEL "QUndoView"
    PURPOSE "Provides a widget which shows the contents of an undo stack."
    CONDITION BOBUI_FEATURE_undostack AND BOBUI_FEATURE_listview
)
bobui_feature_definition("undoview" "BOBUI_NO_UNDOVIEW" NEGATE VALUE "1")
bobui_configure_add_summary_section(NAME "BobUI Widgets")
bobui_configure_add_summary_entry(ARGS "gtk3")
bobui_configure_add_summary_entry(
    TYPE "featureList"
    ARGS "style-fusion style-mac style-windows style-windowsvista style-android"
    MESSAGE "Styles"
)
bobui_configure_end_summary_section() # end of "BobUI Widgets" section
