# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs

# input freetype
set(INPUT_freetype "undefined" CACHE STRING "")
set_property(CACHE INPUT_freetype PROPERTY STRINGS undefined no bobui system)

# input harfbuzz
set(INPUT_harfbuzz "undefined" CACHE STRING "")
set_property(CACHE INPUT_harfbuzz PROPERTY STRINGS undefined no bobui system)

# input libjpeg
set(INPUT_libjpeg "undefined" CACHE STRING "")
set_property(CACHE INPUT_libjpeg PROPERTY STRINGS undefined no bobui system)

# input libmd4c
set(INPUT_libmd4c "undefined" CACHE STRING "")
set_property(CACHE INPUT_libmd4c PROPERTY STRINGS undefined no bobui system)

# input libpng
set(INPUT_libpng "undefined" CACHE STRING "")
set_property(CACHE INPUT_libpng PROPERTY STRINGS undefined no bobui system)



#### Libraries
if(LINUX OR HPUX OR FREEBSD OR NETBSD OR OPENBSD OR SOLARIS OR HURD)
    set(X11_SUPPORTED 1)
else()
    set(X11_SUPPORTED 0)
endif()
bobui_feature_vcpkg_scope(gui)
bobui_find_package(ATSPI2 MODULE PROVIDED_TARGETS PkgConfig::ATSPI2 MODULE_NAME gui QMAKE_LIB atspi)
bobui_find_package(DirectFB PROVIDED_TARGETS PkgConfig::DirectFB MODULE_NAME gui QMAKE_LIB directfb)
bobui_find_package(Libdrm MODULE PROVIDED_TARGETS Libdrm::Libdrm MODULE_NAME gui QMAKE_LIB drm)
bobui_find_package(PlatformGraphics
        PROVIDED_TARGETS PlatformGraphics::PlatformGraphics
        MODULE_NAME gui QMAKE_LIB platform_graphics)
bobui_find_package(EGL MODULE PROVIDED_TARGETS EGL::EGL MODULE_NAME gui QMAKE_LIB egl)

bobui_find_package(WrapSystemFreetype 2.2.0 MODULE
    PROVIDED_TARGETS WrapSystemFreetype::WrapSystemFreetype MODULE_NAME gui QMAKE_LIB freetype
    VCPKG_PORT freetype
    VCPKG_ADD_TO_FEATURE freetype
)
if(BOBUI_FEATURE_system_zlib)
    bobui_add_qmake_lib_dependency(freetype zlib)
endif()
bobui_find_package(Fontconfig PROVIDED_TARGETS Fontconfig::Fontconfig MODULE_NAME gui
    QMAKE_LIB fontconfig
    VCPKG_PORT fontconfig
    VCPKG_ADD_TO_FEATURE fontconfig
    VCPKG_PLATFORM "linux"
)
bobui_add_qmake_lib_dependency(fontconfig freetype)
bobui_find_package(gbm MODULE PROVIDED_TARGETS gbm::gbm MODULE_NAME gui QMAKE_LIB gbm)
bobui_find_package(WrapSystemHarfbuzz 2.6.0 MODULE
    PROVIDED_TARGETS WrapSystemHarfbuzz::WrapSystemHarfbuzz MODULE_NAME gui QMAKE_LIB harfbuzz
    VCPKG_PORT harfbuzz
    VCPKG_ADD_TO_FEATURE harfbuzz
)
bobui_find_package(Libinput MODULE
    PROVIDED_TARGETS Libinput::Libinput MODULE_NAME gui QMAKE_LIB libinput)
bobui_find_package_extend_sbom(TARGETS Libinput::Libinput
    COPYRIGHTS
        "Copyright © 2006-2009 Simon Thum"
        "Copyright © 2008-2012 Kristian Høgsberg"
        "Copyright © 2010-2012 Intel Corporation"
        "Copyright © 2010-2011 Benjamin Franzke"
        "Copyright © 2011-2012 Collabora, Ltd."
        "Copyright © 2013-2014 Jonas Ådahl"
        "Copyright © 2013-2015 Red Hat, Inc."
)
bobui_find_package(WrapSystemJpeg MODULE
    PROVIDED_TARGETS WrapSystemJpeg::WrapSystemJpeg MODULE_NAME gui QMAKE_LIB libjpeg
    VCPKG_PORT libjpeg-turbo
    VCPKG_ADD_TO_FEATURE jpeg
)
bobui_find_package(WrapSystemMd4c MODULE
    PROVIDED_TARGETS WrapSystemMd4c::WrapSystemMd4c MODULE_NAME gui QMAKE_LIB libmd4c
    VCPKG_PORT md4c
    VCPKG_ADD_TO_FEATURE textmarkdownreader
)
bobui_find_package(WrapSystemPNG MODULE
    PROVIDED_TARGETS WrapSystemPNG::WrapSystemPNG MODULE_NAME gui QMAKE_LIB libpng
    VCPKG_PORT libpng
    VCPKG_ADD_TO_FEATURE png
)
if(BOBUI_FEATURE_system_zlib)
    bobui_add_qmake_lib_dependency(libpng zlib)
endif()
bobui_find_package(Mtdev MODULE PROVIDED_TARGETS PkgConfig::Mtdev MODULE_NAME gui QMAKE_LIB mtdev)
bobui_find_package(WrapOpenGL MODULE PROVIDED_TARGETS WrapOpenGL::WrapOpenGL MODULE_NAME gui QMAKE_LIB opengl)
bobui_find_package(GLESv2 MODULE
    PROVIDED_TARGETS GLESv2::GLESv2 MODULE_NAME gui QMAKE_LIB opengl_es2)
bobui_find_package(Tslib MODULE PROVIDED_TARGETS PkgConfig::Tslib MODULE_NAME gui QMAKE_LIB tslib)
bobui_find_package(WrapVulkanHeaders MODULE PROVIDED_TARGETS WrapVulkanHeaders::WrapVulkanHeaders
    MODULE_NAME gui QMAKE_LIB vulkan MARK_OPTIONAL)
if(LINUX OR FREEBSD OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(Wayland MODULE PROVIDED_TARGETS Wayland::Server
                    MODULE_NAME gui QMAKE_LIB wayland_server)
    bobui_find_package(Wayland MODULE PROVIDED_TARGETS Wayland::Client
                    MODULE_NAME gui QMAKE_LIB wayland_client)
    # Gui doesn't use these, but the wayland qpa plugin does, and we
    # need to list the rest of the provided targets here, so they are
    # promoted to global, and can be accessed by the SBOM at the root
    # project level. That's not possible to do in the wayland qpa subdir,
    # due to different cmake directory scopes.
    bobui_find_package(Wayland MODULE PROVIDED_TARGETS Wayland::Cursor
                    MODULE_NAME gui QMAKE_LIB wayland_cursor)
    bobui_find_package(Wayland MODULE PROVIDED_TARGETS Wayland::Egl
                    MODULE_NAME gui QMAKE_LIB wayland_egl)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(X11 MODULE PROVIDED_TARGETS X11::X11 MODULE_NAME gui QMAKE_LIB xlib)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(X11 MODULE PROVIDED_TARGETS X11::SM X11::ICE MODULE_NAME gui QMAKE_LIB x11sm)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB 1.11 MODULE
        COMPONENTS XCB PROVIDED_TARGETS XCB::XCB MODULE_NAME gui QMAKE_LIB xcb)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS CURSOR PROVIDED_TARGETS XCB::CURSOR MODULE_NAME gui QMAKE_LIB xcb_cursor)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS ICCCM PROVIDED_TARGETS XCB::ICCCM MODULE_NAME gui QMAKE_LIB xcb_icccm)
endif()
bobui_add_qmake_lib_dependency(xcb_icccm xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS UTIL PROVIDED_TARGETS XCB::UTIL MODULE_NAME gui QMAKE_LIB xcb_util)
endif()
bobui_add_qmake_lib_dependency(xcb_util xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS IMAGE PROVIDED_TARGETS XCB::IMAGE MODULE_NAME gui QMAKE_LIB xcb_image)
endif()
bobui_add_qmake_lib_dependency(xcb_image xcb_shm xcb_util xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS KEYSYMS PROVIDED_TARGETS XCB::KEYSYMS MODULE_NAME gui QMAKE_LIB xcb_keysyms)
endif()
bobui_add_qmake_lib_dependency(xcb_keysyms xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS RENDERUTIL PROVIDED_TARGETS XCB::RENDERUTIL MODULE_NAME gui QMAKE_LIB xcb_renderutil)
endif()
bobui_add_qmake_lib_dependency(xcb_renderutil xcb xcb_render)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS RANDR PROVIDED_TARGETS XCB::RANDR MODULE_NAME gui QMAKE_LIB xcb_randr)
endif()
bobui_add_qmake_lib_dependency(xcb_randr xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS SHAPE PROVIDED_TARGETS XCB::SHAPE MODULE_NAME gui QMAKE_LIB xcb_shape)
endif()
bobui_add_qmake_lib_dependency(xcb_shape xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS SHM PROVIDED_TARGETS XCB::SHM MODULE_NAME gui QMAKE_LIB xcb_shm)
endif()
bobui_add_qmake_lib_dependency(xcb_shm xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS SYNC PROVIDED_TARGETS XCB::SYNC MODULE_NAME gui QMAKE_LIB xcb_sync)
endif()
bobui_add_qmake_lib_dependency(xcb_sync xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS XFIXES PROVIDED_TARGETS XCB::XFIXES MODULE_NAME gui QMAKE_LIB xcb_xfixes)
endif()
bobui_add_qmake_lib_dependency(xcb_xfixes xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(X11_XCB MODULE PROVIDED_TARGETS X11::XCB MODULE_NAME gui QMAKE_LIB xcb_xlib)
endif()
bobui_add_qmake_lib_dependency(xcb_xlib xcb xlib)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS XKB PROVIDED_TARGETS XCB::XKB MODULE_NAME gui QMAKE_LIB xcb_xkb)
endif()
bobui_add_qmake_lib_dependency(xcb_xkb xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS RENDER PROVIDED_TARGETS XCB::RENDER MODULE_NAME gui QMAKE_LIB xcb_render)
endif()
bobui_add_qmake_lib_dependency(xcb_render xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB MODULE
        COMPONENTS GLX PROVIDED_TARGETS XCB::GLX MODULE_NAME gui QMAKE_LIB xcb_glx)
endif()
bobui_add_qmake_lib_dependency(xcb_glx xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XCB 1.12 MODULE
        COMPONENTS XINPUT PROVIDED_TARGETS XCB::XINPUT MODULE_NAME gui QMAKE_LIB xcb_xinput)
endif()
bobui_add_qmake_lib_dependency(xcb_xinput xcb)
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XKB 0.9.0 MODULE PROVIDED_TARGETS XKB::XKB MODULE_NAME gui QMAKE_LIB xkbcommon)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XKB_COMMON_X11 0.9.0 MODULE
        PROVIDED_TARGETS PkgConfig::XKB_COMMON_X11 MODULE_NAME gui QMAKE_LIB xkbcommon_x11)
endif()
if((X11_SUPPORTED) OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(XRender 0.6 MODULE
        PROVIDED_TARGETS PkgConfig::XRender MODULE_NAME gui QMAKE_LIB xrender)
endif()
bobui_add_qmake_lib_dependency(xrender xlib)

# bobui wayland client
if(LINUX OR FREEBSD OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    # EGL
    if(NOT TARGET EGL::EGL)
        bobui_find_package(EGL MODULE
            PROVIDED_TARGETS EGL::EGL MODULE_NAME gui QMAKE_LIB egl MARK_OPTIONAL)
    endif()
    # and Libdrm
    if(NOT TARGET Libdrm::Libdrm)
        bobui_find_package(Libdrm MODULE
            PROVIDED_TARGETS Libdrm::Libdrm
            MODULE_NAME gui
            QMAKE_LIB drm
            MARK_OPTIONAL)
    endif()
endif()

bobui_find_package(Wayland 1.15 MODULE)
bobui_find_package(WaylandScanner MODULE PROVIDED_TARGETS Wayland::Scanner)

bobui_find_package(RenderDoc MODULE PROVIDED_TARGETS RenderDoc::RenderDoc)

#### Tests

if(TARGET PlatformGraphics::PlatformGraphics)
    set(plaform_graphics_libs PlatformGraphics::PlatformGraphics)
else()
    set(plaform_graphics_libs "")
endif()

# drm_atomic
bobui_config_compile_test(drm_atomic
    LABEL "DRM Atomic API"
    LIBRARIES
        Libdrm::Libdrm
    CODE
"#include <stdlib.h>
#include <stdint.h>
extern \"C\" {
#include <xf86drmMode.h>
#include <xf86drm.h>
}

int main(void)
{
    /* BEGIN TEST: */
drmModeAtomicReq *request;
    /* END TEST: */
    return 0;
}
")

# egl-x11
bobui_config_compile_test(egl_x11
    LABEL "EGL on X11"
    LIBRARIES
        EGL::EGL
        X11::X11
        ${plaform_graphics_libs}
    CODE
"// Check if EGL is compatible with X. Some EGL implementations, typically on
// embedded devices, are not intended to be used together with X. EGL support
// has to be disabled in plugins like xcb in this case since the native display,
// window and pixmap types will be different than what an X-based platform
// plugin would expect.
#define USE_X11
#include <EGL/egl.h>
#include <X11/Xlib.h>

int main(void)
{
    /* BEGIN TEST: */
Display *dpy = EGL_DEFAULT_DISPLAY;
EGLNativeDisplayType egldpy = XOpenDisplay(\"\");
dpy = egldpy;
EGLNativeWindowType w = XCreateWindow(dpy, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
XDestroyWindow(dpy, w);
XCloseDisplay(dpy);
    /* END TEST: */
    return 0;
}
")

# egl-brcm
bobui_config_compile_test(egl_brcm
    LABEL "Broadcom EGL (Raspberry Pi)"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    CODE
"#include <EGL/egl.h>
#include <bcm_host.h>

int main(void)
{
    /* BEGIN TEST: */
vc_dispmanx_display_open(0);
    /* END TEST: */
    return 0;
}
"# FIXME: use: unmapped library: bcm_host
)

# egl-egldevice
bobui_config_compile_test(egl_egldevice
    LABEL "EGLDevice"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    CODE
"#include <EGL/egl.h>
#include <EGL/eglext.h>

int main(void)
{
    /* BEGIN TEST: */
EGLDeviceEXT device = 0;
EGLStreamKHR stream = 0;
EGLOutputLayerEXT layer = 0;
(void) EGL_DRM_CRTC_EXT;
(void) EGL_DRM_MASTER_FD_EXT;
    /* END TEST: */
    return 0;
}
")

# egl-mali
bobui_config_compile_test(egl_mali
    LABEL "Mali EGL"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    CODE
"#include <EGL/fbdev_window.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

int main(void)
{
    /* BEGIN TEST: */
fbdev_window *w = 0;
    /* END TEST: */
    return 0;
}
")

# egl-mali-2
bobui_config_compile_test(egl_mali_2
    LABEL "Mali 2 EGL"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    CODE
"#include <EGL/egl.h>
#include <GLES2/gl2.h>

int main(void)
{
    /* BEGIN TEST: */
mali_native_window *w = 0;
    /* END TEST: */
    return 0;
}
")

# egl-viv

bobui_config_compile_test(egl_viv
    LABEL "i.Mx6 EGL"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    COMPILE_OPTIONS
        "-DEGL_API_FB=1"
    CODE
"#include <EGL/egl.h>
#include <EGL/eglvivante.h>

int main(void)
{
    /* BEGIN TEST: */
#ifdef __INTEGRITY
fbGetDisplay();
#else
// Do not rely on fbGetDisplay(), since the signature has changed over time.
// Stick to fbGetDisplayByIndex().
fbGetDisplayByIndex(0);
#endif
    /* END TEST: */
    return 0;
}
"# FIXME: qmake: ['DEFINES += EGL_API_FB=1', '!integrity: DEFINES += LINUX=1']
)

# egl-openwfd
bobui_config_compile_test(egl_openwfd
    LABEL "OpenWFD EGL"
    LIBRARIES
        EGL::EGL
        ${plaform_graphics_libs}
    CODE
"#include <wfd.h>

int main(void)
{
    /* BEGIN TEST: */
wfdEnumerateDevices(nullptr, 0, nullptr);
    /* END TEST: */
    return 0;
}
")

# egl-rcar
bobui_config_compile_test(egl_rcar
    LABEL "RCAR EGL"
    LIBRARIES
        EGL::EGL
        GLESv2::GLESv2
        ${plaform_graphics_libs}
    CODE
"#include <EGL/egl.h>
extern \"C\" {
extern unsigned long PVRGrfxServerInit(void);
}

int main(void)
{
    /* BEGIN TEST: */
PVRGrfxServerInit();
    /* END TEST: */
    return 0;
}
")

# evdev
bobui_config_compile_test(evdev
    LABEL "evdev"
    CODE
"#if defined(__FreeBSD__)
#  include <dev/evdev/input.h>
#else
#  include <linux/input.h>
#  include <linux/kd.h>
#endif
enum {
    e1 = ABS_PRESSURE,
    e2 = ABS_X,
    e3 = REL_X,
    e4 = SYN_REPORT,
};

int main(void)
{
    /* BEGIN TEST: */
input_event buf[32];
(void) buf;
    /* END TEST: */
    return 0;
}
")

# vxworksevdev
bobui_config_compile_test(vxworksevdev
    LABEL "VxWorks evdev"
"#include <evdevLib.h>
enum {
    e1 = EV_DEV_ABS,
    e2 = EV_DEV_PTR_ABS_X,
    e3 = EV_DEV_PTR_ABS_Y,
    e4 = EV_DEV_PTR_BTN_TOUCH,
};

int main(void)
{
    /* BEGIN TEST: */
EV_DEV_EVENT buf[32];
(void) buf;
    /* END TEST: */
    return 0;
}")

# integrityfb
bobui_config_compile_test(integrityfb
    LABEL "INTEGRITY framebuffer"
    CODE
"#include <device/fbdriver.h>

int main(void)
{
    /* BEGIN TEST: */
FBDriver *driver = 0;
    /* END TEST: */
    return 0;
}
")

# linuxfb
bobui_config_compile_test(linuxfb
    LABEL "LinuxFB"
    CODE
"#include <linux/fb.h>
#include <sys/kd.h>
#include <sys/ioctl.h>

int main(void)
{
    /* BEGIN TEST: */
fb_fix_screeninfo finfo;
fb_var_screeninfo vinfo;
int fd = 3;
ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    /* END TEST: */
    return 0;
}
")

# opengles3
if(WASM)
    set(extra_compiler_options "-s FULL_ES3=1")
endif()

bobui_config_compile_test(opengles3
    LABEL "OpenGL ES 3.0"
    LIBRARIES
        GLESv2::GLESv2
        ${plaform_graphics_libs}
    COMPILE_OPTIONS ${extra_compiler_options}
    CODE
"#ifdef __APPLE__
#  include <OpenGLES/ES3/gl.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GLES3/gl3.h>
#endif

int main(void)
{
    /* BEGIN TEST: */
static GLfloat f[6];
glGetStringi(GL_EXTENSIONS, 0);
glReadBuffer(GL_COLOR_ATTACHMENT1);
glUniformMatrix2x3fv(0, 0, GL_FALSE, f);
glMapBufferRange(GL_ARRAY_BUFFER, 0, 0, GL_MAP_READ_BIT);
    /* END TEST: */
    return 0;
}
")


# opengles31
bobui_config_compile_test(opengles31
    LABEL "OpenGL ES 3.1"
    LIBRARIES
        GLESv2::GLESv2
        ${plaform_graphics_libs}
    CODE
"#include <GLES3/gl31.h>

int main(void)
{
    /* BEGIN TEST: */
glDispatchCompute(1, 1, 1);
glProgramUniform1i(0, 0, 0);
    /* END TEST: */
    return 0;
}
")

# opengles32
bobui_config_compile_test(opengles32
    LABEL "OpenGL ES 3.2"
    LIBRARIES
        GLESv2::GLESv2
        ${plaform_graphics_libs}
    CODE
"#include <GLES3/gl32.h>

int main(void)
{
    /* BEGIN TEST: */
glFramebufferTexture(GL_TEXTURE_2D, GL_DEPTH_STENCIL_ATTACHMENT, 1, 0);
    /* END TEST: */
    return 0;
}
")

# xcb_syslibs
bobui_config_compile_test(xcb_syslibs
    LABEL "XCB (extensions)"
    LIBRARIES
        XCB::CURSOR
        XCB::ICCCM
        XCB::UTIL
        XCB::IMAGE
        XCB::KEYSYMS
        XCB::RANDR
        XCB::RENDER
        XCB::RENDERUTIL
        XCB::SHAPE
        XCB::SHM
        XCB::SYNC
        XCB::XFIXES
        XCB::XKB
        XCB::XCB
    CODE
"// xkb.h is using a variable called 'explicit', which is a reserved keyword in C++
#define explicit dont_use_cxx_explicit
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_cursor.h>
#include <xcb/randr.h>
#include <xcb/render.h>
#include <xcb/shape.h>
#include <xcb/shm.h>
#include <xcb/sync.h>
#include <xcb/xfixes.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_renderutil.h>
#include <xcb/xkb.h>
#undef explicit

int main(void)
{
    /* BEGIN TEST: */
int primaryScreen = 0;
xcb_connection_t *c = xcb_connect(\"\", &primaryScreen);
/* RENDER */
xcb_generic_error_t *error = nullptr;
xcb_render_query_pict_formats_cookie_t formatsCookie =
    xcb_render_query_pict_formats(c);
xcb_render_query_pict_formats_reply_t *formatsReply =
    xcb_render_query_pict_formats_reply(c, formatsCookie, &error);
/* RENDERUTIL: xcb_renderutil.h include won't compile unless version >= 0.3.9 */
xcb_render_util_find_standard_format(nullptr, XCB_PICT_STANDARD_ARGB_32);
/* XKB: This takes more arguments in xcb-xkb < 1.11 */
xcb_xkb_get_kbd_by_name_replies_key_names_value_list_sizeof(nullptr, 0, 0, 0, 0, 0, 0, 0, 0);
    /* END TEST: */
    return 0;
}
")

# libinput_hires_wheel_support
bobui_config_compile_test(libinput_hires_wheel_support
    LABEL "libinput hires wheel support"
    LIBRARIES
        Libinput::Libinput
    CODE
"#include <libinput.h>
int main(void)
{
    /* BEGIN TEST: */
libinput_event_type type = LIBINPUT_EVENT_POINTER_SCROLL_WHEEL;
libinput_event_pointer_get_scroll_value_v120(nullptr, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL);
    /* END TEST: */
    return 0;
}
")

# directwrite (assumes DirectWrite2)
bobui_config_compile_test(directwrite
    LABEL "WINDOWS directwrite"
    LIBRARIES
        dwrite
    CODE
"#include <dwrite_2.h>
int main(int, char **)
{
    IUnknown *factory = nullptr;
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2),
                        &factory);
    return 0;
}
")

# directwrite3 (not present in MinGW)
bobui_config_compile_test(directwrite3
    LABEL "WINDOWS directwrite3"
    LIBRARIES
        dwrite
    CODE
"#include <dwrite_3.h>
int main(int, char **)
{
    IUnknown *factory = nullptr;
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory6),
                        &factory);
    return 0;
}
")

# directwritecolrv1
bobui_config_compile_test(directwritecolrv1
    LABEL "WINDOWS directwritecolrv1"
    LIBRARIES
        dwrite
    CODE
"#include <dwrite_3.h>
int main(int, char **)
{
    IUnknown *factory = nullptr;
    // Just check that the API is available for the build
    DWRITE_PAINT_ELEMENT paintElement;

    if (false) {
        DWRITE_COLOR_F dwColor;
        dwColor.r = 0;
        dwColor.g = 0;
        dwColor.b = 0;
        dwColor.a = 0;
        IDWritePaintReader *paintReader = nullptr;
        // Some versions of MinGW has a dwrite_3.h header with buggy generated APIs. One of these
        // is that the SetTextColor() function takes a pointer instead of a const-ref. We check
        // for this to disable the COLRv1 feature for broken headers.
        paintReader->SetTextColor(dwColor);
    }

    return 0;
}
")

bobui_config_compile_test(d2d1
    LABEL "WINDOWS Direct2D"
    LIBRARIES
        d2d1
    CODE
"#include <d2d1.h>
int main(int, char **)
{
    void *factory = nullptr;
    D2D1_FACTORY_OPTIONS options;
    ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, GUID{}, &options, &factory);
    return 0;
}
")

bobui_config_compile_test(d2d1_1
    LABEL "WINDOWS Direct2D 1.1"
    LIBRARIES
        d2d1
    CODE
"#include <d2d1_1.h>
int main(int, char **)
{
    ID2D1Factory1 *d2dFactory;
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
    return 0;
}
")

bobui_config_compile_test(renderdoc
    LIBRARIES
        RenderDoc::RenderDoc
    LABEL "RenderDoc header check"
    CODE
"#include <renderdoc_app.h>
int main(int, char **)
{
    if (RENDERDOC_Version::eRENDERDOC_API_Version_1_6_0)
        return 0;
    return 0;
}
")

# bobuiwayland client
# drm-egl-server
bobui_config_compile_test(drm_egl_server
    LABEL "DRM EGL Server"
    LIBRARIES
    EGL::EGL
    CODE
    "
#include <EGL/egl.h>
#include <EGL/eglext.h>

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    /* BEGIN TEST: */
#ifdef EGL_MESA_drm_image
return 0;
#else
#error Requires EGL_MESA_drm_image to be defined
return 1;
#endif
    /* END TEST: */
    return 0;
}
")

# libhybris-egl-server
bobui_config_compile_test(libhybris_egl_server
    LABEL "libhybris EGL Server"
    LIBRARIES
    EGL::EGL
    CODE
    "
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <hybris/eglplatformcommon/hybris_nativebufferext.h>

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    /* BEGIN TEST: */
#ifdef EGL_HYBRIS_native_buffer
return 0;
#else
#error Requires EGL_HYBRIS_native_buffer to be defined
return 1;
#endif
    /* END TEST: */
    return 0;
}
")

# dmabuf-server-buffer
bobui_config_compile_test(dmabuf_server_buffer
    LABEL "Linux dma-buf Buffer Sharing"
    LIBRARIES
    EGL::EGL
    Libdrm::Libdrm
    CODE
    "
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <drm_fourcc.h>

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    /* BEGIN TEST: */
#ifdef EGL_LINUX_DMA_BUF_EXT
return 0;
#else
#error Requires EGL_LINUX_DMA_BUF_EXT
return 1;
#endif
    /* END TEST: */
    return 0;
}
")

# vulkan-server-buffer
bobui_config_compile_test(vulkan_server_buffer
    LABEL "Vulkan Buffer Sharing"
    LIBRARIES
    Wayland::Client
    CODE
    "#define VK_USE_PLATFORM_WAYLAND_KHR 1
#include <vulkan/vulkan.h>

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    /* BEGIN TEST: */
VkExportMemoryAllocateInfoKHR exportAllocInfo = {};
exportAllocInfo.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_KHR;
exportAllocInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
return 0;
    /* END TEST: */
    return 0;
}
")

# egl_1_5-wayland
bobui_config_compile_test(egl_1_5_wayland
    LABEL "EGL 1.5 with Wayland Platform"
    LIBRARIES
    EGL::EGL
    Wayland::Client
    CODE
    "
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-client.h>

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    /* BEGIN TEST: */
eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_EXT, (struct wl_display *)(nullptr), nullptr);
    /* END TEST: */
    return 0;
}
")

#### Features

bobui_feature("accessibility-atspi-bridge" PUBLIC PRIVATE
    LABEL "ATSPI Bridge"
    CONDITION BOBUI_FEATURE_accessibility AND BOBUI_FEATURE_dbus AND ATSPI2_FOUND
)
bobui_feature_definition("accessibility-atspi-bridge" "BOBUI_NO_ACCESSIBILITY_ATSPI_BRIDGE" NEGATE VALUE "1")
bobui_feature("directfb" PRIVATE
    SECTION "Platform plugins"
    LABEL "DirectFB"
    AUTODETECT OFF
    CONDITION DirectFB_FOUND
)
bobui_feature("directwrite" PRIVATE
    LABEL "DirectWrite"
    CONDITION TEST_directwrite
    EMIT_IF WIN32
)
bobui_feature("directwrite3" PRIVATE
    LABEL "DirectWrite 3"
    CONDITION BOBUI_FEATURE_directwrite AND TEST_directwrite3
    EMIT_IF WIN32
)
bobui_feature("directwritecolrv1" PRIVATE
    LABEL "DirectWrite COLRv1 Support"
    CONDITION BOBUI_FEATURE_directwrite3 AND TEST_directwritecolrv1
    EMIT_IF WIN32
)
bobui_feature("direct2d" PRIVATE
    LABEL "Direct 2D"
    CONDITION WIN32 AND NOT WINRT AND TEST_d2d1
)
bobui_feature("direct2d1_1" PRIVATE
    LABEL "Direct 2D 1.1"
    CONDITION BOBUI_FEATURE_direct2d AND TEST_d2d1_1
)
bobui_feature("emojisegmenter" PUBLIC PRIVATE
    SECTION "Fonts"
    LABEL "Emoji Segmenter"
    PURPOSE "Supports parsing complex emoji sequences for better font resolution."
)
bobui_feature_definition("emojisegmenter" "BOBUI_NO_EMOJISEGMENTER" NEGATE VALUE "1")
bobui_feature("evdev" PRIVATE
    LABEL "evdev"
    CONDITION BOBUI_FEATURE_thread AND TEST_evdev
)
bobui_feature("vxworksevdev" PRIVATE
    LABEL "vxworksevdev"
    CONDITION BOBUI_FEATURE_thread AND TEST_vxworksevdev
)
bobui_feature("freetype" PUBLIC PRIVATE
    SECTION "Fonts"
    LABEL "FreeType"
    PURPOSE "Supports the FreeType 2 font engine (and its supported font formats)."
)
bobui_feature_definition("freetype" "BOBUI_NO_FREETYPE" NEGATE VALUE "1")
bobui_feature("system-freetype" PRIVATE SYSTEM_LIBRARY
    LABEL "  Using system FreeType"
    AUTODETECT NOT MSVC
    CONDITION BOBUI_FEATURE_freetype AND WrapSystemFreetype_FOUND
    ENABLE INPUT_freetype STREQUAL 'system'
    DISABLE INPUT_freetype STREQUAL 'bobui'
)
bobui_feature("fontconfig" PUBLIC PRIVATE
    LABEL "Fontconfig"
    AUTODETECT NOT APPLE
    CONDITION NOT APPLE AND NOT WIN32 AND BOBUI_FEATURE_system_freetype AND Fontconfig_FOUND
)
bobui_feature_definition("fontconfig" "BOBUI_NO_FONTCONFIG" NEGATE VALUE "1")
bobui_feature("gbm"
    LABEL "GBM"
    CONDITION gbm_FOUND
)
bobui_feature_config("gbm" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("harfbuzz" PUBLIC PRIVATE
    LABEL "HarfBuzz"
)
bobui_feature_definition("harfbuzz" "BOBUI_NO_HARFBUZZ" NEGATE VALUE "1")
bobui_feature("system-harfbuzz" PRIVATE SYSTEM_LIBRARY
    LABEL "  Using system HarfBuzz"
    AUTODETECT NOT APPLE AND NOT WIN32
    CONDITION BOBUI_FEATURE_harfbuzz AND WrapSystemHarfbuzz_FOUND
    ENABLE INPUT_harfbuzz STREQUAL 'system'
    DISABLE INPUT_harfbuzz STREQUAL 'bobui'
)
bobui_feature("qqnx_imf" PRIVATE
    LABEL "IMF"
    CONDITION libs.imf OR FIXME
    EMIT_IF QNX
)
bobui_feature("integrityfb" PRIVATE
    SECTION "Platform plugins"
    LABEL "INTEGRITY framebuffer"
    CONDITION INTEGRITY AND TEST_integrityfb
)
bobui_feature("kms" PRIVATE
    LABEL "KMS"
    CONDITION Libdrm_FOUND
)
bobui_feature_config("kms" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("drm_atomic" PRIVATE
    LABEL "DRM Atomic API"
    CONDITION Libdrm_FOUND AND TEST_drm_atomic
)
bobui_feature("libinput" PRIVATE
    LABEL "libinput"
    CONDITION BOBUI_FEATURE_libudev AND Libinput_FOUND
)
bobui_feature("integrityhid" PRIVATE
    LABEL "INTEGRITY HID"
    CONDITION INTEGRITY AND libs.integrityhid OR FIXME
)
bobui_feature("libinput-axis-api" PRIVATE
    LABEL "axis API in libinput"
    CONDITION BOBUI_FEATURE_libinput AND ON
)
bobui_feature("libinput-hires-wheel-support" PRIVATE
    LABEL "HiRes wheel support in libinput"
    CONDITION BOBUI_FEATURE_libinput AND TEST_libinput_hires_wheel_support
)
bobui_feature("lgmon"
    LABEL "lgmon"
    CONDITION libs.lgmon OR FIXME
    EMIT_IF QNX
)
bobui_feature_config("lgmon" QMAKE_PRIVATE_CONFIG)
bobui_feature("linuxfb" PRIVATE
    SECTION "Platform plugins"
    LABEL "LinuxFB"
    CONDITION TEST_linuxfb AND BOBUI_FEATURE_regularexpression
)
bobui_feature("vsp2" PRIVATE
    LABEL "VSP2"
    AUTODETECT OFF
    CONDITION libs.v4l2 OR FIXME
)
bobui_feature("vnc" PRIVATE
    SECTION "Platform plugins"
    LABEL "VNC"
    CONDITION ( UNIX AND NOT ANDROID AND NOT APPLE AND NOT WASM ) AND ( BOBUI_FEATURE_regularexpression AND BOBUI_FEATURE_network )
)
bobui_feature("mtdev" PRIVATE
    LABEL "mtdev"
    CONDITION Mtdev_FOUND
)
bobui_feature("opengles2" PUBLIC
    LABEL "OpenGL ES 2.0"
    CONDITION NOT WIN32 AND NOT WATCHOS AND NOT BOBUI_FEATURE_opengl_desktop AND GLESv2_FOUND
    ENABLE INPUT_opengl STREQUAL 'es2'
    DISABLE INPUT_opengl STREQUAL 'desktop' OR INPUT_opengl STREQUAL 'dynamic' OR INPUT_opengl STREQUAL 'no'
)
bobui_feature_config("opengles2" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("opengles3" PUBLIC
    LABEL "OpenGL ES 3.0"
    CONDITION BOBUI_FEATURE_opengles2 AND TEST_opengles3
)
bobui_feature("opengles31" PUBLIC
    LABEL "OpenGL ES 3.1"
    CONDITION BOBUI_FEATURE_opengles3 AND TEST_opengles31
)
bobui_feature("opengles32" PUBLIC
    LABEL "OpenGL ES 3.2"
    CONDITION BOBUI_FEATURE_opengles31 AND TEST_opengles32
)
bobui_feature("opengl-desktop"
    LABEL "Desktop OpenGL"
    AUTODETECT NOT WIN32
    CONDITION ( WIN32 AND ( MSVC OR WrapOpenGL_FOUND ) ) OR ( NOT WATCHOS AND NOT WIN32 AND NOT WASM AND WrapOpenGL_FOUND )
    ENABLE INPUT_opengl STREQUAL 'desktop'
    DISABLE INPUT_opengl STREQUAL 'es2' OR INPUT_opengl STREQUAL 'dynamic' OR INPUT_opengl STREQUAL 'no'
)
bobui_feature("opengl-dynamic"
    LABEL "Dynamic OpenGL"
    CONDITION WIN32
    DISABLE INPUT_opengl STREQUAL 'no' OR INPUT_opengl STREQUAL 'desktop'
)
bobui_feature("dynamicgl" PUBLIC
    LABEL "Dynamic OpenGL: dynamicgl"
    CONDITION BOBUI_FEATURE_opengl_dynamic
    DISABLE INPUT_opengl STREQUAL 'no' OR INPUT_opengl STREQUAL 'desktop'
)
bobui_feature_definition("opengl-dynamic" "BOBUI_OPENGL_DYNAMIC")
bobui_feature("opengl" PUBLIC
    LABEL "OpenGL"
    CONDITION BOBUI_FEATURE_opengl_desktop OR BOBUI_FEATURE_opengl_dynamic OR BOBUI_FEATURE_opengles2
)
bobui_feature_definition("opengl" "BOBUI_NO_OPENGL" NEGATE VALUE "1")
bobui_feature("vkgen" PRIVATE
    LABEL "vkgen"
    CONDITION BOBUI_FEATURE_xmlstreamreader
)
bobui_feature("vulkan" PUBLIC
    LABEL "Vulkan"
    CONDITION BOBUI_FEATURE_library AND BOBUI_FEATURE_vkgen AND WrapVulkanHeaders_FOUND
)
bobui_feature("metal" PUBLIC
    LABEL "Metal"
    CONDITION MACOS OR IOS OR VISIONOS
)
bobui_feature("vkkhrdisplay" PRIVATE
    SECTION "Platform plugins"
    LABEL "VK_KHR_display"
    CONDITION NOT ANDROID AND NOT APPLE AND NOT WIN32 AND NOT WASM AND BOBUI_FEATURE_vulkan
)
bobui_feature("openvg" PUBLIC
    LABEL "OpenVG"
    CONDITION libs.openvg OR FIXME
)
bobui_feature("egl" PUBLIC
    LABEL "EGL"
    CONDITION ( BOBUI_FEATURE_opengl OR BOBUI_FEATURE_openvg ) AND EGL_FOUND AND ( BOBUI_FEATURE_dlopen OR NOT UNIX OR INTEGRITY OR VXWORKS)
)
bobui_feature_definition("egl" "BOBUI_NO_EGL" NEGATE VALUE "1")
bobui_feature("egl_x11" PRIVATE
    LABEL "EGL on X11"
    CONDITION BOBUI_FEATURE_thread AND BOBUI_FEATURE_egl AND TEST_egl_x11
)
bobui_feature("eglfs" PRIVATE
    SECTION "Platform plugins"
    LABEL "EGLFS"
    CONDITION NOT ANDROID AND NOT APPLE AND NOT WIN32 AND NOT WASM AND NOT QNX AND BOBUI_FEATURE_egl
)
bobui_feature("eglfs_brcm" PRIVATE
    LABEL "EGLFS Raspberry Pi"
    CONDITION BOBUI_FEATURE_eglfs AND TEST_egl_brcm
)
bobui_feature("eglfs_egldevice" PRIVATE
    LABEL "EGLFS EGLDevice"
    CONDITION BOBUI_FEATURE_eglfs AND TEST_egl_egldevice AND BOBUI_FEATURE_kms
)
bobui_feature("eglfs_gbm" PRIVATE
    LABEL "EGLFS GBM"
    CONDITION BOBUI_FEATURE_eglfs AND gbm_FOUND AND BOBUI_FEATURE_kms
)
bobui_feature("eglfs_vsp2" PRIVATE
    LABEL "EGLFS VSP2"
    CONDITION BOBUI_FEATURE_eglfs AND gbm_FOUND AND BOBUI_FEATURE_kms AND BOBUI_FEATURE_vsp2
)
bobui_feature("eglfs_mali" PRIVATE
    LABEL "EGLFS Mali"
    CONDITION BOBUI_FEATURE_eglfs AND ( TEST_egl_mali OR TEST_egl_mali_2 )
)
bobui_feature("eglfs_viv" PRIVATE
    LABEL "EGLFS i.Mx6"
    CONDITION BOBUI_FEATURE_eglfs AND TEST_egl_viv
)
bobui_feature("eglfs_rcar" PRIVATE
    LABEL "EGLFS RCAR"
    CONDITION INTEGRITY AND BOBUI_FEATURE_eglfs AND TEST_egl_rcar
)
bobui_feature("eglfs_viv_wl" PRIVATE
    LABEL "EGLFS i.Mx6 Wayland"
    CONDITION BOBUI_FEATURE_eglfs_viv AND TARGET Wayland::Server
)
bobui_feature("eglfs_openwfd" PRIVATE
    LABEL "EGLFS OpenWFD"
    CONDITION INTEGRITY AND BOBUI_FEATURE_eglfs AND TEST_egl_openwfd
)
bobui_feature("eglfs_x11" PRIVATE
    LABEL "EGLFS X11"
    CONDITION BOBUI_FEATURE_eglfs AND BOBUI_FEATURE_xcb_xlib AND BOBUI_FEATURE_egl_x11
)
bobui_feature("gif" PRIVATE
    LABEL "GIF"
    CONDITION BOBUI_FEATURE_imageformatplugin
)
bobui_feature_definition("gif" "BOBUI_NO_IMAGEFORMAT_GIF" NEGATE)
bobui_feature("ico" PUBLIC PRIVATE
    LABEL "ICO"
    CONDITION BOBUI_FEATURE_imageformatplugin
)
bobui_feature_definition("ico" "BOBUI_NO_ICO" NEGATE VALUE "1")
bobui_feature("jpeg" PRIVATE
    LABEL "JPEG"
    CONDITION BOBUI_FEATURE_imageformatplugin
    DISABLE INPUT_libjpeg STREQUAL 'no'
)
bobui_feature_definition("jpeg" "BOBUI_NO_IMAGEFORMAT_JPEG" NEGATE VALUE "1")
bobui_feature("system-jpeg" PRIVATE SYSTEM_LIBRARY
    LABEL "  Using system libjpeg"
    CONDITION BOBUI_FEATURE_jpeg AND JPEG_FOUND
    ENABLE INPUT_libjpeg STREQUAL 'system'
    DISABLE INPUT_libjpeg STREQUAL 'bobui'
)
bobui_feature("png" PRIVATE
    LABEL "PNG"
    DISABLE INPUT_libpng STREQUAL 'no'
)
bobui_feature_definition("png" "BOBUI_NO_IMAGEFORMAT_PNG" NEGATE)
bobui_feature("system-png" PRIVATE SYSTEM_LIBRARY
    LABEL "  Using system libpng"
    AUTODETECT BOBUI_FEATURE_system_zlib
    CONDITION BOBUI_FEATURE_png AND WrapSystemPNG_FOUND
    ENABLE INPUT_libpng STREQUAL 'system'
    DISABLE INPUT_libpng STREQUAL 'bobui'
)
bobui_feature("imageio-text-loading" PRIVATE
    LABEL "Image Text section loading"
)
bobui_feature_definition("imageio-text-loading" "BOBUI_NO_IMAGEIO_TEXT_LOADING" NEGATE)
bobui_feature("sessionmanager" PUBLIC
    SECTION "Kernel"
    LABEL "Session Management"
    PURPOSE "Provides an interface to the windowing system's session management."
)
bobui_feature_definition("sessionmanager" "BOBUI_NO_SESSIONMANAGER" NEGATE VALUE "1")
bobui_feature("tslib" PRIVATE
    LABEL "tslib"
    CONDITION Tslib_FOUND AND NOT INTEGRITY
)
bobui_feature("tuiotouch" PRIVATE
    LABEL "TuioTouch"
    PURPOSE "Provides the TuioTouch input plugin."
    CONDITION BOBUI_FEATURE_network AND BOBUI_FEATURE_udpsocket
)
bobui_feature("xcb" PUBLIC
    SECTION "Platform plugins"
    LABEL "XCB"
    AUTODETECT NOT APPLE
    CONDITION BOBUI_FEATURE_thread AND TARGET XCB::XCB AND TEST_xcb_syslibs AND BOBUI_FEATURE_xkbcommon_x11
)
bobui_feature("xcb-glx-plugin" PUBLIC
    LABEL "GLX Plugin"
    CONDITION BOBUI_FEATURE_xcb_xlib AND BOBUI_FEATURE_opengl AND NOT BOBUI_FEATURE_opengles2
    EMIT_IF BOBUI_FEATURE_xcb
)
bobui_feature("xcb-glx" PRIVATE
    LABEL "  XCB GLX"
    CONDITION XCB_GLX_FOUND
    EMIT_IF BOBUI_FEATURE_xcb AND BOBUI_FEATURE_xcb_glx_plugin
)
bobui_feature("xcb-egl-plugin" PRIVATE
    LABEL "EGL-X11 Plugin"
    CONDITION BOBUI_FEATURE_egl AND BOBUI_FEATURE_opengl
    EMIT_IF BOBUI_FEATURE_xcb
)
bobui_feature("xcb-xlib" PRIVATE
    LABEL "XCB Xlib"
    CONDITION BOBUI_FEATURE_xlib AND X11_XCB_FOUND
)
bobui_feature("xcb-sm" PRIVATE
    LABEL "xcb-sm"
    CONDITION BOBUI_FEATURE_sessionmanager AND X11_SM_FOUND
    EMIT_IF BOBUI_FEATURE_xcb
)
bobui_feature("system-xcb-xinput" PRIVATE SYSTEM_LIBRARY
    LABEL "Using system-provided xcb-xinput"
    AUTODETECT OFF
    CONDITION XCB_XINPUT_FOUND
    ENABLE INPUT_bundled_xcb_xinput STREQUAL 'no'
    DISABLE INPUT_bundled_xcb_xinput STREQUAL 'yes'
    EMIT_IF BOBUI_FEATURE_xcb
)
bobui_feature("xkbcommon" PUBLIC
    LABEL "xkbcommon"
    CONDITION XKB_FOUND
)
bobui_feature("xkbcommon-x11" PRIVATE
    LABEL "xkbcommon-x11"
    CONDITION BOBUI_FEATURE_xkbcommon AND XKB_COMMON_X11_FOUND
)
bobui_feature("xlib" PRIVATE
    LABEL "XLib"
    AUTODETECT NOT APPLE OR BOBUI_FEATURE_xcb
    CONDITION X11_FOUND
)
bobui_feature("texthtmlparser" PUBLIC
    SECTION "Kernel"
    LABEL "HtmlParser"
    PURPOSE "Provides a parser for HTML."
)
bobui_feature_definition("texthtmlparser" "BOBUI_NO_TEXTHTMLPARSER" NEGATE VALUE "1")
bobui_feature("textmarkdownreader" PUBLIC
    SECTION "Kernel"
    LABEL "MarkdownReader"
    PURPOSE "Provides a Markdown (CommonMark and GitHub) reader"
    ENABLE INPUT_libmd4c STREQUAL 'system' OR INPUT_libmd4c STREQUAL 'bobui' OR INPUT_libmd4c STREQUAL 'yes'
    DISABLE INPUT_libmd4c STREQUAL 'no'
)
bobui_feature("system-textmarkdownreader" PUBLIC SYSTEM_LIBRARY
    SECTION "Kernel"
    LABEL "  Using system libmd4c"
    CONDITION BOBUI_FEATURE_textmarkdownreader AND WrapSystemMd4c_FOUND
    ENABLE INPUT_libmd4c STREQUAL 'system'
    DISABLE INPUT_libmd4c STREQUAL 'bobui'
)
bobui_feature("textmarkdownwriter" PUBLIC
    SECTION "Kernel"
    LABEL "MarkdownWriter"
    CONDITION BOBUI_FEATURE_regularexpression
    PURPOSE "Provides a Markdown (CommonMark and GitHub) writer"
)
bobui_feature("textodfwriter" PUBLIC
    SECTION "Kernel"
    LABEL "OdfWriter"
    PURPOSE "Provides an ODF writer."
    CONDITION BOBUI_FEATURE_xmlstreamwriter
)
bobui_feature_definition("textodfwriter" "BOBUI_NO_TEXTODFWRITER" NEGATE VALUE "1")
bobui_feature("cssparser" PUBLIC
    SECTION "Kernel"
    LABEL "CssParser"
    PURPOSE "Provides a parser for Cascading Style Sheets."
)
bobui_feature_definition("cssparser" "BOBUI_NO_CSSPARSER" NEGATE VALUE "1")
bobui_feature("draganddrop" PUBLIC
    SECTION "Kernel"
    LABEL "Drag and Drop"
    PURPOSE "Supports the drag and drop mechanism."
    CONDITION BOBUI_FEATURE_imageformat_xpm
)
bobui_feature_definition("draganddrop" "BOBUI_NO_DRAGANDDROP" NEGATE VALUE "1")
bobui_feature("action" PUBLIC
    SECTION "Kernel"
    LABEL "Q(Gui)Action(Group)"
    PURPOSE "Provides abstract user interface actions."
)
bobui_feature_definition("action" "BOBUI_NO_ACTION" NEGATE VALUE "1")
bobui_feature("cursor" PUBLIC
    SECTION "Kernel"
    LABEL "QCursor"
    PURPOSE "Provides mouse cursors."
)
bobui_feature_definition("cursor" "BOBUI_NO_CURSOR" NEGATE VALUE "1")
bobui_feature("clipboard" PUBLIC
    SECTION "Kernel"
    LABEL "QClipboard"
    PURPOSE "Provides cut and paste operations."
    CONDITION NOT INTEGRITY AND NOT QNX AND NOT rtems
)
bobui_feature_definition("clipboard" "BOBUI_NO_CLIPBOARD" NEGATE VALUE "1")
bobui_feature("wheelevent" PUBLIC
    SECTION "Kernel"
    LABEL "QWheelEvent"
    PURPOSE "Supports wheel events."
)
bobui_feature_definition("wheelevent" "BOBUI_NO_WHEELEVENT" NEGATE VALUE "1")
bobui_feature("tabletevent" PUBLIC
    SECTION "Kernel"
    LABEL "BOBUIabletEvent"
    PURPOSE "Supports tablet events."
)
bobui_feature_definition("tabletevent" "BOBUI_NO_TABLETEVENT" NEGATE VALUE "1")
bobui_feature("im" PUBLIC
    SECTION "Kernel"
    LABEL "QInputContext"
    PURPOSE "Provides complex input methods."
)
bobui_feature_definition("im" "BOBUI_NO_IM" NEGATE VALUE "1")
bobui_feature("highdpiscaling" PUBLIC
    SECTION "Kernel"
    LABEL "High DPI Scaling"
    PURPOSE "Provides automatic scaling of DPI-unaware applications on high-DPI displays."
)
bobui_feature_definition("highdpiscaling" "BOBUI_NO_HIGHDPISCALING" NEGATE VALUE "1")
bobui_feature("validator" PUBLIC
    SECTION "Widgets"
    LABEL "QValidator"
    PURPOSE "Supports validation of input text."
)
bobui_feature_definition("validator" "BOBUI_NO_VALIDATOR" NEGATE VALUE "1")
bobui_feature("standarditemmodel" PUBLIC
    SECTION "ItemViews"
    LABEL "QStandardItemModel"
    PURPOSE "Provides a generic model for storing custom data."
    CONDITION BOBUI_FEATURE_itemmodel
)
bobui_feature_definition("standarditemmodel" "BOBUI_NO_STANDARDITEMMODEL" NEGATE VALUE "1")
bobui_feature("filesystemmodel" PUBLIC
    SECTION "File I/O"
    LABEL "QFileSystemModel"
    PURPOSE "Provides a data model for the local filesystem."
    CONDITION BOBUI_FEATURE_itemmodel
)
bobui_feature_definition("filesystemmodel" "BOBUI_NO_FILESYSTEMMODEL" NEGATE VALUE "1")
bobui_feature("imageformatplugin" PUBLIC
    SECTION "Images"
    LABEL "QImageIOPlugin"
    PURPOSE "Provides a base for writing a image format plugins."
)
bobui_feature_definition("imageformatplugin" "BOBUI_NO_IMAGEFORMATPLUGIN" NEGATE VALUE "1")
bobui_feature("movie" PUBLIC
    SECTION "Images"
    LABEL "QMovie"
    PURPOSE "Supports animated images."
)
bobui_feature_definition("movie" "BOBUI_NO_MOVIE" NEGATE VALUE "1")
bobui_feature("imageformat_bmp" PUBLIC
    SECTION "Images"
    LABEL "BMP Image Format"
    PURPOSE "Supports Microsoft's Bitmap image file format."
)
bobui_feature_definition("imageformat_bmp" "BOBUI_NO_IMAGEFORMAT_BMP" NEGATE VALUE "1")
bobui_feature("imageformat_ppm" PUBLIC
    SECTION "Images"
    LABEL "PPM Image Format"
    PURPOSE "Supports the Portable Pixmap image file format."
)
bobui_feature_definition("imageformat_ppm" "BOBUI_NO_IMAGEFORMAT_PPM" NEGATE VALUE "1")
bobui_feature("imageformat_xbm" PUBLIC
    SECTION "Images"
    LABEL "XBM Image Format"
    PURPOSE "Supports the X11 Bitmap image file format."
)
bobui_feature_definition("imageformat_xbm" "BOBUI_NO_IMAGEFORMAT_XBM" NEGATE VALUE "1")
bobui_feature("imageformat_xpm" PUBLIC
    SECTION "Images"
    LABEL "XPM Image Format"
    PURPOSE "Supports the X11 Pixmap image file format."
)
bobui_feature_definition("imageformat_xpm" "BOBUI_NO_IMAGEFORMAT_XPM" NEGATE VALUE "1")
bobui_feature("imageformat_png" PUBLIC
    SECTION "Images"
    LABEL "PNG Image Format"
    PURPOSE "Supports the Portable Network Graphics image file format."
)
bobui_feature_definition("imageformat_png" "BOBUI_NO_IMAGEFORMAT_PNG" NEGATE VALUE "1")
bobui_feature("imageformat_jpeg" PUBLIC
    SECTION "Images"
    LABEL "JPEG Image Format"
    PURPOSE "Supports the Joint Photographic Experts Group image file format."
)
bobui_feature_definition("imageformat_jpeg" "BOBUI_NO_IMAGEFORMAT_JPEG" NEGATE VALUE "1")
bobui_feature("image_heuristic_mask" PUBLIC
    SECTION "Images"
    LABEL "QImage::createHeuristicMask()"
    PURPOSE "Supports creating a 1-bpp heuristic mask for images."
)
bobui_feature_definition("image_heuristic_mask" "BOBUI_NO_IMAGE_HEURISTIC_MASK" NEGATE VALUE "1")
bobui_feature("image_text" PUBLIC
    SECTION "Images"
    LABEL "Image Text"
    PURPOSE "Supports image file text strings."
)
bobui_feature_definition("image_text" "BOBUI_NO_IMAGE_TEXT" NEGATE VALUE "1")
bobui_feature("picture" PUBLIC
    SECTION "Painting"
    LABEL "QPicture"
    PURPOSE "Supports recording and replaying QPainter commands."
)
bobui_feature_definition("picture" "BOBUI_NO_PICTURE" NEGATE VALUE "1")
bobui_feature("colornames" PUBLIC
    SECTION "Painting"
    LABEL "Color Names"
    PURPOSE "Supports color names such as \"red\", used by QColor and by some HTML documents."
)
bobui_feature_definition("colornames" "BOBUI_NO_COLORNAMES" NEGATE VALUE "1")
bobui_feature("pdf" PUBLIC
    SECTION "Painting"
    LABEL "QPdf"
    PURPOSE "Provides a PDF backend for QPainter."
    CONDITION BOBUI_FEATURE_temporaryfile
)
bobui_feature_definition("pdf" "BOBUI_NO_PDF" NEGATE VALUE "1")
bobui_feature("desktopservices" PUBLIC
    SECTION "Utilities"
    LABEL "QDesktopServices"
    PURPOSE "Provides methods for accessing common desktop services."
)
bobui_feature_definition("desktopservices" "BOBUI_NO_DESKTOPSERVICES" NEGATE VALUE "1")
bobui_feature("systemtrayicon" PUBLIC
    SECTION "Utilities"
    LABEL "QSystemTrayIcon"
    PURPOSE "Provides an icon for an application in the system tray."
    CONDITION BOBUI_FEATURE_temporaryfile
)
bobui_feature_definition("systemtrayicon" "BOBUI_NO_SYSTEMTRAYICON" NEGATE VALUE "1")
bobui_feature("accessibility" PUBLIC
    SECTION "Utilities"
    LABEL "Accessibility"
    PURPOSE "Provides accessibility support."
)
bobui_feature_definition("accessibility" "BOBUI_NO_ACCESSIBILITY" NEGATE VALUE "1")
bobui_feature("multiprocess" PRIVATE
    SECTION "Utilities"
    LABEL "Multi process"
    PURPOSE "Provides support for detecting the desktop environment, launching external processes and opening URLs."
    CONDITION NOT INTEGRITY AND NOT rtems
)
bobui_feature("whatsthis" PUBLIC
    SECTION "Widget Support"
    LABEL "QWhatsThis"
    PURPOSE "Supports displaying \"What's this\" help."
)
bobui_feature_definition("whatsthis" "BOBUI_NO_WHATSTHIS" NEGATE VALUE "1")
bobui_feature("raster-64bit" PRIVATE
    SECTION "Painting"
    LABEL "QPainter - 64 bit raster"
    PURPOSE "Internal painting support for 64 bit (16 bpc) rasterization."
)
bobui_feature("raster-fp" PRIVATE
    SECTION "Painting"
    LABEL "QPainter - floating point raster"
    PURPOSE "Internal painting support for floating point rasterization."
    CONDITION NOT VXWORKS # BOBUIBUG-115777
)

bobui_feature("bobuigui-threadpool" PRIVATE
    SECTION "Painting"
    LABEL "Multi-threaded image and painting helpers"
    PURPOSE "Multi-threaded image transforms and QPainter fills."
    CONDITION BOBUI_FEATURE_thread AND NOT WASM
)

bobui_feature("undocommand" PUBLIC
    SECTION "Utilities"
    LABEL "QUndoCommand"
    PURPOSE "Applies (redo or) undo of a single change in a document."
)
bobui_feature_definition("undocommand" "BOBUI_NO_UNDOCOMMAND" NEGATE VALUE "1")
bobui_feature("undostack" PUBLIC
    SECTION "Utilities"
    LABEL "QUndoStack"
    PURPOSE "Provides the ability to (redo or) undo a list of changes in a document."
    CONDITION BOBUI_FEATURE_undocommand
)
bobui_feature_definition("undostack" "BOBUI_NO_UNDOSTACK" NEGATE VALUE "1")
bobui_feature("undogroup" PUBLIC
    SECTION "Utilities"
    LABEL "QUndoGroup"
    PURPOSE "Provides the ability to cluster QUndoCommands."
    CONDITION BOBUI_FEATURE_undostack
)
bobui_feature("graphicsframecapture" PRIVATE
    SECTION "Utilities"
    LABEL "QGraphicsFrameCapture"
    PURPOSE "Provides a way to capture 3D graphics API calls for a rendered frame."
    CONDITION TEST_renderdoc OR (MACOS OR IOS)
)
bobui_feature_definition("undogroup" "BOBUI_NO_UNDOGROUP" NEGATE VALUE "1")
bobui_feature("wayland" PUBLIC
    SECTION "Platform plugins"
    LABEL "Wayland"
    CONDITION TARGET Wayland::Client
)
bobui_feature("waylandscanner" PUBLIC
    SECTION "Wayland Scanner tool"
    LABEL "Wayland Scanner"
    CONDITION TARGET Wayland::Scanner
)

# bobui wayland client
bobui_feature("wayland-client" PRIVATE
    LABEL "Client"
    CONDITION NOT WIN32 AND BOBUI_FEATURE_wayland AND BOBUI_FEATURE_waylandscanner
)
bobui_feature("wayland-server" PRIVATE
    LABEL "BobUI Wayland Compositor"
    CONDITION NOT WIN32 AND BOBUI_FEATURE_wayland AND BOBUI_FEATURE_waylandscanner
)
bobui_feature("wayland-egl" PRIVATE
    LABEL "EGL"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server)
              AND BOBUI_FEATURE_opengl AND BOBUI_FEATURE_egl
              AND (NOT QNX OR BOBUI_FEATURE_egl_extension_platform_wayland)
)
bobui_feature("wayland-brcm" PRIVATE
    LABEL "Raspberry Pi"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_eglfs_brcm
)
bobui_feature("wayland-drm-egl-server-buffer" PRIVATE
    LABEL "DRM EGL"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_opengl
              AND BOBUI_FEATURE_egl AND TEST_drm_egl_server
              AND (NOT QNX OR BOBUI_FEATURE_egl_extension_platform_wayland)
)
bobui_feature("wayland-libhybris-egl-server-buffer" PRIVATE
    LABEL "libhybris EGL"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_opengl
              AND BOBUI_FEATURE_egl AND TEST_libhybris_egl_server
)
bobui_feature("wayland-dmabuf-server-buffer" PRIVATE
    LABEL "Linux dma-buf server buffer"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_opengl
              AND BOBUI_FEATURE_egl AND TEST_dmabuf_server_buffer
)
bobui_feature("wayland-shm-emulation-server-buffer" PRIVATE
    LABEL "Shm emulation server buffer"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_opengl AND BOBUI_FEATURE_sharedmemory
)
bobui_feature("wayland-vulkan-server-buffer" PRIVATE
    LABEL "Vulkan-based server buffer"
    CONDITION (BOBUI_FEATURE_wayland_client OR BOBUI_FEATURE_wayland_server) AND BOBUI_FEATURE_vulkan
              AND BOBUI_FEATURE_opengl AND BOBUI_FEATURE_egl AND TEST_vulkan_server_buffer
)
bobui_feature("wayland-datadevice" PRIVATE
    CONDITION BOBUI_FEATURE_draganddrop OR BOBUI_FEATURE_clipboard
)
bobui_feature("wayland-client-primary-selection" PRIVATE
    LABEL "primary-selection clipboard"
    CONDITION BOBUI_FEATURE_clipboard
)
bobui_feature("wayland-client-fullscreen-shell-v1" PRIVATE
    LABEL "fullscreen-shell-v1"
    CONDITION BOBUI_FEATURE_wayland_client
)
bobui_feature("wayland-client-wl-shell" PRIVATE
    LABEL "wl-shell (deprecated)"
    CONDITION BOBUI_FEATURE_wayland_client
)
bobui_feature("wayland-client-xdg-shell" PRIVATE
    LABEL "xdg-shell"
    CONDITION BOBUI_FEATURE_wayland_client
)
bobui_feature("egl-extension-platform-wayland" PRIVATE
    LABEL "EGL wayland platform extension"
    CONDITION BOBUI_FEATURE_wayland_client AND BOBUI_FEATURE_opengl AND BOBUI_FEATURE_egl
              AND TEST_egl_1_5_wayland
)
bobui_feature("run-opengl-tests" PRIVATE
    LABEL "Run opengl tests"
    PURPOSE "Provides the ability to skip tests which require opengl to run"
    CONDITION BOBUI_FEATURE_opengl
)


bobui_configure_add_summary_section(NAME "BobUI Gui")
bobui_configure_add_summary_entry(ARGS "accessibility")
bobui_configure_add_summary_entry(ARGS "emojisegmenter")
bobui_configure_add_summary_entry(ARGS "freetype")
bobui_configure_add_summary_entry(ARGS "system-freetype")
bobui_configure_add_summary_entry(ARGS "harfbuzz")
bobui_configure_add_summary_entry(ARGS "system-harfbuzz")
bobui_configure_add_summary_entry(ARGS "fontconfig")
bobui_configure_add_summary_section(NAME "Image formats")
bobui_configure_add_summary_entry(ARGS "gif")
bobui_configure_add_summary_entry(ARGS "ico")
bobui_configure_add_summary_entry(ARGS "jpeg")
bobui_configure_add_summary_entry(ARGS "system-jpeg")
bobui_configure_add_summary_entry(ARGS "png")
bobui_configure_add_summary_entry(ARGS "system-png")
bobui_configure_end_summary_section() # end of "Image formats" section
bobui_configure_add_summary_section(NAME "Text formats")
bobui_configure_add_summary_entry(ARGS "texthtmlparser")
bobui_configure_add_summary_entry(ARGS "cssparser")
bobui_configure_add_summary_entry(ARGS "textodfwriter")
bobui_configure_add_summary_entry(ARGS "textmarkdownreader")
bobui_configure_add_summary_entry(ARGS "system-textmarkdownreader")
bobui_configure_add_summary_entry(ARGS "textmarkdownwriter")
bobui_configure_end_summary_section() # end of "Text formats" section
bobui_configure_add_summary_entry(ARGS "egl")
bobui_configure_add_summary_entry(ARGS "openvg")
bobui_configure_add_summary_section(NAME "OpenGL")
bobui_configure_add_summary_entry(ARGS "opengl-desktop")
bobui_configure_add_summary_entry(
    ARGS "opengl-dynamic"
    CONDITION WIN32
)
bobui_configure_add_summary_entry(ARGS "opengles2")
bobui_configure_add_summary_entry(ARGS "opengles3")
bobui_configure_add_summary_entry(ARGS "opengles31")
bobui_configure_add_summary_entry(ARGS "opengles32")
bobui_configure_end_summary_section() # end of "OpenGL" section
bobui_configure_add_summary_entry(ARGS "vulkan")
bobui_configure_add_summary_entry(ARGS "metal")
bobui_configure_add_summary_entry(ARGS "graphicsframecapture")
bobui_configure_add_summary_entry(ARGS "sessionmanager")
bobui_configure_add_summary_entry(
    ARGS "bobuigui-threadpool"
    CONDITION BOBUI_FEATURE_thread
)
bobui_configure_end_summary_section() # end of "BobUI Gui" section
bobui_configure_add_summary_section(NAME "Features used by QPA backends")
bobui_configure_add_summary_entry(ARGS "evdev")
bobui_configure_add_summary_entry(ARGS "libinput")
bobui_configure_add_summary_entry(ARGS "libinput_hires_wheel_support")
bobui_configure_add_summary_entry(ARGS "integrityhid")
bobui_configure_add_summary_entry(ARGS "mtdev")
bobui_configure_add_summary_entry(ARGS "tslib")
bobui_configure_add_summary_entry(ARGS "xkbcommon")
bobui_configure_add_summary_entry(ARGS "vxworksevdev")
bobui_configure_add_summary_section(NAME "X11 specific")
bobui_configure_add_summary_entry(ARGS "xlib")
bobui_configure_add_summary_entry(ARGS "xcb-xlib")
bobui_configure_add_summary_entry(ARGS "egl_x11")
bobui_configure_add_summary_entry(ARGS "xkbcommon-x11")
bobui_configure_add_summary_entry(ARGS "xcb-sm")
bobui_configure_end_summary_section() # end of "X11 specific" section
bobui_configure_end_summary_section() # end of "Features used by QPA backends" section
bobui_configure_add_summary_section(NAME "QPA backends")
bobui_configure_add_summary_entry(ARGS "directfb")
bobui_configure_add_summary_entry(ARGS "eglfs")
bobui_configure_add_summary_section(NAME "EGLFS details")
bobui_configure_add_summary_entry(ARGS "eglfs_openwfd")
bobui_configure_add_summary_entry(ARGS "eglfs_viv")
bobui_configure_add_summary_entry(ARGS "eglfs_viv_wl")
bobui_configure_add_summary_entry(ARGS "eglfs_rcar")
bobui_configure_add_summary_entry(ARGS "eglfs_egldevice")
bobui_configure_add_summary_entry(ARGS "eglfs_gbm")
bobui_configure_add_summary_entry(ARGS "eglfs_vsp2")
bobui_configure_add_summary_entry(ARGS "eglfs_mali")
bobui_configure_add_summary_entry(ARGS "eglfs_brcm")
bobui_configure_add_summary_entry(ARGS "eglfs_x11")
bobui_configure_end_summary_section() # end of "EGLFS details" section
bobui_configure_add_summary_entry(ARGS "linuxfb")
bobui_configure_add_summary_entry(ARGS "vnc")
bobui_configure_add_summary_entry(ARGS "vkkhrdisplay")
bobui_configure_add_summary_entry(
    ARGS "integrityfb"
    CONDITION INTEGRITY
)
bobui_configure_add_summary_section(NAME "QNX")
bobui_configure_add_summary_entry(ARGS "lgmon")
bobui_configure_add_summary_entry(ARGS "qqnx_imf")
bobui_configure_end_summary_section() # end of "QNX" section
bobui_configure_add_summary_section(NAME "XCB")
bobui_configure_add_summary_entry(ARGS "system-xcb-xinput")
bobui_configure_add_summary_section(NAME "GL integrations")
bobui_configure_add_summary_entry(ARGS "xcb-glx-plugin")
bobui_configure_add_summary_entry(ARGS "xcb-glx")
bobui_configure_add_summary_entry(ARGS "xcb-egl-plugin")
bobui_configure_end_summary_section() # end of "GL integrations" section
bobui_configure_end_summary_section() # end of "XCB" section
bobui_configure_add_summary_section(NAME "Windows")
bobui_configure_add_summary_entry(ARGS "direct2d")
bobui_configure_add_summary_entry(ARGS "direct2d1_1")
bobui_configure_add_summary_entry(ARGS "directwrite")
bobui_configure_add_summary_entry(ARGS "directwrite3")
bobui_configure_add_summary_entry(ARGS "directwritecolrv1")
bobui_configure_end_summary_section() # end of "Windows" section
bobui_configure_add_summary_section(NAME "Wayland")
bobui_configure_add_summary_entry(ARGS "wayland-client")
bobui_configure_add_summary_section(NAME "Hardware Integrations")
bobui_configure_add_summary_entry(ARGS "wayland-egl")
bobui_configure_add_summary_entry(ARGS "wayland-brcm")
bobui_configure_add_summary_entry(ARGS "wayland-drm-egl-server-buffer")
bobui_configure_add_summary_entry(ARGS "wayland-libhybris-egl-server-buffer")
bobui_configure_add_summary_entry(ARGS "wayland-dmabuf-server-buffer")
bobui_configure_add_summary_entry(ARGS "wayland-shm-emulation-server-buffer")
bobui_configure_add_summary_entry(ARGS "wayland-vulkan-server-buffer")
bobui_configure_end_summary_section() # end of "BobUI Wayland Drivers" section
bobui_configure_add_summary_section(NAME "Shell Integrations")
bobui_configure_add_summary_entry(ARGS "wayland-client-xdg-shell")
bobui_configure_add_summary_entry(ARGS "wayland-client-wl-shell")
bobui_configure_end_summary_section() # end of "Shell Integrations" section
bobui_configure_end_summary_section() # end of "Wayland" section
bobui_configure_end_summary_section() # end of "QPA backends" section
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "XCB support on macOS is minimal and untested. Some features will not work properly or at all (e.g. OpenGL, desktop services or accessibility), or may depend on your system and XQuartz setup."
    CONDITION BOBUI_FEATURE_xcb AND APPLE
)
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "Disabling X11 Accessibility Bridge: D-Bus or AT-SPI is missing."
    CONDITION BOBUI_FEATURE_accessibility AND BOBUI_FEATURE_xcb AND NOT BOBUI_FEATURE_accessibility_atspi_bridge
)
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "No QPA platform plugin enabled! This will produce a BobUI that cannot run GUI applications.  See \"Platform backends\" in the output of --help."
    CONDITION BOBUI_FEATURE_gui AND LINUX AND NOT ANDROID AND NOT BOBUI_FEATURE_xcb AND NOT BOBUI_FEATURE_eglfs AND NOT BOBUI_FEATURE_directfb AND NOT BOBUI_FEATURE_linuxfb
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "The OpenGL functionality tests failed! You might need to modify the OpenGL package search path by setting the OpenGL_DIR CMake variable to the OpenGL library's installation directory."
    CONDITION BOBUI_FEATURE_gui AND NOT WATCHOS AND NOT VISIONOS AND ( NOT INPUT_opengl STREQUAL 'no' ) AND NOT BOBUI_FEATURE_opengl_desktop AND NOT BOBUI_FEATURE_opengles2 AND NOT BOBUI_FEATURE_opengl_dynamic
)
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Accessibility disabled. This configuration of BobUI is unsupported."
    CONDITION NOT BOBUI_FEATURE_accessibility
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "XCB plugin requires xkbcommon and xkbcommon-x11, but -no-xkbcommon was provided."
    CONDITION ( NOT INPUT_xcb STREQUAL '' ) AND ( NOT INPUT_xcb STREQUAL 'no' ) AND INPUT_xkbcommon STREQUAL 'no'
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "The desktopservices feature is required on macOS and iOS, and cannot be disabled."
    CONDITION APPLE AND NOT BOBUI_FEATURE_desktopservices
)
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "BobUI Gui has been built without 'bobuiwaylandscanner' feature. This feature is required for building BobUI Wayland Client."
    CONDITION NOT BOBUI_FEATURE_waylandscanner AND BOBUI_FEATURE_wayland_client
)
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "BobUI Gui has been built without 'wayland' feature. This feature is required for building BobUI Wayland Client."
    CONDITION NOT BOBUI_FEATURE_wayland AND BOBUI_FEATURE_wayland_client
)

#### Inputs



#### Libraries


#### Tests


#### Features

