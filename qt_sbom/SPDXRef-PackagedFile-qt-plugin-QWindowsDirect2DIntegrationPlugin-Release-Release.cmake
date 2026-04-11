
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qdirect2d.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'plugins/platforms/qdirect2d.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qdirect2d.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qdirect2d.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./plugins/platforms/qdirect2d.lib
SPDXID: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qtwindowsglobal.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwin10helpers.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwin10helpers.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsapplication.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsapplication.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowscontext.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowscontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowscursor.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowscursor.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdialoghelpers.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdialoghelpers.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdropdataobject.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdropdataobject.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsiconengine.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsiconengine.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsinputcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsinputcontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsintegration.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsintegration.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsinternalmimedata.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsinternalmimedata.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowskeymapper.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowskeymapper.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsmenu.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsmenu.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsmimeregistry.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsmimeregistry.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsnativeinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsnativeinterface.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsole.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsole.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsopengltester.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsopengltester.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowspointerhandler.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowspointerhandler.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsscreen.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsscreen.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsservices.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsservices.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowstheme.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowstheme.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsthreadpoolrunner.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindow.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindow.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindowclassdescription.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindowclassdescription.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindowclassregistry.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowswindowclassregistry.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dbackingstore.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dbackingstore.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dbitmap.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dbitmap.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dcontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2ddevicecontext.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2ddevicecontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dhelpers.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dintegration.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dintegration.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dnativeinterface.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dnativeinterface.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dpaintdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dpaintdevice.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dpaintengine.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dpaintengine.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dplatformpixmap.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dplatformpixmap.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dplatformplugin.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/qwindowsdirect2dwindow.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/.qt/rcc/qrc_openglblacklists.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsglcontext.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsglcontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsopenglcontext.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowssystemtrayicon.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowssystemtrayicon.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsclipboard.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsclipboard.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdrag.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowsdrag.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowstabletsupport.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowstabletsupport.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowssessionmanager.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/qwindowssessionmanager.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/.qt/rcc/qrc_cursors.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiautomation.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiautomation.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaaccessibility.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaaccessibility.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiabaseprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiabaseprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaexpandcollapseprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaexpandcollapseprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiagriditemprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiagriditemprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiagridprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiagridprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiainvokeprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiainvokeprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiamainprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiamainprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaprovidercache.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaprovidercache.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiarangevalueprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiarangevalueprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaselectionitemprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaselectionitemprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaselectionprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiaselectionprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatableitemprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatableitemprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatableprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatableprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatextprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatextprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatextrangeprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatextrangeprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatoggleprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiatoggleprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiautils.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiautils.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiavalueprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiavalueprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiawindowprovider.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/uiautomation/qwindowsuiawindowprovider.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/.qt/rcc/openglblacklists.qrc
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/openglblacklists/default.json
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/.qt/rcc/cursors.qrc
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/closedhandcursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/closedhandcursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/closedhandcursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragcopycursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragcopycursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragcopycursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/draglinkcursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/draglinkcursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/draglinkcursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragmovecursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragmovecursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/dragmovecursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/openhandcursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/openhandcursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/openhandcursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splithcursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splithcursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splithcursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splitvcursor_32.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splitvcursor_48.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/windows/images/splitvcursor_64.png
Relationship: SPDXRef-PackagedFile-qt-plugin-QWindowsDirect2DIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/direct2d/ddf_QWindowsDirect2DIntegrationPlugin.h
"
                )
        endif()
