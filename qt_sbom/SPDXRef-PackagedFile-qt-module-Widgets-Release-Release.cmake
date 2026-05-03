
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Widgets.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Widgets.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Widgets.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Widgets.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Widgets" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Widgets")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Widgets"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Widgets.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Widgets-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Widgets CONTAINS SPDXRef-PackagedFile-qt-module-Widgets-Release
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtWidgets/qtwidgetsexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qfileiconprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qfileiconprovider.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qfileiconprovider_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qapplication.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qapplication.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qapplication_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qboxlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qboxlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesture.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesture.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesture_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesturemanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesturemanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesturerecognizer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgesturerecognizer.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgridlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qgridlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayoutengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayoutengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayoutitem.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qlayoutitem.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qrhiwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qrhiwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qrhiwidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qsizepolicy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qsizepolicy.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qstackedlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qstackedlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qstandardgestures.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qstandardgestures_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtestsupport_widgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtestsupport_widgets.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtwidgetsglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtwidgetsglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetrepaintmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetrepaintmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetsvariant.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetwindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwindowcontainer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwindowcontainer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qcommonstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qcommonstyle.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qcommonstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qcommonstylepixmaps_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qdrawutil.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qdrawutil.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qpixmapstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qpixmapstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qpixmapstyle_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qproxystyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qproxystyle.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qproxystyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyle.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylefactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylefactory.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylehelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylehelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleoption.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleoption.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylepainter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylepainter.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qcolormap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qcolormap.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qsystemtrayicon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qsystemtrayicon.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qsystemtrayicon_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractscrollarea.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractscrollarea.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractscrollarea_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qfocusframe.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qfocusframe.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qframe.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qframe.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qframe_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetanimator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetanimator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/qtwidgets-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/qtwidgets-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/ddf_Widgets.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/.qt/rcc/qrc_qstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/.qt/rcc/qrc_qstyle1.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/.qt/rcc/qrc_qstyle_fusion.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/effects/qgraphicseffect.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/effects/qgraphicseffect.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/effects/qgraphicseffect_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/effects/qpixmapfilter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/effects/qpixmapfilter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qaction.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qaction_widgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qaction_widgets_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qactiongroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetaction.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetaction.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwidgetaction_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qformlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qformlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qshortcut.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qshortcut_widgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtooltip.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtooltip.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qtooltip_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwhatsthis.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/kernel/qwhatsthis.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstyleanimation_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylesheetstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylesheetstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qstylesheetstyle_default.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qwindowsstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qwindowsstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qwindowsstyle_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qfusionstyle.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qfusionstyle_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/styles/qfusionstyle_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractbutton.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractbutton.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractbutton_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractslider.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractslider.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractslider_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qbuttongroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qbuttongroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qbuttongroup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcalendarwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcalendarwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcheckbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcheckbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcombobox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcombobox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcombobox_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcommandlinkbutton.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qcommandlinkbutton.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdatetimeedit.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdatetimeedit.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdatetimeedit_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdial.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdial.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdockarealayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdockarealayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdockwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdockwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdockwidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qeffects.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qeffects_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qfontcombobox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qfontcombobox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qgroupbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qgroupbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qkeysequenceedit.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qkeysequenceedit.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qkeysequenceedit_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlabel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlabel.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlabel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlcdnumber.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlcdnumber.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlineedit.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlineedit.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlineedit_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qlineedit_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetlinecontrol.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetlinecontrol_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmainwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmainwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmainwindowlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmainwindowlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdiarea.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdiarea.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdiarea_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdisubwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdisubwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmdisubwindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenu.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenu.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenu_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenubar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenubar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qmenubar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qprogressbar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qprogressbar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qpushbutton.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qpushbutton.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qpushbutton_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qradiobutton.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qradiobutton.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetresizehandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgetresizehandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdialogbuttonbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdialogbuttonbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qdialogbuttonbox_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qrubberband.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qrubberband.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollarea.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollarea.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollarea_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollbar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollbar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qscrollbar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsizegrip.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsizegrip.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qslider.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qslider.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractspinbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractspinbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qabstractspinbox_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qspinbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qspinbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsplashscreen.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsplashscreen.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsplitter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsplitter.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qsplitter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qstackedwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qstackedwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qstatusbar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qstatusbar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtabbar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtabbar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtabbar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qplaintextedit.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qplaintextedit.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qplaintextedit_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtextedit.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtextedit.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtextedit_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtextbrowser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtextbrowser.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtabwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtabwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbar.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbararealayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbararealayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarseparator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarseparator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarextension.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbarextension_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbutton.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qtoolbutton.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgettextcontrol.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgettextcontrol_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/widgets/qwidgettextcontrol_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qcolordialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qcolordialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qcolorwell_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qdialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qdialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qdialog_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qerrormessage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qerrormessage.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfiledialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfiledialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfiledialog.ui
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfiledialog_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qsidebar.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qsidebar_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfilesystemmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfontdialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfontdialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfontdialog_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qfscompleter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qinputdialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qinputdialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qmessagebox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qmessagebox.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/.qt/rcc/qrc_qmessagebox.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qprogressdialog.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qprogressdialog.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qwizard.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qwizard.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qwizard_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/dialogs/qwizard_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/complexwidgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/complexwidgets_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblecolorwell.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblecolorwell_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblemenu.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblemenu_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidgetfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidgetfactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/qaccessiblewidgets_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/rangecontrols.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/rangecontrols_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/simplewidgets.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/simplewidgets_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/itemviews.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/accessible/itemviews_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemdelegate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemdelegate.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemdelegate_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qabstractitemview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qbsptree.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qbsptree_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qheaderview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qheaderview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qheaderview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qitemdelegate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qitemdelegate.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qitemeditorfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qitemeditorfactory.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qitemeditorfactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qstyleditemdelegate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qstyleditemdelegate.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qwidgetitemdata_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qcolumnview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qcolumnview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qcolumnview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qcolumnviewgrip.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qcolumnviewgrip_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qdatawidgetmapper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qdatawidgetmapper.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistwidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistwidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qlistwidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtableview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtableview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtableview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtablewidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtablewidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtablewidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreeview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreeview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreeview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidgetitemiterator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidgetitemiterator.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/itemviews/qtreewidgetitemiterator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraph_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsanchorlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsanchorlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsanchorlayout_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsanchorlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsgridlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsgridlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsgridlayoutengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsgridlayoutengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsitem.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsitem.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsitem_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayout_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayoutitem.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayoutitem.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayoutitem_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayoutstyleinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslayoutstyleinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslinearlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicslinearlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsproxywidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsproxywidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsproxywidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscene.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscene.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscene_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscene_bsp.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscene_bsp_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscenebsptreeindex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscenebsptreeindex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicssceneevent.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicssceneevent.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicssceneindex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicssceneindex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscenelinearindex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsscenelinearindex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicstransform.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicstransform.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicstransform_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsview_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicswidget.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicswidget.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicswidget_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicswidget_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qsimplex_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qsimplex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsitemanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/graphicsview/qgraphicsitemanimation.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qcompleter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qcompleter.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qcompleter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qflickgesture.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qflickgesture_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscroller.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscroller.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscroller_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscrollerproperties.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscrollerproperties.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qscrollerproperties_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qundostack.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qundogroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qundoview.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qundoview.h
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/util/qsystemtrayicon_qpa.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Widgets-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/widgets/doc/qtwidgets.qdocconf
"
                )
        endif()
