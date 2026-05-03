
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6OpenGL.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6OpenGL.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6OpenGL.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6OpenGL.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-OpenGL" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-OpenGL")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-OpenGL"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6OpenGL.lib
SPDXID: SPDXRef-PackagedFile-qt-module-OpenGL-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-OpenGL CONTAINS SPDXRef-PackagedFile-qt-module-OpenGL-Release
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtOpenGL/qtopenglexports.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/compat/removed_api.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengl2pexvertexarray.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengl2pexvertexarray_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglbuffer.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglbuffer.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglcustomshaderstage.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglcustomshaderstage_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengldebug.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengldebug.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglengineshadermanager.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglengineshadermanager_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglengineshadersource_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglframebufferobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglframebufferobject.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglframebufferobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglgradientcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglgradientcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpaintdevice.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpaintdevice.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpaintdevice_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpaintengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpaintengine_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpixeltransferoptions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglpixeltransferoptions.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglshadercache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglshaderprogram.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglshaderprogram.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexture.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexture.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexture_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureblitter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureblitter.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexturecache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexturecache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureglyphcache.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureglyphcache_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexturehelper.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltexturehelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureuploader.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltextureuploader_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltimerquery.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionfunctions.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionfunctions.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionfunctions_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionfunctionsfactory.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionfunctionsfactory.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionprofile.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglversionprofile.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglvertexarrayobject.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglvertexarrayobject.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglvertexarrayobject_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglwindow.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qtopenglglobal.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/ddf_OpenGL.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_0.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_0.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_1.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_1.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_2.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_2.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_3.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_3.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_4.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_4.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_5.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_1_5.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_2_0.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_2_0.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_2_1.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_2_1.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_0.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_0.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_1.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_1.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_2_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_2_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_2_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_2_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_3_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_3_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_3_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_3_3_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_0_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_0_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_0_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_0_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_1_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_1_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_1_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_1_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_2_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_2_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_2_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_2_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_3_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_3_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_3_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_3_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_4_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_4_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_4_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_4_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_5_compatibility.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_5_compatibility.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_5_core.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglfunctions_4_5_core.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopenglqueryhelper_p.h
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/qopengltimerquery.cpp
Relationship: SPDXRef-PackagedFile-qt-module-OpenGL-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/opengl/doc/qtopengl.qdocconf
"
                )
        endif()
