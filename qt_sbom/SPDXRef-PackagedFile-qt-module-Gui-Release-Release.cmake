
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Gui.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Gui.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Gui.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Gui.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Gui" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Gui")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Gui"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Gui.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Gui-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Gui CONTAINS SPDXRef-PackagedFile-qt-module-Gui-Release
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtGui/qtguiexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qabstractfileiconengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qabstractfileiconengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qabstractfileiconprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qabstractfileiconprovider.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qabstractfileiconprovider_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qbitmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qbitmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qbmphandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qbmphandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qfonticonengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qfonticonengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qicon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qicon.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qicon_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconengine.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconengineplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconengineplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconloader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qiconloader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimage.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimage_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimage_conversions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimageiohandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimageiohandler.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagepixmapcleanuphooks.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagepixmapcleanuphooks_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagereader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagereader.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagereaderwriterhelpers.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagereaderwriterhelpers_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagewriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimagewriter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpaintengine_pic.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpaintengine_pic_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpicture.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpicture.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpicture_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_blitter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_blitter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_raster.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_raster_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmapcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmapcache.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmapcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qplatformpixmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qplatformpixmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qppmhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qppmhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qxbmhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qxbmhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qxpmhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qxpmhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaccessibilityhints.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaccessibilityhints.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaccessibilityhints_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qclipboard.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qclipboard.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qcursor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qcursor.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qcursor_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qeventpoint.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qeventpoint.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qeventpoint_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qevent.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qevent.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qevent_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qgenericplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qgenericplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qgenericpluginfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qgenericpluginfactory.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qguiapplication.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qguiapplication.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qguiapplication_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qguiapplication_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qguivariant.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qhighdpiscaling.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qhighdpiscaling_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevicemanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevicemanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputdevicemanager_p_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputmethod.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputmethod.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinputmethod_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinternalmimedata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qinternalmimedata_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qkeymapper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qkeymapper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qoffscreensurface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qoffscreensurface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qoffscreensurface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qoffscreensurface_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qopenglcontext.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpaintdevicewindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpaintdevicewindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpaintdevicewindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpalette.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpalette.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpalette_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpixelformat.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpixelformat.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformclipboard.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformclipboard.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformcursor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformcursor.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformdialoghelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformdialoghelper.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformgraphicsbuffer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformgraphicsbuffer.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformgraphicsbufferhelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformgraphicsbufferhelper.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontext.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontext_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontextfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontextfactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontextplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatforminputcontextplugin_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegration.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegration.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegrationfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegrationfactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegrationplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformintegrationplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformkeymapper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformkeymapper.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformmenu.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformmenu.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformmenu_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformnativeinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformnativeinterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformoffscreensurface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformoffscreensurface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformopenglcontext.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformscreen.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformscreen.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformscreen_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformservices.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformservices.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsessionmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsessionmanager.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsharedgraphicscache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsharedgraphicscache.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsurface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsurface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsystemtrayicon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformsystemtrayicon.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformtheme.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformtheme.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformtheme_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformthemefactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformthemefactory_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformthemeplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformthemeplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformwindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpointingdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpointingdevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qpointingdevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qrasterwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qrasterwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qscreen.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qscreen.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qscreen_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qscreen_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsessionmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsessionmanager.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsessionmanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qstylehints.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qstylehints.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qstylehints_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsurface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsurface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsurfaceformat.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsurfaceformat.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qtestsupport_gui.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qtestsupport_gui.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qtguiglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qtguiglobal_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindowdefs.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindowsysteminterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindowsysteminterface.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindowsysteminterface_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qgenericmatrix.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qgenericmatrix.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qmatrix4x4.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qmatrix4x4.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qquaternion.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qquaternion.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qvector2d.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qvector3d.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qvector4d.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qvectornd.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/math3d/qvectornd.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopengl.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglext.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglfunctions.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglextrafunctions.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstore.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstoredefaultcompositor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstoredefaultcompositor_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstorerhisupport.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbackingstorerhisupport_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbezier.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbezier_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qblendfunctions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qblendfunctions_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qblittable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qblittable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbrush.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qbrush.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolor.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolor_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolorclut_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolormatrix_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolorspace.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolorspace.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolorspace_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransferfunction_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransfergeneric_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransfertable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransform.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortransform_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortrc_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortrclut.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcolortrclut_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcompositionfunctions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcosmeticstroker.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcosmeticstroker_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcmyk_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdatabuffer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_x86_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawingprimitive_sse2_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_loongarch64_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawingprimitive_lsx_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qemulationpaintengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qemulationpaintengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qfixed_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qgrayraster.c
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qgrayraster_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qicc.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qicc_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qimageeffects.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qimagescale.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qimagescale_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qmath_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qmemrotate.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qmemrotate_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qoutlinemapper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qoutlinemapper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagedpaintdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagedpaintdevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagedpaintdevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagelayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagelayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpageranges.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpageranges.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpageranges_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagesize.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpagesize.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintdevice.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine_blitter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine_blitter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine_raster.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengine_raster_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengineex.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpaintengineex_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainterstateguard.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainterstateguard.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainterpath.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainterpath.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpainterpath_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpathclipper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpathclipper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpathsimplifier.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpathsimplifier_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdf.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdf_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdfoutputintent.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdfoutputintent.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdfwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpdfwriter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpen.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpen.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpen_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpixellayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpixellayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qplatformbackingstore.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qplatformbackingstore.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpolygon.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qpolygon.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrasterdefs_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrasterizer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrasterizer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrbtree_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qregion.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qregion.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrgb.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrgba64.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrgba64_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qrgbafloat.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qstroker.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qstroker_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtextureglyphcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtextureglyphcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtransform.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtransform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtriangulatingstroker.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtriangulatingstroker_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtriangulator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qtriangulator_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qvectorpath_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhi.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhi.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhi_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhi_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhinull.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhinull_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshader.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshaderdescription.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshaderdescription.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qshaderdescription_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qabstracttextdocumentlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qabstracttextdocumentlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qabstracttextdocumentlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qcolrpaintgraphrenderer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qcolrpaintgraphrenderer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qdistancefield.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qdistancefield_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfont.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfont.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfont_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontdatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontdatabase.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontdatabase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontengineglyphcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontengineglyphcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontinfo.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontmetrics.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontmetrics.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontsubset.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontsubset_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontvariableaxis.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfontvariableaxis.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfragmentmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qfragmentmap_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qglyphrun.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qglyphrun.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qglyphrun_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qinputcontrol.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qinputcontrol_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qplatformfontdatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qplatformfontdatabase.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qrawfont.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qrawfont.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qrawfont_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qstatictext.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qstatictext.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qstatictext_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qsyntaxhighlighter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qsyntaxhighlighter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextcursor.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextcursor.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextcursor_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocument.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocument.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocument_p.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocument_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentfragment.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentfragment.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentfragment_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentlayout_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextdocumentwriter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextformat.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextformat.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextformat_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtexthtmlparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtexthtmlparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextimagehandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextimagehandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextlayout.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextlayout.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextlist.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextlist.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextobject.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextoption.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextoption.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtexttable.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtexttable.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtexttable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qabstractlayoutstyleinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qabstractlayoutstyleinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qastchandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qastchandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qdesktopservices.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qdesktopservices.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qgridlayoutengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qgridlayoutengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qhexstring_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qktxhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qktxhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qlayoutpolicy.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qlayoutpolicy_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qpkmhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qpkmhandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qtexturefiledata.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qtexturefiledata_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qtexturefilehandler_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qtexturefilereader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qtexturefilereader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qvalidator.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qvalidator.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/qtgui-config.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/qtgui-config_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/ddf_Gui.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/.qt/rcc/qrc_qpdf.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/.qt/rcc/qrc_gui_shaders.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qstandarditemmodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qstandarditemmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qstandarditemmodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qopenglcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qopenglcontext_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qopenglcontext_platform.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformopenglcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopengl.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopengl_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglextensions_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglfunctions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglprogrambinarycache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/opengl/qopenglprogrambinarycache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhigles2.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhigles2_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/animation/qguivariantanimation.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_win.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qpixmap_win_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qwindowdefs_win.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsguieventdispatcher.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsguieventdispatcher_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsmimeconverter.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsmimeconverter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsnativeinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsthemecache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/platform/windows/qwindowsthemecache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3d11.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3d11_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3d12.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3d12_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3dhelpers.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qrhid3dhelpers_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/vs_test_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/cs_mipmap_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/cs_mipmap_3d_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/D3D12MemoryAllocator/D3D12MemAlloc.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/D3D12MemoryAllocator/D3D12MemAlloc.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qdxgivsyncservice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qdxgivsyncservice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qdxgihdrinfo_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/rhi/qdxgihdrinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabasebase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabasebase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsnativeimage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsnativeimage_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessible.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qplatformaccessibility.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessible.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessible_base.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblebridge.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblebridge.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblebridgeutils.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblebridgeutils_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblecache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblecache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblehelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessiblehelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessibleobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessibleobject.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessibleplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qaccessibleplugin.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/accessible/qplatformaccessibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaction.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaction.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qaction_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qactiongroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qactiongroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qactiongroup_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qdnd.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qdnd_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qdrag.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qdrag.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformdrag.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qplatformdrag.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshapedpixmapdndwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshapedpixmapdndwindow_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsimpledrag.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qsimpledrag_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qkeysequence.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qkeysequence.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qkeysequence_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshortcut.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshortcut.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshortcut_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshortcutmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/kernel/qshortcutmap_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qmovie.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qmovie.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_sse2.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/image/qimage_ssse3.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_ssse3.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_sse4.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qimagescale_sse4.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_avx2.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qharfbuzzng.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qharfbuzzng_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextodfwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextodfwriter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextmarkdownimporter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextmarkdownimporter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/md4c/md4c.c
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/md4c/md4c.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextmarkdownwriter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qtextmarkdownwriter_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcssutil.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qcssutil_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qcssparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/qcssparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/freetype/qfontengine_ft.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/freetype/qfontengine_ft_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/freetype/qfreetypefontdatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/freetype/qfreetypefontdatabase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabase_ft.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontdatabase_ft_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontenginedirectwrite.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsfontenginedirectwrite_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsdirectwritefontdatabase.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/text/windows/qwindowsdirectwritefontdatabase_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qundostack.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qundostack.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qundostack_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qundogroup.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qundogroup.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qfileinfogatherer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qfileinfogatherer_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qfilesystemmodel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qfilesystemmodel.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/itemmodels/qfilesystemmodel_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/painting/qdrawhelper_neon_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qedidparser.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qedidparser_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/util/qedidvendortable_p.h
Relationship: SPDXRef-PackagedFile-qt-module-Gui-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/gui/doc/qtgui.qdocconf
"
                )
        endif()
