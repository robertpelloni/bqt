# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(X11_XCB MODULE)
bobui_find_package(WrapHarfbuzz MODULE PROVIDED_TARGETS WrapHarfbuzz::WrapHarfbuzz)
bobui_find_package(WrapPNG MODULE PROVIDED_TARGETS WrapPNG::WrapPNG)
bobui_find_package(WrapFreetype MODULE PROVIDED_TARGETS WrapFreetype::WrapFreetype)

if (BOBUI_FEATURE_gui)
    if(BOBUI_QPA_PLATFORMS)
        list(GET BOBUI_QPA_PLATFORMS 0 _default_platform)
    elseif(WIN32)
        set(_default_platform "windows")
    elseif(ANDROID)
        set(_default_platform "android")
    elseif(MACOS)
        set(_default_platform "cocoa")
    elseif(UIKIT)
        set(_default_platform "ios")
    elseif(WATCHOS)
        set(_default_platform "minimal")
    elseif(QNX)
        set(_default_platform "qnx")
    elseif(INTEGRITY)
        set(_default_platform "eglfs")
    elseif(HAIKU)
        set(_default_platform "haiku")
    elseif(WASM)
        set(_default_platform "wasm")
    else()
        set(_default_platform "xcb")
    endif()

    set(BOBUI_QPA_DEFAULT_PLATFORM "${_default_platform}" CACHE STRING "QPA default platform")
    if(NOT "${BOBUI_QPA_DEFAULT_PLATFORM}" IN_LIST BOBUI_QPA_PLATFORMS)
        list(APPEND BOBUI_QPA_PLATFORMS "${BOBUI_QPA_DEFAULT_PLATFORM}")
        set(BOBUI_QPA_PLATFORMS "${BOBUI_QPA_PLATFORMS}" CACHE STRING
            "QPA platforms deployed by default" FORCE)
    endif()
endif()

# Silence warnings in 3rdparty code
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_source_files_properties(../3rdparty/md4c/md4c.c PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set_source_files_properties(../3rdparty/md4c/md4c.c PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers -Wno-missing-braces")
endif()

# Silence AUTOMOC warning 'No relevant classes found. No output generated.'
if (NOT BOBUI_FEATURE_wayland)
    set_source_files_properties(kernel/qplatformwindow_p.h
                                PROPERTIES SKIP_AUTOMOC TRUE)
endif()

unset(qmake_module_config)
if(BOBUI_FEATURE_opengl)
    list(APPEND qmake_module_config opengl)
endif()

bobui_internal_add_module(Gui
    PLUGIN_TYPES accessiblebridge platforms platforms/darwin xcbglintegrations platformthemes platforminputcontexts generic iconengines imageformats egldeviceintegrations
    QMAKE_MODULE_CONFIG "${qmake_module_config}"
    SOURCES
        compat/removed_api.cpp
        image/qabstractfileiconengine.cpp image/qabstractfileiconengine_p.h
        image/qabstractfileiconprovider.cpp image/qabstractfileiconprovider.h image/qabstractfileiconprovider_p.h
        image/qbitmap.cpp image/qbitmap.h
        image/qbmphandler.cpp image/qbmphandler_p.h
        image/qfonticonengine.cpp image/qfonticonengine_p.h
        image/qicon.cpp image/qicon.h image/qicon_p.h
        image/qiconengine.cpp image/qiconengine.h image/qiconengine_p.h
        image/qiconengineplugin.cpp image/qiconengineplugin.h
        image/qiconloader.cpp image/qiconloader_p.h
        image/qimage.cpp image/qimage.h image/qimage_p.h
        image/qimage_conversions.cpp
        image/qimageiohandler.cpp image/qimageiohandler.h
        image/qimagepixmapcleanuphooks.cpp image/qimagepixmapcleanuphooks_p.h
        image/qimagereader.cpp image/qimagereader.h
        image/qimagereaderwriterhelpers.cpp image/qimagereaderwriterhelpers_p.h
        image/qimagewriter.cpp image/qimagewriter.h
        image/qpaintengine_pic.cpp image/qpaintengine_pic_p.h
        image/qpicture.cpp image/qpicture.h image/qpicture_p.h
        image/qpixmap.cpp image/qpixmap.h
        image/qpixmap_blitter.cpp image/qpixmap_blitter_p.h
        image/qpixmap_raster.cpp image/qpixmap_raster_p.h
        image/qpixmapcache.cpp image/qpixmapcache.h image/qpixmapcache_p.h
        image/qplatformpixmap.cpp image/qplatformpixmap.h
        image/qppmhandler.cpp image/qppmhandler_p.h
        image/qxbmhandler.cpp image/qxbmhandler_p.h
        image/qxpmhandler.cpp image/qxpmhandler_p.h
        kernel/qaccessibilityhints.cpp kernel/qaccessibilityhints.h kernel/qaccessibilityhints_p.h
        kernel/qclipboard.cpp kernel/qclipboard.h
        kernel/qcursor.cpp kernel/qcursor.h kernel/qcursor_p.h
        kernel/qeventpoint.cpp kernel/qeventpoint.h kernel/qeventpoint_p.h
        kernel/qevent.cpp kernel/qevent.h kernel/qevent_p.h
        kernel/qgenericplugin.cpp kernel/qgenericplugin.h
        kernel/qgenericpluginfactory.cpp kernel/qgenericpluginfactory.h
        kernel/qguiapplication.cpp kernel/qguiapplication.h kernel/qguiapplication_p.h kernel/qguiapplication_platform.h
        kernel/qguivariant.cpp
        kernel/qhighdpiscaling.cpp kernel/qhighdpiscaling_p.h
        kernel/qinputdevice.cpp kernel/qinputdevice.h kernel/qinputdevice_p.h
        kernel/qinputdevicemanager.cpp kernel/qinputdevicemanager_p.h
        kernel/qinputdevicemanager_p_p.h
        kernel/qinputmethod.cpp kernel/qinputmethod.h kernel/qinputmethod_p.h
        kernel/qinternalmimedata.cpp kernel/qinternalmimedata_p.h
        kernel/qkeymapper.cpp kernel/qkeymapper_p.h
        kernel/qoffscreensurface.cpp kernel/qoffscreensurface.h kernel/qoffscreensurface_p.h
        kernel/qoffscreensurface_platform.h
        kernel/qopenglcontext.h
        kernel/qpaintdevicewindow.cpp kernel/qpaintdevicewindow.h kernel/qpaintdevicewindow_p.h
        kernel/qpalette.cpp kernel/qpalette.h kernel/qpalette_p.h
        kernel/qpixelformat.cpp kernel/qpixelformat.h
        kernel/qplatformclipboard.cpp kernel/qplatformclipboard.h
        kernel/qplatformcursor.cpp kernel/qplatformcursor.h
        kernel/qplatformdialoghelper.cpp kernel/qplatformdialoghelper.h
        kernel/qplatformgraphicsbuffer.cpp kernel/qplatformgraphicsbuffer.h
        kernel/qplatformgraphicsbufferhelper.cpp kernel/qplatformgraphicsbufferhelper.h
        kernel/qplatforminputcontext.cpp kernel/qplatforminputcontext.h kernel/qplatforminputcontext_p.h
        kernel/qplatforminputcontextfactory.cpp kernel/qplatforminputcontextfactory_p.h
        kernel/qplatforminputcontextplugin.cpp kernel/qplatforminputcontextplugin_p.h
        kernel/qplatformintegration.cpp kernel/qplatformintegration.h
        kernel/qplatformintegrationfactory.cpp kernel/qplatformintegrationfactory_p.h
        kernel/qplatformintegrationplugin.cpp kernel/qplatformintegrationplugin.h
        kernel/qplatformkeymapper.cpp kernel/qplatformkeymapper.h
        kernel/qplatformmenu.cpp kernel/qplatformmenu.h kernel/qplatformmenu_p.h
        kernel/qplatformnativeinterface.cpp kernel/qplatformnativeinterface.h
        kernel/qplatformoffscreensurface.cpp kernel/qplatformoffscreensurface.h
        kernel/qplatformopenglcontext.h
        kernel/qplatformscreen.cpp kernel/qplatformscreen.h kernel/qplatformscreen_p.h
        kernel/qplatformservices.cpp kernel/qplatformservices.h
        kernel/qplatformsessionmanager.cpp kernel/qplatformsessionmanager.h
        kernel/qplatformsharedgraphicscache.cpp kernel/qplatformsharedgraphicscache.h
        kernel/qplatformsurface.cpp kernel/qplatformsurface.h
        kernel/qplatformsystemtrayicon.cpp kernel/qplatformsystemtrayicon.h
        kernel/qplatformtheme.cpp kernel/qplatformtheme.h kernel/qplatformtheme_p.h
        kernel/qplatformthemefactory.cpp kernel/qplatformthemefactory_p.h
        kernel/qplatformthemeplugin.cpp kernel/qplatformthemeplugin.h
        kernel/qplatformwindow.cpp kernel/qplatformwindow.h kernel/qplatformwindow_p.h
        kernel/qpointingdevice.cpp kernel/qpointingdevice.h kernel/qpointingdevice_p.h
        kernel/qrasterwindow.cpp kernel/qrasterwindow.h
        kernel/qscreen.cpp kernel/qscreen.h kernel/qscreen_p.h kernel/qscreen_platform.h
        kernel/qsessionmanager.cpp kernel/qsessionmanager.h kernel/qsessionmanager_p.h
        kernel/qstylehints.cpp kernel/qstylehints.h kernel/qstylehints_p.h
        kernel/qsurface.cpp kernel/qsurface.h
        kernel/qsurfaceformat.cpp kernel/qsurfaceformat.h
        kernel/bobuiestsupport_gui.cpp kernel/bobuiestsupport_gui.h
        kernel/bobuiguiglobal.h kernel/bobuiguiglobal_p.h
        kernel/qwindow.cpp kernel/qwindow.h kernel/qwindow_p.h
        kernel/qwindowdefs.h
        kernel/qwindowsysteminterface.cpp kernel/qwindowsysteminterface.h kernel/qwindowsysteminterface_p.h
        math3d/qgenericmatrix.cpp math3d/qgenericmatrix.h
        math3d/qmatrix4x4.cpp math3d/qmatrix4x4.h
        math3d/qquaternion.cpp math3d/qquaternion.h
        math3d/qvector2d.h
        math3d/qvector3d.h
        math3d/qvector4d.h
        math3d/qvectornd.cpp math3d/qvectornd.h
        opengl/qopengl.h
        opengl/qopenglext.h
        opengl/qopenglfunctions.h
        opengl/qopenglextrafunctions.h
        painting/qbackingstore.cpp painting/qbackingstore.h
        painting/qbackingstoredefaultcompositor.cpp painting/qbackingstoredefaultcompositor_p.h
        painting/qbackingstorerhisupport.cpp painting/qbackingstorerhisupport_p.h
        painting/qbezier.cpp painting/qbezier_p.h
        painting/qblendfunctions.cpp painting/qblendfunctions_p.h
        painting/qblittable.cpp painting/qblittable_p.h
        painting/qbrush.cpp painting/qbrush.h
        painting/qcolor.cpp painting/qcolor.h painting/qcolor_p.h
        painting/qcolorclut_p.h
        painting/qcolormatrix_p.h
        painting/qcolorspace.cpp painting/qcolorspace.h painting/qcolorspace_p.h
        painting/qcolortransferfunction_p.h
        painting/qcolortransfergeneric_p.h
        painting/qcolortransfertable_p.h
        painting/qcolortransform.cpp painting/qcolortransform.h painting/qcolortransform_p.h
        painting/qcolortrc_p.h
        painting/qcolortrclut.cpp painting/qcolortrclut_p.h
        painting/qcompositionfunctions.cpp
        painting/qcosmeticstroker.cpp painting/qcosmeticstroker_p.h
        painting/qcmyk_p.h
        painting/qdatabuffer_p.h
        painting/qdrawhelper_p.h
        painting/qdrawhelper_x86_p.h
        painting/qdrawingprimitive_sse2_p.h
        painting/qdrawhelper_loongarch64_p.h
        painting/qdrawingprimitive_lsx_p.h
        painting/qemulationpaintengine.cpp painting/qemulationpaintengine_p.h
        painting/qfixed_p.h
        painting/qgrayraster.c painting/qgrayraster_p.h
        painting/qicc.cpp painting/qicc_p.h
        painting/qimageeffects.cpp
        painting/qimagescale.cpp painting/qimagescale_p.h
        painting/qmath_p.h
        painting/qmemrotate.cpp painting/qmemrotate_p.h
        painting/qoutlinemapper.cpp painting/qoutlinemapper_p.h
        painting/qpagedpaintdevice.cpp painting/qpagedpaintdevice.h painting/qpagedpaintdevice_p.h
        painting/qpagelayout.cpp painting/qpagelayout.h
        painting/qpageranges.cpp painting/qpageranges.h painting/qpageranges_p.h
        painting/qpagesize.cpp painting/qpagesize.h
        painting/qpaintdevice.cpp painting/qpaintdevice.h
        painting/qpaintengine.cpp painting/qpaintengine.h painting/qpaintengine_p.h
        painting/qpaintengine_blitter.cpp painting/qpaintengine_blitter_p.h
        painting/qpaintengine_raster.cpp painting/qpaintengine_raster_p.h
        painting/qpaintengineex.cpp painting/qpaintengineex_p.h
        painting/qpainter.cpp painting/qpainter.h painting/qpainter_p.h
        painting/qpainterstateguard.cpp painting/qpainterstateguard.h
        painting/qpainterpath.cpp painting/qpainterpath.h painting/qpainterpath_p.h
        painting/qpathclipper.cpp painting/qpathclipper_p.h
        painting/qpathsimplifier.cpp painting/qpathsimplifier_p.h
        painting/qpdf.cpp painting/qpdf_p.h
        painting/qpdfoutputintent.cpp painting/qpdfoutputintent.h
        painting/qpdfwriter.cpp painting/qpdfwriter.h
        painting/qpen.cpp painting/qpen.h painting/qpen_p.h
        painting/qpixellayout.cpp painting/qpixellayout_p.h
        painting/qplatformbackingstore.cpp painting/qplatformbackingstore.h
        painting/qpolygon.cpp painting/qpolygon.h
        painting/qrasterdefs_p.h
        painting/qrasterizer.cpp painting/qrasterizer_p.h
        painting/qrbtree_p.h
        painting/qregion.cpp painting/qregion.h
        painting/qrgb.h
        painting/qrgba64.h painting/qrgba64_p.h
        painting/qrgbafloat.h
        painting/qstroker.cpp painting/qstroker_p.h
        painting/bobuiextureglyphcache.cpp painting/bobuiextureglyphcache_p.h
        painting/bobuiransform.cpp painting/bobuiransform.h
        painting/bobuiriangulatingstroker.cpp painting/bobuiriangulatingstroker_p.h
        painting/bobuiriangulator.cpp painting/bobuiriangulator_p.h
        painting/qvectorpath_p.h
        rhi/qrhi.cpp rhi/qrhi.h rhi/qrhi_platform.h rhi/qrhi_p.h
        rhi/qrhinull.cpp rhi/qrhinull_p.h
        rhi/qshader.cpp rhi/qshader.h rhi/qshader_p.h
        rhi/qshaderdescription.cpp rhi/qshaderdescription.h rhi/qshaderdescription_p.h
        text/qabstracttextdocumentlayout.cpp text/qabstracttextdocumentlayout.h text/qabstracttextdocumentlayout_p.h
        text/qcolrpaintgraphrenderer.cpp text/qcolrpaintgraphrenderer_p.h
        text/qdistancefield.cpp text/qdistancefield_p.h
        text/qfont.cpp text/qfont.h text/qfont_p.h
        text/qfontdatabase.cpp text/qfontdatabase.h text/qfontdatabase_p.h
        text/qfontengine.cpp text/qfontengine_p.h
        text/qfontengineglyphcache.cpp text/qfontengineglyphcache_p.h
        text/qfontinfo.h
        text/qfontmetrics.cpp text/qfontmetrics.h
        text/qfontsubset.cpp text/qfontsubset_p.h
        text/qfontvariableaxis.cpp text/qfontvariableaxis.h
        text/qfragmentmap.cpp text/qfragmentmap_p.h
        text/qglyphrun.cpp text/qglyphrun.h text/qglyphrun_p.h
        text/qinputcontrol.cpp text/qinputcontrol_p.h
        text/qplatformfontdatabase.cpp text/qplatformfontdatabase.h
        text/qrawfont.cpp text/qrawfont.h text/qrawfont_p.h
        text/qstatictext.cpp text/qstatictext.h text/qstatictext_p.h
        text/qsyntaxhighlighter.cpp text/qsyntaxhighlighter.h
        text/bobuiextcursor.cpp text/bobuiextcursor.h text/bobuiextcursor_p.h
        text/bobuiextdocument.cpp text/bobuiextdocument.h text/bobuiextdocument_p.cpp text/bobuiextdocument_p.h
        text/bobuiextdocumentfragment.cpp text/bobuiextdocumentfragment.h text/bobuiextdocumentfragment_p.h
        text/bobuiextdocumentlayout.cpp text/bobuiextdocumentlayout_p.h
        text/bobuiextdocumentwriter.cpp text/bobuiextdocumentwriter.h
        text/bobuiextengine.cpp text/bobuiextengine_p.h
        text/bobuiextformat.cpp text/bobuiextformat.h text/bobuiextformat_p.h
        text/bobuiexthtmlparser.cpp text/bobuiexthtmlparser_p.h
        text/bobuiextimagehandler.cpp text/bobuiextimagehandler_p.h
        text/bobuiextlayout.cpp text/bobuiextlayout.h
        text/bobuiextlist.cpp text/bobuiextlist.h
        text/bobuiextobject.cpp text/bobuiextobject.h text/bobuiextobject_p.h
        text/bobuiextoption.cpp text/bobuiextoption.h
        text/bobuiexttable.cpp text/bobuiexttable.h text/bobuiexttable_p.h
        util/qabstractlayoutstyleinfo.cpp util/qabstractlayoutstyleinfo_p.h
        util/qastchandler.cpp util/qastchandler_p.h
        util/qdesktopservices.cpp util/qdesktopservices.h
        util/qgridlayoutengine.cpp util/qgridlayoutengine_p.h
        util/qhexstring_p.h
        util/qktxhandler.cpp util/qktxhandler_p.h
        util/qlayoutpolicy.cpp util/qlayoutpolicy_p.h
        util/qpkmhandler.cpp util/qpkmhandler_p.h
        util/bobuiexturefiledata.cpp util/bobuiexturefiledata_p.h
        util/bobuiexturefilehandler_p.h
        util/bobuiexturefilereader.cpp util/bobuiexturefilereader_p.h
        util/qvalidator.cpp util/qvalidator.h
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_NO_USING_NAMESPACE
        BOBUI_USE_NODISCARD_FILE_OPEN
        BOBUI_QPA_DEFAULT_PLATFORM_NAME="${BOBUI_QPA_DEFAULT_PLATFORM}"
    INCLUDE_DIRECTORIES
        ../3rdparty/VulkanMemoryAllocator
        ../3rdparty/D3D12MemoryAllocator
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
    NO_PCH_SOURCES
        compat/removed_api.cpp
        painting/qdrawhelper.cpp
    PRECOMPILED_HEADER
        "kernel/bobui_gui_pch.h"
    QPA_HEADER_FILTERS
        "(^|/)qplatform.+\\.h$|(^|/)qwindowsystem.+\\.h$"
    RHI_HEADER_FILTERS
        "(^|/)qrhi\\.h$|(^|/)qrhi_platform\\.h$|(^|/)qshader\\.h$|(^|/)qshaderdescription\\.h$"
    ATTRIBUTION_FILE_DIR_PATHS
        rhi
        opengl
        painting
        text
        ../3rdparty/VulkanMemoryAllocator
        ../3rdparty/emoji-segmenter
        # Used by qbrush.cpp -> painting/webgradients.cpp which is generated by the JS script
        ../../util/gradientgen
    ELF_LINKER_DYNAMIC_LIST
        BobUIGui.dynlist.in
)

# Resources:
if(BOBUI_FEATURE_pdf)
    set_source_files_properties("../3rdparty/icc/sRGB2014.icc"
        PROPERTIES BOBUI_RESOURCE_ALIAS "sRGB2014.icc"
    )
    set(qpdf_resource_files
        "../3rdparty/icc/sRGB2014.icc"
    )
    bobui_internal_extend_target(Gui
        ATTRIBUTION_FILE_DIR_PATHS
            ../3rdparty/icc
    )

    bobui_internal_add_resource(Gui "qpdf"
       PREFIX
           "/qpdf/"
       BASE
           "painting"
       FILES
           ${qpdf_resource_files}
    )
endif()

if(UNIX AND NOT APPLE)
    set_target_properties(Gui PROPERTIES UNITY_BUILD OFF) # X11 define clashes.
endif()

bobui_internal_add_resource(Gui "gui_shaders"
    PREFIX
        "/bobui-project.org/gui"
    FILES
        "painting/shaders/backingstorecompose.vert.qsb"
        "painting/shaders/backingstorecompose.frag.qsb"
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_standarditemmodel
    SOURCES
        itemmodels/qstandarditemmodel.cpp itemmodels/qstandarditemmodel.h itemmodels/qstandarditemmodel_p.h
)

# With qmake, gui's opengl.pri used CONFIG += opengl, where opengl.prf
# used direct public linkage against either libGLESv2 or libGL, depending
# on the opengl _feature_. This is done by hand now here (where the
# feature is available).
# On Windows with 'dynamic' OpenGL we must not link to opengl32.
# DO NOT MOVE THIS TO THE BEGINNING OF THE FILE, the feature variables are not
# available until the bobui_internal_add_module call.
if(BOBUI_FEATURE_opengl)
    if(BOBUI_FEATURE_opengles2)
        find_package(GLESv2)
        target_link_libraries(Gui PUBLIC GLESv2::GLESv2)

        if(TARGET PlatformGraphics::PlatformGraphics)
            bobui_internal_extend_target(Gui
                LIBRARIES
                    PlatformGraphics::PlatformGraphics
            )
        endif()
    elseif(NOT BOBUI_FEATURE_opengl_dynamic)
        target_link_libraries(Gui PUBLIC WrapOpenGL::WrapOpenGL)
    endif()
endif()

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_opengl
    SOURCES
        kernel/qopenglcontext.cpp kernel/qopenglcontext_p.h
        kernel/qopenglcontext_platform.h
        kernel/qplatformopenglcontext.cpp
        opengl/qopengl.cpp opengl/qopengl_p.h
        opengl/qopenglextensions_p.h
        opengl/qopenglfunctions.cpp
        opengl/qopenglprogrambinarycache.cpp opengl/qopenglprogrambinarycache_p.h
        rhi/qrhigles2.cpp rhi/qrhigles2_p.h
)

bobui_internal_extend_target(Gui CONDITION MACOS
    SOURCES
        platform/macos/qcocoanativeinterface.mm
        platform/macos/qmacstyle_p.h
    LIBRARIES
        ${FWAppKit}
        ${FWCarbon}
    PUBLIC_LIBRARIES
        ${FWAppKit}
)

bobui_internal_extend_target(Gui CONDITION MACOS AND BOBUI_FEATURE_opengl
    PUBLIC_LIBRARIES
        ${FWOpenGL}
)

bobui_internal_extend_target(Gui CONDITION UIKIT
    SOURCES
        platform/ios/qiosnativeinterface.cpp
)

bobui_internal_extend_target(Gui CONDITION WASM
    SOURCES
        platform/wasm/qwasmnativeinterface.cpp
)

bobui_internal_extend_target(Gui CONDITION APPLE
    SOURCES
        image/qimage_darwin.mm
        painting/qcoregraphics.mm painting/qcoregraphics_p.h
        painting/qrasterbackingstore.cpp painting/qrasterbackingstore_p.h
        painting/qrhibackingstore.cpp painting/qrhibackingstore_p.h
        platform/darwin/qmacmimeregistry.mm platform/darwin/qmacmimeregistry_p.h
        platform/darwin/qutimimeconverter.mm platform/darwin/qutimimeconverter.h
        platform/darwin/qapplekeymapper.mm platform/darwin/qapplekeymapper_p.h
        platform/darwin/qapplefileiconengine.mm platform/darwin/qapplefileiconengine_p.h
        platform/darwin/qappleiconengine.mm platform/darwin/qappleiconengine_p.h
        text/coretext/qcoretextfontdatabase.mm text/coretext/qcoretextfontdatabase_p.h
        text/coretext/qfontengine_coretext.mm text/coretext/qfontengine_coretext_p.h
    LIBRARIES
        ${FWCoreFoundation}
        ${FWCoreGraphics}
        ${FWCoreText}
        ${FWFoundation}
        ${FWAccelerate}
    PUBLIC_LIBRARIES
        ${FWImageIO}
)

bobui_internal_extend_target(Gui CONDITION APPLE AND BOBUI_FEATURE_metal
    SOURCES
        platform/darwin/qmetallayer.mm platform/darwin/qmetallayer_p.h
    LIBRARIES
        ${FWQuartzCore}
)

bobui_internal_extend_target(Gui CONDITION QNX
    SOURCES
        painting/qrasterbackingstore.cpp painting/qrasterbackingstore_p.h
        painting/qrhibackingstore.cpp painting/qrhibackingstore_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_animation
    SOURCES
        animation/qguivariantanimation.cpp
)

bobui_internal_extend_target(Gui CONDITION WIN32
    SOURCES
        image/qpixmap_win.cpp image/qpixmap_win_p.h
        kernel/qwindowdefs_win.h
        platform/windows/qwindowsguieventdispatcher.cpp platform/windows/qwindowsguieventdispatcher_p.h
        platform/windows/qwindowsmimeconverter.h platform/windows/qwindowsmimeconverter.cpp
        platform/windows/qwindowsnativeinterface.cpp
        platform/windows/qwindowsthemecache.cpp platform/windows/qwindowsthemecache_p.h
        rhi/qrhid3d11.cpp rhi/qrhid3d11_p.h
        rhi/qrhid3d12.cpp rhi/qrhid3d12_p.h
        rhi/qrhid3dhelpers.cpp rhi/qrhid3dhelpers_p.h
        rhi/vs_test_p.h
        rhi/cs_mipmap_p.h rhi/cs_mipmap_3d_p.h
        ../3rdparty/D3D12MemoryAllocator/D3D12MemAlloc.h
        ../3rdparty/D3D12MemoryAllocator/D3D12MemAlloc.cpp
        rhi/qdxgivsyncservice_p.h rhi/qdxgivsyncservice.cpp
        rhi/qdxgihdrinfo_p.h rhi/qdxgihdrinfo.cpp
        text/windows/qwindowsfontdatabase.cpp text/windows/qwindowsfontdatabase_p.h
        text/windows/qwindowsfontdatabasebase.cpp text/windows/qwindowsfontdatabasebase_p.h
        text/windows/qwindowsfontengine.cpp text/windows/qwindowsfontengine_p.h
        text/windows/qwindowsnativeimage.cpp text/windows/qwindowsnativeimage_p.h
    LIBRARIES
        advapi32
        gdi32
        ole32
        shell32
        user32
        uxtheme
    PUBLIC_LIBRARIES
        d3d11
        dxgi
        dxguid
        d3d12
    NO_UNITY_BUILD_SOURCES
        rhi/qrhid3d11.cpp rhi/qrhid3d12.cpp # duplicate aligned() etc
        text/windows/qwindowsfontdatabasebase.cpp # duplicate struct OS2Table
    ATTRIBUTION_FILE_DIR_PATHS
        ../3rdparty/D3D12MemoryAllocator
)

if(BOBUI_FEATURE_graphicsframecapture)
    bobui_internal_extend_target(Gui
        SOURCES
            util/qgraphicsframecapture_p.h util/qgraphicsframecapture.cpp
            util/qgraphicsframecapture_p_p.h
    )

    bobui_internal_extend_target(Gui CONDITION (WIN32 OR (UNIX AND NOT APPLE)) AND BOBUI_FEATURE_library
        LIBRARIES
            RenderDoc::RenderDoc
        SOURCES
            util/qgraphicsframecapturerenderdoc_p_p.h util/qgraphicsframecapturerenderdoc.cpp
    )

    bobui_internal_extend_target(Gui CONDITION IOS OR MACOS
        SOURCES
            util/qgraphicsframecapturemetal_p_p.h util/qgraphicsframecapturemetal.mm
        PUBLIC_LIBRARIES
            ${FWMetal}
    )
endif()

if(BOBUI_FEATURE_egl)
    bobui_find_package(EGL MODULE)
endif()

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_egl
    SOURCES
        opengl/platform/egl/qeglconvenience.cpp opengl/platform/egl/qeglconvenience_p.h
        opengl/platform/egl/qeglstreamconvenience.cpp opengl/platform/egl/qeglstreamconvenience_p.h
        opengl/platform/egl/bobui_egl_p.h
    LIBRARIES
        EGL::EGL
)

# These two headers are always installed, their contents are guarded with
# "#if BOBUI_CONFIG(accessibility)", so if BOBUI_FEATURE_accessibility is not
# enabled, they are just duds.
bobui_internal_extend_target(Gui
    SOURCES
        accessible/qaccessible.h accessible/qplatformaccessibility.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_accessibility
    SOURCES
        accessible/qaccessible.cpp accessible/qaccessible_base.h
        accessible/qaccessiblebridge.cpp accessible/qaccessiblebridge.h
        accessible/qaccessiblebridgeutils.cpp accessible/qaccessiblebridgeutils_p.h
        accessible/qaccessiblecache.cpp accessible/qaccessiblecache_p.h
        accessible/qaccessiblehelper.cpp accessible/qaccessiblehelper_p.h
        accessible/qaccessibleobject.cpp accessible/qaccessibleobject.h
        accessible/qaccessibleplugin.cpp accessible/qaccessibleplugin.h
        accessible/qplatformaccessibility.cpp
)

bobui_internal_extend_target(Gui CONDITION APPLE AND BOBUI_FEATURE_accessibility
    SOURCES
        accessible/qaccessiblecache_mac.mm
    LIBRARIES
        ${FWFoundation}
)

if(BOBUI_FEATURE_accessibility AND BOBUI_FEATURE_accessibility_atspi_bridge)
    set(atspi_accessibility ON)
else()
    set(atspi_accessibility OFF)
endif()

bobui_internal_extend_target(Gui CONDITION atspi_accessibility
    SOURCES
        accessible/linux/atspiadaptor.cpp accessible/linux/atspiadaptor_p.h
        accessible/linux/dbusconnection.cpp accessible/linux/dbusconnection_p.h
        accessible/linux/qspi_constant_mappings.cpp accessible/linux/qspi_constant_mappings_p.h
        accessible/linux/qspi_struct_marshallers.cpp accessible/linux/qspi_struct_marshallers_p.h
        accessible/linux/qspiaccessiblebridge.cpp accessible/linux/qspiaccessiblebridge_p.h
        accessible/linux/qspiapplicationadaptor.cpp accessible/linux/qspiapplicationadaptor_p.h
        accessible/linux/qspidbuscache.cpp accessible/linux/qspidbuscache_p.h
        accessible/linux/qspimatchrulematcher.cpp accessible/linux/qspimatchrulematcher_p.h
    DBUS_ADAPTOR_SOURCES
        accessible/linux/dbusxml/Cache.xml
        accessible/linux/dbusxml/DeviceEventController.xml
    DBUS_ADAPTOR_FLAGS
        "-i" "BobUIGui/private/qspi_struct_marshallers_p.h"
        "--namespace" "BobUIGuiPrivate"
    DBUS_INTERFACE_SOURCES
        accessible/linux/dbusxml/Bus.xml
        accessible/linux/dbusxml/Socket.xml
        accessible/linux/dbusxml/org.freedesktop.DBus.Properties.xml
    DBUS_INTERFACE_FLAGS
        "-i" "BobUIGui/private/qspi_struct_marshallers_p.h"
        "--namespace" "BobUIGuiPrivate"
)

if(atspi_accessibility)
    bobui_internal_add_target_include_dirs(Gui PkgConfig::ATSPI2)
endif()

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_action
    SOURCES
        kernel/qaction.cpp kernel/qaction.h kernel/qaction_p.h
        kernel/qactiongroup.cpp kernel/qactiongroup.h kernel/qactiongroup_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_draganddrop
    SOURCES
        kernel/qdnd.cpp kernel/qdnd_p.h
        kernel/qdrag.cpp kernel/qdrag.h
        kernel/qplatformdrag.cpp kernel/qplatformdrag.h
        kernel/qshapedpixmapdndwindow.cpp kernel/qshapedpixmapdndwindow_p.h
        kernel/qsimpledrag.cpp kernel/qsimpledrag_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_shortcut
    SOURCES
        kernel/qkeysequence.cpp kernel/qkeysequence.h kernel/qkeysequence_p.h
        kernel/qshortcut.cpp kernel/qshortcut.h kernel/qshortcut_p.h
        kernel/qshortcutmap.cpp kernel/qshortcutmap_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_movie
    SOURCES
        image/qmovie.cpp image/qmovie.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_png
    SOURCES
        image/qpnghandler.cpp image/qpnghandler_p.h
    LIBRARIES
        WrapPNG::WrapPNG
)

bobui_internal_extend_target(Gui
    CONDITION
        BOBUI_FEATURE_png AND WIN32 AND MINGW AND
        (CMAKE_CXX_COMPILER_VESION VERSION_EQUAL "8.1.0")
    COMPILE_OPTIONS
        -fno-reorder-blocks-and-partition
)

if(TEST_architecture_arch STREQUAL x86_64)
    bobui_internal_extend_target(Gui
       SOURCES
            painting/qdrawhelper_sse2.cpp
    )
else()
    bobui_internal_add_simd_part(Gui SIMD sse2
        SOURCES
            painting/qdrawhelper_sse2.cpp
    )
endif()

bobui_internal_add_simd_part(Gui SIMD ssse3
    SOURCES
        image/qimage_ssse3.cpp
        painting/qdrawhelper_ssse3.cpp
)

bobui_internal_add_simd_part(Gui SIMD sse4_1
    SOURCES
        painting/qdrawhelper_sse4.cpp
        painting/qimagescale_sse4.cpp
)

bobui_internal_add_simd_part(Gui SIMD arch_haswell
    SOURCES
        painting/qdrawhelper_avx2.cpp
    EXCLUDE_OSX_ARCHITECTURES
        arm64
)

bobui_internal_add_simd_part(Gui SIMD neon
    SOURCES
        image/qimage_neon.cpp
        painting/qdrawhelper_neon.cpp painting/qdrawhelper_neon_p.h
        painting/qimagescale_neon.cpp
)

bobui_internal_add_simd_part(Gui SIMD lsx
    SOURCES
        image/qimage_lsx.cpp
        painting/qdrawhelper_lsx.cpp
        painting/qimagescale_lsx.cpp
)

bobui_internal_add_simd_part(Gui SIMD lasx
    SOURCES
        image/qimage_lasx.cpp
        painting/qdrawhelper_lasx.cpp
)

if(NOT ANDROID)
    bobui_internal_add_simd_part(Gui SIMD mips_dsp
        SOURCES
            painting/qdrawhelper_mips_dsp.cpp painting/qdrawhelper_mips_dsp_p.h
            painting/qdrawhelper_mips_dsp_asm.S
            painting/bobui_mips_asm_dsp_p.h
    )

    bobui_internal_add_simd_part(Gui SIMD mips_dspr2
        SOURCES
            image/qimage_mips_dspr2.cpp
            image/qimage_mips_dspr2_asm.S
            painting/qdrawhelper_mips_dspr2_asm.S
    )
endif()

bobui_internal_extend_target(Gui CONDITION ANDROID
    SOURCES
        platform/android/qandroidnativeinterface.cpp
        painting/qrasterbackingstore.cpp painting/qrasterbackingstore_p.h
        painting/qrhibackingstore.cpp painting/qrhibackingstore_p.h
)

bobui_internal_extend_target(Gui CONDITION ANDROID AND (TEST_architecture_arch STREQUAL arm64 OR TEST_architecture_arch STREQUAL arm)
    SOURCES
        image/qimage_neon.cpp
        painting/qdrawhelper_neon.cpp painting/qdrawhelper_neon_p.h
        painting/qimagescale_neon.cpp
)

bobui_internal_extend_target(Gui CONDITION ANDROID AND (TEST_architecture_arch STREQUAL i386 OR TEST_architecture_arch STREQUAL x86_64)
    SOURCES
        image/qimage_ssse3.cpp
        painting/qdrawhelper_sse2.cpp
        painting/qdrawhelper_ssse3.cpp
    DEFINES
        BOBUI_COMPILER_SUPPORTS_SSE2 BOBUI_COMPILER_SUPPORTS_SSE2
        BOBUI_COMPILER_SUPPORTS_SSE3 BOBUI_COMPILER_SUPPORTS_SSE3
        BOBUI_COMPILER_SUPPORTS_SSSE3 BOBUI_COMPILER_SUPPORTS_SSSE3
)

if (MINGW AND CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 8.1.0)
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=86048
  set_source_files_properties(image/qpnghandler.cpp
    PROPERTIES COMPILE_OPTIONS -fno-reorder-blocks-and-partition
  )
endif()

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_harfbuzz
    SOURCES
        text/qharfbuzzng.cpp text/qharfbuzzng_p.h
    LIBRARIES
        WrapHarfbuzz::WrapHarfbuzz
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_harfbuzz AND UIKIT
    LIBRARIES
        ${FWCoreText}
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_textodfwriter
    SOURCES
        text/bobuiextodfwriter.cpp text/bobuiextodfwriter_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_textmarkdownreader
    SOURCES
        text/bobuiextmarkdownimporter.cpp text/bobuiextmarkdownimporter_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_system_textmarkdownreader AND BOBUI_FEATURE_textmarkdownreader
    LIBRARIES
        WrapSystemMd4c::WrapSystemMd4c
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_textmarkdownreader AND NOT BOBUI_FEATURE_system_textmarkdownreader
    SOURCES
        ../3rdparty/md4c/md4c.c ../3rdparty/md4c/md4c.h
    DEFINES
        MD4C_USE_UTF8
    INCLUDE_DIRECTORIES
        ../3rdparty/md4c
    ATTRIBUTION_FILE_DIR_PATHS
        ../3rdparty/md4c
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_textmarkdownwriter
    SOURCES
        text/bobuiextmarkdownwriter.cpp text/bobuiextmarkdownwriter_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_cssparser
    SOURCES
        painting/qcssutil.cpp painting/qcssutil_p.h
        text/qcssparser.cpp text/qcssparser_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_freetype
    SOURCES
        text/freetype/qfontengine_ft.cpp text/freetype/qfontengine_ft_p.h
        text/freetype/qfreetypefontdatabase.cpp text/freetype/qfreetypefontdatabase_p.h
    LIBRARIES
        WrapFreetype::WrapFreetype
)

bobui_internal_extend_target(Gui CONDITION UNIX AND NOT APPLE
    SOURCES
        text/unix/qgenericunixfontdatabase_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_fontconfig AND BOBUI_FEATURE_freetype AND UNIX AND NOT APPLE
    SOURCES
        text/unix/qfontconfigdatabase.cpp text/unix/qfontconfigdatabase_p.h
        text/unix/qfontenginemultifontconfig.cpp text/unix/qfontenginemultifontconfig_p.h
    LIBRARIES
        Fontconfig::Fontconfig
)

bobui_internal_extend_target(Gui CONDITION APPLE AND NOT MACOS
    LIBRARIES
        ${FWUIKit}
)

bobui_internal_extend_target(Gui CONDITION APPLE AND BOBUI_FEATURE_freetype
    LIBRARIES
        WrapFreetype::WrapFreetype
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_freetype AND WIN32
    SOURCES
        text/windows/qwindowsfontdatabase_ft.cpp text/windows/qwindowsfontdatabase_ft_p.h
    LIBRARIES
        WrapFreetype::WrapFreetype
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_direct2d AND BOBUI_FEATURE_directwrite AND WIN32
    SOURCES
        text/windows/qwindowsfontenginedirectwrite.cpp text/windows/qwindowsfontenginedirectwrite_p.h
    NO_UNITY_BUILD_SOURCES
        text/windows/qwindowsfontenginedirectwrite.cpp # conflicts
    LIBRARIES
        d2d1
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_direct2d AND BOBUI_FEATURE_directwrite AND BOBUI_FEATURE_directwrite3 AND WIN32
    SOURCES
        text/windows/qwindowsdirectwritefontdatabase.cpp text/windows/qwindowsdirectwritefontdatabase_p.h
    LIBRARIES
        dwrite
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_direct2d AND BOBUI_FEATURE_directwrite AND WIN32 AND NOT BOBUI_FEATURE_directwrite3
    LIBRARIES
        dwrite
)

bobui_internal_extend_target(Gui CONDITION MINGW AND WIN32
    LIBRARIES
        uuid
)

# Only enable the Pixman draw-helpers on platforms that support the GAS syntax of their asm files
# Note: These helpers are only used for 16-bit surfaces, so excluding them does not generally
# exclude neon-drawhelpers on these platforms.
if(UNIX AND NOT ANDROID AND NOT INTEGRITY AND NOT (TEST_architecture_arch STREQUAL "arm64") AND NOT UIKIT AND NOT BOBUI_IS_MACOS_UNIVERSAL)
    bobui_internal_extend_target(Gui
        DEFINES
            ENABLE_PIXMAN_DRAWHELPERS
        ATTRIBUTION_FILE_DIR_PATHS
            ../3rdparty/pixman
    )
    bobui_internal_add_simd_part(Gui SIMD neon
        SOURCES
            ../3rdparty/pixman/pixman-arm-neon-asm.S
            painting/qdrawhelper_neon_asm.S
    )
endif()

bobui_internal_extend_target(Gui CONDITION ANDROID AND TEST_architecture_arch STREQUAL x86_64
    SOURCES
        painting/qdrawhelper_sse4.cpp
        painting/qimagescale_sse4.cpp
    DEFINES
        BOBUI_COMPILER_SUPPORTS_SSE4_1
        BOBUI_COMPILER_SUPPORTS_SSE4_2
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_system_zlib
    LIBRARIES
        WrapZLIB::WrapZLIB
)

bobui_internal_extend_target(Gui CONDITION NOT BOBUI_FEATURE_system_zlib
    INCLUDE_DIRECTORIES
        ../3rdparty/zlib/src
    ATTRIBUTION_FILE_DIR_PATHS
        ../3rdparty/zlib
)

bobui_internal_extend_target(Gui CONDITION NOT BOBUI_FEATURE_system_zlib
    LIBRARIES
        BobUI::Core
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_undocommand
    SOURCES
        util/qundostack.cpp util/qundostack.h util/qundostack_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_undogroup
    SOURCES
        util/qundogroup.cpp util/qundogroup.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_opengl AND BOBUI_FEATURE_xlib AND NOT BOBUI_FEATURE_opengles2
    SOURCES
        opengl/platform/unix/qglxconvenience.cpp opengl/platform/unix/qglxconvenience_p.h
    LIBRARIES
        X11::X11
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_opengles2
    SOURCES
        opengl/qopengles2ext.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_egl AND BOBUI_FEATURE_opengl AND NOT WASM
    SOURCES
        opengl/platform/egl/qeglpbuffer.cpp opengl/platform/egl/qeglpbuffer_p.h
        opengl/platform/egl/qeglplatformcontext.cpp opengl/platform/egl/qeglplatformcontext_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_egl AND NOT BOBUI_FEATURE_egl_x11
    DEFINES
        BOBUI_EGL_NO_X11
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_dlopen AND BOBUI_FEATURE_egl
    LIBRARIES
        ${CMAKE_DL_LIBS}
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_standarditemmodel
    SOURCES
        itemmodels/qstandarditemmodel.cpp itemmodels/qstandarditemmodel.h itemmodels/qstandarditemmodel_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_filesystemmodel
    SOURCES
        itemmodels/qfileinfogatherer.cpp itemmodels/qfileinfogatherer_p.h
        itemmodels/qfilesystemmodel.cpp itemmodels/qfilesystemmodel.h itemmodels/qfilesystemmodel_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_vulkan
    SOURCES
        rhi/qrhivulkan.cpp rhi/qrhivulkan_p.h
        vulkan/qbasicvulkanplatforminstance.cpp vulkan/qbasicvulkanplatforminstance_p.h
        vulkan/qplatformvulkaninstance.cpp vulkan/qplatformvulkaninstance.h
        vulkan/qvulkandefaultinstance.cpp vulkan/qvulkandefaultinstance_p.h
        vulkan/qvulkanfunctions.cpp
        vulkan/qvulkaninstance.cpp vulkan/qvulkaninstance.h vulkan/qvulkaninstance_p.h
        vulkan/qvulkanwindow.cpp vulkan/qvulkanwindow.h vulkan/qvulkanwindow_p.h
    ATTRIBUTION_FILE_DIR_PATHS
        vulkan
)
if(BOBUI_FEATURE_vulkan)
    bobui_internal_add_target_include_dirs_and_optionally_propagate(
        Gui WrapVulkanHeaders::WrapVulkanHeaders)
endif()

# We must always generate syncbobui-injected header files,
# because we added a custom command earlier for those for framework builds.
set(vulkan_fun "qvulkanfunctions.h")
set(vulkan_fun_p "qvulkanfunctions_p.h")
set(vulkan_fun_outputs "vulkan/${vulkan_fun}" "vulkan/${vulkan_fun_p}")
set(vulkan_fun_command_content COMMAND "${CMAKE_COMMAND}" -E make_directory "vulkan")
if (BOBUI_FEATURE_vulkan)
    set(vulkan_fun_p_cpp "qvulkanfunctions_p.cpp")
    list(APPEND vulkan_fun_outputs "vulkan/${vulkan_fun_p_cpp}")

    if(NOT "${BOBUI_HOST_PATH}" STREQUAL "")
        set(host_executable_suffix "")
        if(CMAKE_HOST_WIN32)
            set(host_executable_suffix ".exe")
        endif()
        bobui_internal_get_host_info_var_prefix(host_info_var_prefix)
        bobui_path_join(qvkgen
            "${BOBUI_HOST_PATH}"
            "${${host_info_var_prefix}_LIBEXECDIR}"
            "qvkgen${host_executable_suffix}")
    elseif(BOBUI_OPTIONAL_TOOLS_PATH)
        bobui_path_join(qvkgen
            "${BOBUI_OPTIONAL_TOOLS_PATH}"
            "${INSTALL_LIBEXECDIR}"
            "qvkgen${host_executable_suffix}")
    else()
        set(qvkgen "${BOBUI_CMAKE_EXPORT_NAMESPACE}::qvkgen")
    endif()

    list(APPEND vulkan_fun_command_content
        COMMAND "${qvkgen}"
            "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vk.xml"
            "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/licenseheader.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/vulkan/qvulkanfunctions"
        DEPENDS vulkan/vk.xml ${qvkgen}
        COMMENT "Generating vulkan data"
    )

    bobui_internal_extend_target(Gui
        SOURCES
            "${CMAKE_CURRENT_BINARY_DIR}/vulkan/${vulkan_fun}"
            "${CMAKE_CURRENT_BINARY_DIR}/vulkan/${vulkan_fun_p}"
            "${CMAKE_CURRENT_BINARY_DIR}/vulkan/${vulkan_fun_p_cpp}"
    )
else()
    foreach(file ${vulkan_fun_outputs})
        list(APPEND vulkan_fun_command_content
            COMMAND "${CMAKE_COMMAND}" -E touch "${file}")
    endforeach()
endif()
add_custom_command(
    OUTPUT ${vulkan_fun_outputs}
    ${vulkan_fun_command_content}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
)
set_source_files_properties(${vulkan_fun_outputs} PROPERTIES _bobui_syncbobui_exclude_from_docs TRUE)

bobui_internal_extend_target(Gui CONDITION WASM
    SOURCES
        platform/wasm/qlocalfileapi.cpp platform/wasm/qlocalfileapi_p.h
        platform/wasm/qwasmlocalfileaccess.cpp platform/wasm/qwasmlocalfileaccess_p.h
)

bobui_internal_extend_target(Gui CONDITION UNIX
    SOURCES
        platform/unix/qunixnativeinterface.cpp
)

bobui_internal_extend_target(Gui CONDITION UNIX AND NOT WASM
    SOURCES
        platform/unix/qgenericunixeventdispatcher.cpp platform/unix/qgenericunixeventdispatcher_p.h
        platform/unix/qunixeventdispatcher.cpp
        platform/unix/qunixeventdispatcher_qpa_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_glib AND UNIX
    SOURCES
        platform/unix/qeventdispatcher_glib.cpp platform/unix/qeventdispatcher_glib_p.h
    LIBRARIES
        GLIB2::GLIB2
)

bobui_internal_extend_target(Gui CONDITION UNIX AND (BOBUI_FEATURE_xcb OR NOT UIKIT)
    SOURCES
        platform/unix/qdesktopunixservices.cpp platform/unix/qdesktopunixservices_p.h
)

bobui_internal_extend_target(Gui CONDITION UNIX AND (BOBUI_FEATURE_xcb)
    SOURCES
        platform/unix/bobuix11extras.cpp platform/unix/bobuix11extras_p.h
)

bobui_internal_extend_target(Gui CONDITION TARGET BobUI::DBus AND UNIX AND (BOBUI_FEATURE_xcb OR NOT UIKIT)
    LIBRARIES
        BobUI::DBus
)

bobui_internal_extend_target(Gui CONDITION UNIX AND BOBUI_FEATURE_settings AND BOBUI_FEATURE_dbus AND (BOBUI_FEATURE_xcb OR BOBUI_FEATURE_wayland)
    SOURCES
        platform/unix/qkdetheme_p.h platform/unix/qkdetheme.cpp
)

bobui_internal_extend_target(Gui CONDITION UNIX AND (((BOBUI_FEATURE_xcb OR NOT MACOS) AND (BOBUI_FEATURE_xcb OR NOT UIKIT)) OR BOBUI_FEATURE_wayland)
    SOURCES
        platform/unix/qgenericunixtheme.cpp platform/unix/qgenericunixtheme_p.h
        platform/unix/qgnometheme_p.h platform/unix/qgnometheme.cpp
)

bobui_internal_extend_target(Gui CONDITION TARGET BobUI::DBus AND UNIX AND (BOBUI_FEATURE_xcb OR NOT MACOS) AND (BOBUI_FEATURE_xcb OR NOT UIKIT)
    SOURCES
        platform/unix/dbusmenu/qdbusmenuadaptor.cpp platform/unix/dbusmenu/qdbusmenuadaptor_p.h
        platform/unix/dbusmenu/qdbusmenubar.cpp platform/unix/dbusmenu/qdbusmenubar_p.h
        platform/unix/dbusmenu/qdbusmenuconnection.cpp platform/unix/dbusmenu/qdbusmenuconnection_p.h
        platform/unix/dbusmenu/qdbusmenuregistrarproxy.cpp platform/unix/dbusmenu/qdbusmenuregistrarproxy_p.h
        platform/unix/dbusmenu/qdbusmenutypes.cpp platform/unix/dbusmenu/qdbusmenutypes_p.h
        platform/unix/dbusmenu/qdbusplatformmenu.cpp platform/unix/dbusmenu/qdbusplatformmenu_p.h
        platform/unix/qdbuslistener_p.h platform/unix/qdbuslistener.cpp
        platform/unix/qdbussettings_p.h platform/unix/qdbussettings.cpp
        platform/unix/qgnomeportalinterface.cpp platform/unix/qgnomeportalinterface_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_systemtrayicon AND TARGET BobUI::DBus AND UNIX AND (BOBUI_FEATURE_xcb OR NOT MACOS) AND (BOBUI_FEATURE_xcb OR NOT UIKIT)
    SOURCES
        platform/unix/dbustray/qdbustrayicon.cpp platform/unix/dbustray/qdbustrayicon_p.h
        platform/unix/dbustray/qdbustraytypes.cpp platform/unix/dbustray/qdbustraytypes_p.h
        platform/unix/dbustray/qstatusnotifieritemadaptor.cpp platform/unix/dbustray/qstatusnotifieritemadaptor_p.h
        platform/unix/dbustray/qxdgnotificationproxy.cpp platform/unix/dbustray/qxdgnotificationproxy_p.h
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_xkbcommon AND UNIX
    SOURCES
        platform/unix/qxkbcommon.cpp platform/unix/qxkbcommon_p.h
        platform/unix/qxkbcommon_3rdparty.cpp
    LIBRARIES
        XKB::XKB
    PRIVATE_MODULE_INTERFACE
        XKB::XKB
)

bobui_internal_extend_target(Gui CONDITION BOBUI_FEATURE_metal
    SOURCES
        rhi/qrhimetal.mm rhi/qrhimetal_p.h
    PUBLIC_LIBRARIES
        ${FWMetal}
)

bobui_internal_extend_target(Gui
    SOURCES
        painting/qdrawhelper.cpp painting/qdrawhelper_neon_p.h
    NO_PCH_SOURCES
        "painting/qdrawhelper.cpp"
)

bobui_internal_extend_target(Gui CONDITION (BOBUI_FEATURE_eglfs OR BOBUI_FEATURE_xcb OR BOBUI_FEATURE_direct2d OR WIN32)
    SOURCES
        util/qedidparser.cpp util/qedidparser_p.h
        util/qedidvendortable_p.h
)

bobui_internal_generate_tracepoints(Gui gui
    SOURCES
        image/qimage.cpp image/qimagereader.cpp image/qpixmap.cpp kernel/qguiapplication.cpp
        text/qfontdatabase.cpp
)
bobui_internal_add_docs(Gui
    doc/bobuigui.qdocconf
)

if(IOS)
    bobui_internal_set_apple_privacy_manifest(Gui
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/ios/PrivacyInfo.xcprivacy")
endif()

bobui_internal_add_optimize_full_flags()

if(BOBUI_FEATURE_doc_snippets)
    add_subdirectory(doc/snippets)
endif()
