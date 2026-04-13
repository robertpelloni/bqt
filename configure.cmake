# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#### Inputs



#### Libraries

bobui_find_package(WrapSystemZLIB 1.0.8 MODULE
    PROVIDED_TARGETS WrapSystemZLIB::WrapSystemZLIB MODULE_NAME global QMAKE_LIB zlib
    VCPKG_PORT zlib
)
# Work around global target promotion failure when WrapZLIB is used on APPLE platforms.
# What ends up happening is that the ZLIB::ZLIB target is not promoted to global by bobui_find_package,
# then bobui_find_package(WrapSystemPNG) tries to find its dependency ZLIB::ZLIB, sees it's not global
# and tries to promote it to global, but fails because the directory scope of the PNG package is
# different (src/gui) from where ZLIB was originally found (bobuibase root).
# To avoid that, just manually promote the target to global here.
if(TARGET ZLIB::ZLIB)
    set_property(TARGET ZLIB::ZLIB PROPERTY IMPORTED_GLOBAL TRUE)
endif()

# Look for Threads in the same scope as OpenSSL package, because OpenSSL sometimes depends on
# Threads (for static OpenSSL builds) and we want to promote the target to global in the same
# directory scope.
bobui_find_package(Threads MODULE PROVIDED_TARGETS Threads::Threads)
bobui_find_package(WrapOpenSSLHeaders MODULE
    PROVIDED_TARGETS WrapOpenSSLHeaders::WrapOpenSSLHeaders MODULE_NAME core
    VCPKG_PORT openssl
    VCPKG_ADD_TO_FEATURE openssl
)
# openssl_headers
# OPENSSL_VERSION_MAJOR is not defined for OpenSSL 1.1.1
bobui_config_compile_test(opensslv11_headers
    LABEL "opensslv11_headers"
    LIBRARIES
        WrapOpenSSLHeaders::WrapOpenSSLHeaders
    CODE
"#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#if !defined(OPENSSL_VERSION_NUMBER) || defined(OPENSSL_VERSION_MAJOR) || OPENSSL_VERSION_NUMBER-0 < 0x10101000L
#  error OpenSSL >= 1.1.1 is required
#endif
#if !defined(OPENSSL_NO_EC) && !defined(SSL_CTRL_SET_CURVES)
#  error OpenSSL was reported as >= 1.1.1 but is missing required features, possibly it is libressl which is unsupported
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
")

bobui_find_package(WrapOpenSSL MODULE
    PROVIDED_TARGETS WrapOpenSSL::WrapOpenSSL MODULE_NAME core QMAKE_LIB openssl)
# openssl
# OPENSSL_VERSION_MAJOR is not defined for OpenSSL 1.1.1
bobui_config_compile_test(opensslv11
    LABEL "opensslv11"
    LIBRARIES
        WrapOpenSSL::WrapOpenSSL
    CODE
"#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#if !defined(OPENSSL_VERSION_NUMBER) || defined(OPENSSL_VERSION_MAJOR) || OPENSSL_VERSION_NUMBER-0 < 0x10101000L
#  error OpenSSL >= 1.1.1 is required
#endif
#if !defined(OPENSSL_NO_EC) && !defined(SSL_CTRL_SET_CURVES)
#  error OpenSSL was reported as >= 1.1.1 but is missing required features, possibly it is libressl which is unsupported
#endif

int main(void)
{
    /* BEGIN TEST: */
SSL_free(SSL_new(0));
    /* END TEST: */
    return 0;
}
")

# opensslv30
# openssl_headers
bobui_config_compile_test(opensslv30_headers
    LABEL "opensslv30_headers"
    LIBRARIES
        WrapOpenSSLHeaders::WrapOpenSSLHeaders
    CODE
"#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#if !OPENSSL_VERSION_PREREQ(3,0)
#  error OpenSSL >= 3.0 is required
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
")
bobui_config_compile_test(opensslv30
    LABEL "opensslv30"
    LIBRARIES
        WrapOpenSSL::WrapOpenSSL
    CODE
"#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#if !OPENSSL_VERSION_PREREQ(3,0)
#  error OpenSSL >= 3.0 is required
#endif

int main(void)
{
    /* BEGIN TEST: */
SSL_free(SSL_new(0));
    /* END TEST: */
    return 0;
}
")

bobui_find_package(WrapZSTD 1.3 MODULE
    PROVIDED_TARGETS
        WrapZSTD::WrapZSTD
        zstd::libzstd
        zstd::libzstd_static
        zstd::libzstd_shared
    MODULE_NAME global
    QMAKE_LIB zstd
)
bobui_find_package(WrapDBus1 1.2 MODULE PROVIDED_TARGETS dbus-1 MODULE_NAME global QMAKE_LIB dbus)
bobui_find_package(Libudev MODULE
    PROVIDED_TARGETS PkgConfig::Libudev MODULE_NAME global QMAKE_LIB libudev)
bobui_find_package(LTTngUST MODULE PROVIDED_TARGETS LTTng::UST MODULE_NAME core QMAKE_LIB lttng-ust)
bobui_add_qmake_lib_dependency(lttng-ust libdl)


#### Early-evaluated, Linker-related Tests and Features

bobui_internal_check_if_linker_is_available(use_bfd_linker
    LABEL "bfd linker"
    FLAG "-fuse-ld=bfd"
    )

bobui_internal_check_if_linker_is_available(use_gold_linker
    LABEL "gold linker"
    FLAG "-fuse-ld=gold"
    )

bobui_internal_check_if_linker_is_available(use_lld_linker
    LABEL "lld linker"
    FLAG "-fuse-ld=lld"
    )

# We set an invalid flag as a default flag so the compile test fails
# in case if no mold is found in PATH.
set(__bobui_internal_mold_linker_flags "-Wl,-invalid-flag")
if(NOT BOBUI_CONFIGURE_RUNNING)
    bobui_internal_get_mold_linker_flags(__bobui_internal_mold_linker_flags)
endif()
bobui_internal_check_if_linker_is_available(use_mold_linker
    LABEL "mold linker"
    FLAG "${__bobui_internal_mold_linker_flags}"
    )
unset(__bobui_internal_mold_linker_flags)

bobui_feature("use_bfd_linker"
    PRIVATE
    LABEL "bfd"
    AUTODETECT false
    CONDITION NOT MSVC AND NOT INTEGRITY AND NOT WASM AND TEST_use_bfd_linker
    ENABLE INPUT_linker STREQUAL 'bfd'
    DISABLE INPUT_linker STREQUAL 'gold' OR INPUT_linker STREQUAL 'lld'
            OR INPUT_linker STREQUAL 'mold'
)
bobui_feature_config("use_bfd_linker" QMAKE_PRIVATE_CONFIG)

bobui_feature("use_gold_linker_alias"
    AUTODETECT false
    CONDITION NOT WIN32 AND NOT INTEGRITY AND NOT WASM AND TEST_use_gold_linker
)
bobui_feature("use_gold_linker"
    PRIVATE
    LABEL "gold"
    AUTODETECT false
    CONDITION NOT WIN32 AND NOT INTEGRITY AND NOT WASM AND NOT rtems AND TEST_use_gold_linker
    ENABLE INPUT_linker STREQUAL 'gold' OR BOBUI_FEATURE_use_gold_linker_alias
    DISABLE INPUT_linker STREQUAL 'bfd' OR INPUT_linker STREQUAL 'lld'
            OR INPUT_linker STREQUAL 'mold'
)
bobui_feature_config("use_gold_linker" QMAKE_PRIVATE_CONFIG)

bobui_feature("use_lld_linker"
    PRIVATE
    LABEL "lld"
    AUTODETECT false
    CONDITION NOT MSVC AND NOT INTEGRITY AND NOT WASM AND TEST_use_lld_linker
    ENABLE INPUT_linker STREQUAL 'lld'
    DISABLE INPUT_linker STREQUAL 'bfd' OR INPUT_linker STREQUAL 'gold'
            OR INPUT_linker STREQUAL 'mold'
)
bobui_feature_config("use_lld_linker" QMAKE_PRIVATE_CONFIG)

bobui_feature("use_mold_linker"
    PRIVATE
    LABEL "mold"
    AUTODETECT FALSE
    CONDITION NOT WIN32 AND NOT INTEGRITY AND NOT WASM AND TEST_use_mold_linker
    ENABLE INPUT_linker STREQUAL 'mold'
    DISABLE INPUT_linker STREQUAL 'bfd' OR INPUT_linker STREQUAL 'gold'
            OR INPUT_linker STREQUAL 'lld'
)
bobui_feature_config("use_mold_linker" QMAKE_PRIVATE_CONFIG)

if(NOT BOBUI_CONFIGURE_RUNNING)
    bobui_evaluate_feature(use_bfd_linker)
    bobui_evaluate_feature(use_gold_linker_alias)
    bobui_evaluate_feature(use_gold_linker)
    bobui_evaluate_feature(use_lld_linker)
    bobui_evaluate_feature(use_mold_linker)

    bobui_run_linker_version_script_support()
endif()

bobui_feature("version_tagging"
    PUBLIC
    AUTODETECT TRUE
    CONDITION TEST_ld_version_script OR APPLE OR WIN32
)
bobui_feature_definition("version_tagging" "BOBUI_NO_VERSION_TAGGING" NEGATE)

#### Tests

# machineTuple
bobui_config_compile_test_machine_tuple("machine tuple")

# glibc
bobui_config_compile_test(glibc
    LABEL "Using Glibc"
    CODE
"#include <features.h>
#ifndef __GLIBC__
#error
#endif
int main() {}"
)

# glibc 2.34, for _FORTIFY_SOURCE == 3
bobui_config_compile_test(glibc_234
    LABEL "Using Glibc >= 2.34"
    CODE
"#include <features.h>
#if !defined(__GLIBC__) || !__GLIBC_PREREQ(2, 34)
#error
#endif
int main() {}"
)

# cxx20
bobui_config_compile_test(cxx20
    LABEL "C++20 support"
    CODE
"#if __cplusplus > 201703L
// Compiler claims to support C++20, trust it
#else
#  error __cplusplus must be > 201703L (the value for C++17)
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
"
    CXX_STANDARD 20
)

bobui_config_compile_test(cxx2b
    LABEL "C++23 support"
    CODE
"#if __cplusplus > 202002L
// Compiler claims to support C++23, trust it
#else
#  error __cplusplus must be > 202002L (the value for C++20)
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
"
    CXX_STANDARD 23
)

bobui_config_compile_test(cxx2c
    LABEL "C++2c support"
    CODE
"#if __cplusplus > 202302L
// Compiler claims to support C++2c, trust it
#else
#  error __cplusplus must be > 202302L (the value for C++23)
#endif

int main(void)
{
    /* BEGIN TEST: */
    /* END TEST: */
    return 0;
}
"
    CXX_STANDARD 26
)

bobui_config_compile_test(elf
    LABEL "ELF system"
    CODE
"#ifdef __ELF__
#include <elf.h>
#else
#error __ELF__ not defined
#endif
int main() { return 0; }
"
)

bobui_config_linker_supports_flag_test(linker_dynamic_list
    LABEL "--dynamic-list support"
    # doesn't matter that it's an .in, it just has to be the right syntax
    FLAG "--dynamic-list=${CMAKE_CURRENT_SOURCE_DIR}/src/corelib/BobUICore.dynlist.in"
)

bobui_config_linker_supports_flag_test(linker_symbolic_functions
    # Note: we infer the linker also supports -Bsymbolic if it supports this
    LABEL "-Bsymbolic-functions support"
    FLAG "-Bsymbolic-functions"
)

bobui_config_compiler_supports_flag_test(optimize_debug
    LABEL "-Og support"
    FLAG "-Og"
)

bobui_config_compile_test(no_direct_extern_access
    LABEL "-mno-direct-extern-access / -fno-direct-access-external-data support"
    PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config.tests/no_direct_extern_access"
)

bobui_config_linker_supports_flag_test(enable_new_dtags
    LABEL "new dtags support"
    FLAG "--enable-new-dtags"
)

bobui_config_linker_supports_flag_test(gdb_index
    LABEL "gdb index support"
    FLAG "--gdb-index"
)


if(MSVC OR APPLE)
    # These platforms / toolchains support separate debug information. Skip the compile test.
    set(TEST_separate_debug_info ON CACHE INTERNAL "separate debug information support")
else()
    bobui_config_compile_test("separate_debug_info"
                       LABEL "separate debug information support"
                       PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config.tests/separate_debug_info"
    )
endif()

# signaling_nan
bobui_config_compile_test(signaling_nan
    LABEL "Signaling NaN for doubles"
    CODE
"#if defined(__ghs)  && (__GHS_VERSION_NUMBER <= 202014)
#  error Signal NaNs are not supported by GHS compiler, but has_signaling_NaN returns TRUE. Will be fixed in future compiler releases.
#endif

#include <limits>

int main(void)
{
    /* BEGIN TEST: */
using B = std::numeric_limits<double>;
static_assert(B::has_signaling_NaN, \"System lacks signaling NaN\");
    /* END TEST: */
    return 0;
}
")

# basic x86 intrinsics support
bobui_config_compile_test(x86intrin
    LABEL "Basic x86 intrinsics"
    PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config.tests/x86intrin"
)

# x86: avx512vbmi2
bobui_config_compile_test_x86simd(avx512vbmi2 "AVX512VBMI2")

# x86: vaes
bobui_config_compile_test_x86simd(vaes "VAES")

# arm: crypto
bobui_config_compile_test_armintrin(crypto "CRYPTO")

# arm: sve
bobui_config_compile_test_armintrin(sve "SVE")

# loongarch: lsx
bobui_config_compile_test_loongarchsimd(lsx "LSX")

# loongarch: lasx
bobui_config_compile_test_loongarchsimd(lasx "LASX")

# localtime_r
bobui_config_compile_test(localtime_r
    LABEL "localtime_r()"
    CODE
"#include <time.h>

int main(void)
{
    /* BEGIN TEST: */
(void) localtime_r(nullptr, nullptr);
    /* END TEST: */
    return 0;
}
")

# localtime_s
bobui_config_compile_test(localtime_s
    LABEL "localtime_s()"
    CODE
"#include <time.h>

int main(void)
{
    /* BEGIN TEST: */
(void) localtime_s(nullptr, nullptr);
    /* END TEST: */
    return 0;
}
")

# posix_fallocate
bobui_config_compile_test(posix_fallocate
    LABEL "POSIX fallocate()"
    CODE
"#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    /* BEGIN TEST: */
(void) posix_fallocate(0, 0, 0);
    /* END TEST: */
    return 0;
}
")

# stack_protector
if(NOT WASM AND NOT VXWORKS)
    # emcc doesn't support this, but the detection accidentally succeeds
    # https://github.com/emscripten-core/emscripten/issues/17030

    # VXWORKS: We currently don't know the correct linker options. This is
    # tracked at BOBUIBUG-123715
    bobui_config_compiler_supports_flag_test(stack_protector
        LABEL "stack protection"
        FLAG "-fstack-protector-strong"
    )
endif()

# stack_clash_protection
if(NOT CLANG) # https://gitlab.kitware.com/cmake/cmake/-/issues/21998
    bobui_config_compiler_supports_flag_test(stack_clash_protection
        LABEL "-fstack-clash-protection support"
        FLAG "-fstack-clash-protection"
    )
endif()

# trivial_auto_var_init_pattern
if(NOT GCC OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "14.2.0")
    # Causes broken codegen on GCC https://gcc.gnu.org/bugzilla/show_bug.cgi?id=115527
    bobui_config_compiler_supports_flag_test(trivial_auto_var_init_pattern
        LABEL "-ftrivial-auto-var-init=pattern support"
        FLAG "-ftrivial-auto-var-init=pattern"
    )
endif()

# intelcet
if(MSVC)
    bobui_config_linker_supports_flag_test(intelcet
        LABEL "Support for Intel Control-flow Enforcement Technology (CET)"
        FLAG "-CETCOMPAT"
    )
else()
    bobui_config_compile_test(intelcet
        LABEL "Support for Intel Control-flow Enforcement Technology (CET)"
        COMPILE_OPTIONS -fcf-protection=full
        CODE
"int main(void)
{
    /* BEGIN TEST: */
#if !defined(__CET__)
#  error Intel CET not available
#endif
    /* END TEST: */
    return 0;
}
"
    )
endif()

# -z relro -z now
if(NOT WIN32)
    bobui_config_linker_supports_flag_test(relro_now_linker
        LABEL "Support for -z relro and -z now"
        FLAG "-z,relro,-z,now"
    )
endif()

# Is libc++ the default Standard Library?
bobui_config_compile_test(using_stdlib_libcpp
    LABEL "Compiler defaults to libc++"
    CODE
"
#include <ciso646>

int main(void)
{
/* BEGIN TEST: */
#ifndef _LIBCPP_VERSION
#   error
#endif
/* END TEST: */
}
"
)


#### Features

# This belongs into gui, but the license check needs it here already.
bobui_feature("android-style-assets" PRIVATE
    LABEL "Android Style Assets"
    CONDITION ANDROID
)
bobui_feature_alias("shared" PUBLIC
    LABEL "Building shared libraries"
    ALIAS_OF_CACHE BUILD_SHARED_LIBS
)
bobui_feature_definition("shared" "BOBUI_STATIC" NEGATE PREREQUISITE "!defined(BOBUI_SHARED) && !defined(BOBUI_STATIC)")
bobui_feature_config("shared" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature_config("shared" QMAKE_PUBLIC_CONFIG)
bobui_feature_alias("static"
    ALIAS_OF_FEATURE "shared"
    NEGATE
)
bobui_feature_config("static" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature_config("static" QMAKE_PUBLIC_CONFIG)
bobui_feature("cross_compile" PUBLIC
    LABEL "Cross compiling"
    CONDITION CMAKE_CROSSCOMPILING
)
bobui_feature_config("cross_compile" QMAKE_PUBLIC_CONFIG)
bobui_feature_config("cross_compile" QMAKE_PRIVATE_CONFIG)
bobui_feature("gc_binaries" PRIVATE
    CONDITION NOT BOBUI_FEATURE_shared
)
bobui_feature("optimize_debug"
    LABEL "Optimize debug build"
    AUTODETECT NOT BOBUI_FEATURE_developer_build
    CONDITION NOT MSVC AND NOT CLANG AND ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release ) AND TEST_optimize_debug
)
bobui_feature_config("optimize_debug" QMAKE_PRIVATE_CONFIG)
bobui_feature("optimize_size"
    LABEL "Optimize release build for size"
    AUTODETECT OFF
    CONDITION NOT BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release
)
bobui_feature_config("optimize_size" QMAKE_PRIVATE_CONFIG)
bobui_feature("optimize_full"
    LABEL "Fully optimize release builds (-O3)"
    AUTODETECT OFF
)
bobui_feature_config("optimize_full" QMAKE_PRIVATE_CONFIG)
bobui_feature("optimized_tools" PRIVATE
    LABEL "Build optimized tools for the Debug configuration"
    AUTODETECT OFF
)
bobui_feature("msvc_obj_debug_info"
    LABEL "Embed debug info in object files (MSVC)"
    ENABLE BOBUI_USE_CCACHE OR CMAKE_CXX_COMPILER_LAUNCHER
    AUTODETECT OFF
    EMIT_IF MSVC
)
bobui_feature_config("msvc_obj_debug_info" QMAKE_PRIVATE_CONFIG)
bobui_feature("pkg-config" PUBLIC
    LABEL "Using pkg-config"
    AUTODETECT NOT APPLE AND NOT WIN32 AND NOT ANDROID
    CONDITION PKG_CONFIG_FOUND
)
bobui_feature_config("pkg-config" QMAKE_PUBLIC_BOBUI_CONFIG
    NEGATE)
bobui_feature("developer-build" PRIVATE
    LABEL "Developer build"
    AUTODETECT OFF
)
bobui_feature("no-prefix"
    LABEL "No prefix build"
    AUTODETECT NOT BOBUI_WILL_INSTALL
    CONDITION NOT BOBUI_WILL_INSTALL
)
bobui_feature("lint_generated_code"
    LABEL "Lint bobui-generated code"
    AUTODETECT BOBUI_FEATURE_developer_build
)
bobui_feature("private_tests" PRIVATE
    LABEL "Developer build: private_tests"
    CONDITION BOBUI_FEATURE_developer_build
)
bobui_feature("doc_snippets" PRIVATE
    LABEL "Developer build: doc_snippets"
    AUTODETECT BOBUI_BUILD_DOC_SNIPPETS
    CONDITION BOBUI_FEATURE_shared
)
bobui_feature_definition("developer-build" "BOBUI_BUILD_INTERNAL")
bobui_feature_config("developer-build" QMAKE_PUBLIC_BOBUI_CONFIG
    NAME "private_tests"
)
bobui_feature("debug" PRIVATE
    LABEL "Build for debugging"
    AUTODETECT ON
    CONDITION CMAKE_BUILD_TYPE STREQUAL Debug OR Debug IN_LIST CMAKE_CONFIGURATION_TYPES
)
bobui_feature("debug_and_release" PUBLIC
    LABEL "Compile libs in debug and release mode"
    AUTODETECT 1
    CONDITION BOBUI_GENERATOR_IS_MULTI_CONFIG
)
bobui_feature_config("debug_and_release" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("force_debug_info"
    LABEL "Add debug info in release mode"
    AUTODETECT CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo OR RelWithDebInfo IN_LIST CMAKE_CONFIGURATION_TYPES
)
bobui_feature_config("force_debug_info" QMAKE_PRIVATE_CONFIG)
bobui_feature("separate_debug_info" PUBLIC
    LABEL "Split off debug information"
    AUTODETECT OFF
    CONDITION ( BOBUI_FEATURE_shared ) AND ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release OR BOBUI_FEATURE_force_debug_info ) AND TEST_separate_debug_info
)
bobui_feature_config("separate_debug_info" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("appstore-compliant" PUBLIC
    LABEL "App store compliance"
    PURPOSE "Disables code that is not allowed in platform app stores"
    AUTODETECT UIKIT OR ANDROID
)
if(APPLE)
    bobui_feature_definition("appstore-compliant" "BOBUI_APPLE_NO_PRIVATE_APIS")
endif()
bobui_feature("simulator_and_device" PUBLIC
    LABEL "Build for both simulator and device"
    CONDITION IOS AND NOT BOBUI_APPLE_SDK
)
bobui_feature_config("simulator_and_device" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("rpath" PUBLIC
    LABEL "Build with RPATH"
    AUTODETECT 1
    CONDITION BUILD_SHARED_LIBS AND UNIX AND NOT WIN32 AND NOT ANDROID
)
bobui_feature_config("rpath" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("elf"
    LABEL "ELF system"
    AUTODETECT UNIX AND NOT APPLE
    CONDITION TEST_elf
)
bobui_feature("elf_private_full_version" PRIVATE
    LABEL "Use BobUI's full version number in ELF version symbols"
    AUTODETECT OFF
    CONDITION BUILD_SHARED_LIBS AND BOBUI_FEATURE_elf
)
bobui_feature_config("elf_private_full_version" QMAKE_PRIVATE_BOBUI_CONFIG)
bobui_feature("force_asserts" PUBLIC
    LABEL "Force assertions"
    AUTODETECT OFF
)

bobui_feature("exceptions"
    LABEL "Enable exceptions"
    AUTODETECT OFF
)

bobui_feature("framework" PUBLIC
    LABEL "Build Apple Frameworks"
    AUTODETECT ON
    # If changing this, please align with logic in
    # bobui_internal_setup_cmake_config_postfix.
    CONDITION APPLE
)
bobui_feature_definition("framework" "BOBUI_MAC_FRAMEWORK_BUILD")
bobui_feature_config("framework" QMAKE_PUBLIC_BOBUI_CONFIG
    NAME "bobui_framework"
)
bobui_feature_config("framework" QMAKE_PUBLIC_CONFIG
    NAME "bobui_framework"
)
bobui_feature("largefile"
    LABEL "Large file support"
    CONDITION NOT ANDROID AND NOT INTEGRITY AND NOT rtems
)
bobui_feature_definition("largefile" "BOBUI_LARGEFILE_SUPPORT" VALUE "64")
bobui_feature_config("largefile" QMAKE_PRIVATE_CONFIG)
bobui_feature("sanitize_address"
    LABEL "Addresses"
    AUTODETECT OFF
)
bobui_feature_config("sanitize_address" QMAKE_PUBLIC_CONFIG)
bobui_feature("sanitize_thread"
    LABEL "Threads"
    AUTODETECT OFF
)
bobui_feature_config("sanitize_thread" QMAKE_PUBLIC_CONFIG)
bobui_feature("sanitize_memory"
    LABEL "Memory"
    AUTODETECT OFF
)
bobui_feature_config("sanitize_memory" QMAKE_PUBLIC_CONFIG)
bobui_feature("sanitize_fuzzer_no_link"
    LABEL "Fuzzer (instrumentation only)"
    PURPOSE "Adds instrumentation for fuzzing to the binaries but links to the usual main function instead of a fuzzer's."
    AUTODETECT OFF
)
bobui_feature_config("sanitize_fuzzer_no_link" QMAKE_PUBLIC_CONFIG)
bobui_feature("sanitize_undefined"
    LABEL "Undefined"
    AUTODETECT OFF
)
bobui_feature_config("sanitize_undefined" QMAKE_PUBLIC_CONFIG)
bobui_feature("sanitizer"
    LABEL "Sanitizers"
    CONDITION BOBUI_FEATURE_sanitize_address OR BOBUI_FEATURE_sanitize_thread OR BOBUI_FEATURE_sanitize_memory OR BOBUI_FEATURE_sanitize_fuzzer_no_link OR BOBUI_FEATURE_sanitize_undefined
)
bobui_feature_config("sanitizer" QMAKE_PUBLIC_CONFIG)
bobui_feature("plugin-manifests"
    LABEL "Embed manifests in plugins"
    AUTODETECT OFF
    EMIT_IF WIN32
)
bobui_feature_config("plugin-manifests" QMAKE_PUBLIC_CONFIG
    NEGATE
    NAME "no_plugin_manifest"
)
bobui_feature("c++20" PUBLIC
    LABEL "C++20"
    AUTODETECT OFF
    CONDITION TEST_cxx20 AND NOT VXWORKS
)
bobui_feature_config("c++20" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("c++2a" PUBLIC
    LABEL "C++20"
    CONDITION BOBUI_FEATURE_cxx20
)
bobui_feature_config("c++2a" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("c++2b" PUBLIC
    LABEL "C++23"
    AUTODETECT OFF
    CONDITION BOBUI_FEATURE_cxx20 AND (CMAKE_VERSION VERSION_GREATER_EQUAL "3.20") AND TEST_cxx2b
)
bobui_feature_config("c++2b" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("c++2c" PUBLIC
    LABEL "C++2c"
    AUTODETECT OFF
    CONDITION BOBUI_FEATURE_cxx2b AND (CMAKE_VERSION VERSION_GREATER_EQUAL "3.25") AND TEST_cxx2c
)
bobui_feature_config("c++2c" QMAKE_PUBLIC_BOBUI_CONFIG)
set(__bobui_ltcg_detected FALSE)
if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    set(__bobui_ltcg_detected TRUE)
else()
    foreach(config ${CMAKE_BUILD_TYPE} ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER "${config}" __bobui_uc_config)
        if(CMAKE_INTERPROCEDURAL_OPTIMIZATION_${__bobui_uc_config})
            set(__bobui_ltcg_detected TRUE)
            break()
        endif()
    endforeach()
    unset(__bobui_uc_config)
endif()
bobui_feature("ltcg"
    LABEL "Using Link Time Optimization (LTCG)"
    AUTODETECT ON
    CONDITION __bobui_ltcg_detected
)
bobui_feature_config("ltcg" QMAKE_PRIVATE_CONFIG)

if(NOT BOBUI_CONFIGURE_RUNNING)
    # This feature is used early in BobUICompilerOptimization.cmake.
    bobui_evaluate_feature(ltcg)
endif()

bobui_feature("enable_new_dtags"
    LABEL "Using new DTAGS"
    CONDITION BOBUI_FEATURE_elf AND TEST_enable_new_dtags
)
bobui_feature_config("enable_new_dtags" QMAKE_PRIVATE_CONFIG)
bobui_feature("enable_gdb_index"
    LABEL "Generating GDB index"
    AUTODETECT BOBUI_FEATURE_developer_build
    CONDITION GCC AND NOT CLANG AND ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_force_debug_info OR BOBUI_FEATURE_debug_and_release ) AND TEST_gdb_index
)
bobui_feature_config("enable_gdb_index" QMAKE_PRIVATE_CONFIG)
bobui_feature("reduce_exports" PRIVATE
    LABEL "Reduce amount of exported symbols"
    CONDITION NOT MSVC
)
bobui_feature_definition("reduce_exports" "BOBUI_VISIBILITY_AVAILABLE")
bobui_feature_config("reduce_exports" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("no_direct_extern_access" PRIVATE
    LABEL "Use protected visibility and -mno-direct-extern-access"
    CONDITION NOT WIN32 AND TEST_no_direct_extern_access
    AUTODETECT OFF
)
bobui_feature_definition("no_direct_extern_access" "BOBUI_USE_PROTECTED_VISIBILITY")
bobui_feature_config("no_direct_extern_access" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("reduce_relocations" PUBLIC
    LABEL "Reduce amount of relocations"
    AUTODETECT NOT WIN32 AND
        (TEST_architecture_arch STREQUAL i386) OR (TEST_architecture_arch STREQUAL x86_64) # See BOBUIBUG-36129
    CONDITION TEST_linker_symbolic_functions AND TEST_linker_dynamic_list
)
bobui_feature_definition("reduce_relocations" "BOBUI_REDUCE_RELOCATIONS")
bobui_feature_config("reduce_relocations" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("signaling_nan" PUBLIC
    LABEL "Signaling NaN"
    CONDITION TEST_signaling_nan
)
bobui_feature("x86intrin" PRIVATE
    LABEL "Basic"
    CONDITION
        (    (TEST_architecture_arch STREQUAL i386)
          OR (TEST_architecture_arch STREQUAL x86_64)
          OR (BOBUI_IS_MACOS_UNIVERSAL AND x86_64 IN_LIST CMAKE_OSX_ARCHITECTURES))
        AND (BOBUI_FORCE_FEATURE_x86intrin OR TEST_x86intrin)
    AUTODETECT NOT WASM
)
bobui_feature("sse2" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("sse2" "BOBUI_COMPILER_SUPPORTS_SSE2" VALUE "1")
bobui_feature_config("sse2" QMAKE_PRIVATE_CONFIG)
bobui_feature("sse3" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("sse3" "BOBUI_COMPILER_SUPPORTS_SSE3" VALUE "1")
bobui_feature_config("sse3" QMAKE_PRIVATE_CONFIG)
bobui_feature("ssse3" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("ssse3" "BOBUI_COMPILER_SUPPORTS_SSSE3" VALUE "1")
bobui_feature_config("ssse3" QMAKE_PRIVATE_CONFIG)
bobui_feature("sse4_1" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("sse4_1" "BOBUI_COMPILER_SUPPORTS_SSE4_1" VALUE "1")
bobui_feature_config("sse4_1" QMAKE_PRIVATE_CONFIG)
bobui_feature("sse4_2" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("sse4_2" "BOBUI_COMPILER_SUPPORTS_SSE4_2" VALUE "1")
bobui_feature_config("sse4_2" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx" "BOBUI_COMPILER_SUPPORTS_AVX" VALUE "1")
bobui_feature_config("avx" QMAKE_PRIVATE_CONFIG)
bobui_feature("f16c" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("f16c" "BOBUI_COMPILER_SUPPORTS_F16C" VALUE "1")
bobui_feature_config("f16c" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx2" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx2" "BOBUI_COMPILER_SUPPORTS_AVX2" VALUE "1")
bobui_feature_config("avx2" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512f" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512f" "BOBUI_COMPILER_SUPPORTS_AVX512F" VALUE "1")
bobui_feature_config("avx512f" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512er" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512er" "BOBUI_COMPILER_SUPPORTS_AVX512ER" VALUE "1")
bobui_feature_config("avx512er" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512cd" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512cd" "BOBUI_COMPILER_SUPPORTS_AVX512CD" VALUE "1")
bobui_feature_config("avx512cd" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512pf" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512pf" "BOBUI_COMPILER_SUPPORTS_AVX512PF" VALUE "1")
bobui_feature_config("avx512pf" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512dq" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512dq" "BOBUI_COMPILER_SUPPORTS_AVX512DQ" VALUE "1")
bobui_feature_config("avx512dq" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512bw" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512bw" "BOBUI_COMPILER_SUPPORTS_AVX512BW" VALUE "1")
bobui_feature_config("avx512bw" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512vl" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512vl" "BOBUI_COMPILER_SUPPORTS_AVX512VL" VALUE "1")
bobui_feature_config("avx512vl" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512ifma" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512ifma" "BOBUI_COMPILER_SUPPORTS_AVX512IFMA" VALUE "1")
bobui_feature_config("avx512ifma" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512vbmi" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("avx512vbmi" "BOBUI_COMPILER_SUPPORTS_AVX512VBMI" VALUE "1")
bobui_feature_config("avx512vbmi" QMAKE_PRIVATE_CONFIG)
bobui_feature("avx512vbmi2" PRIVATE
    LABEL "AVX512VBMI2"
    CONDITION BOBUI_FEATURE_x86intrin AND TEST_subarch_avx512vbmi2
)
bobui_feature_definition("avx512vbmi2" "BOBUI_COMPILER_SUPPORTS_AVX512VBMI2" VALUE "1")
bobui_feature_config("avx512vbmi2" QMAKE_PRIVATE_CONFIG)
bobui_feature("aesni" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("aesni" "BOBUI_COMPILER_SUPPORTS_AES" VALUE "1")
bobui_feature_config("aesni" QMAKE_PRIVATE_CONFIG)
bobui_feature("vaes" PRIVATE
    LABEL "VAES"
    CONDITION BOBUI_FEATURE_x86intrin AND TEST_subarch_vaes
)
bobui_feature_definition("vaes" "BOBUI_COMPILER_SUPPORTS_VAES" VALUE "1")
bobui_feature_config("vaes" QMAKE_PRIVATE_CONFIG)
bobui_feature("rdrnd" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("rdrnd" "BOBUI_COMPILER_SUPPORTS_RDRND" VALUE "1")
bobui_feature_config("rdrnd" QMAKE_PRIVATE_CONFIG)
bobui_feature("rdseed" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("rdseed" "BOBUI_COMPILER_SUPPORTS_RDSEED" VALUE "1")
bobui_feature_config("rdseed" QMAKE_PRIVATE_CONFIG)
bobui_feature("shani" PRIVATE
    CONDITION BOBUI_FEATURE_x86intrin
)
bobui_feature_definition("shani" "BOBUI_COMPILER_SUPPORTS_SHA" VALUE "1")
bobui_feature_config("shani" QMAKE_PRIVATE_CONFIG)
bobui_feature("lsx" PRIVATE
    LABEL "LSX"
    CONDITION ( TEST_architecture_arch STREQUAL loongarch64 ) AND TEST_subarch_lsx
)
bobui_feature_definition("lsx" "BOBUI_COMPILER_SUPPORTS_LSX" VALUE "1")
bobui_feature_config("lsx" QMAKE_PRIVATE_CONFIG)
bobui_feature("lasx" PRIVATE
    LABEL "LASX"
    CONDITION ( TEST_architecture_arch STREQUAL loongarch64 ) AND TEST_subarch_lasx
)
bobui_feature_definition("lasx" "BOBUI_COMPILER_SUPPORTS_LASX" VALUE "1")
bobui_feature_config("lasx" QMAKE_PRIVATE_CONFIG)
bobui_feature("mips_dsp" PRIVATE
    LABEL "DSP"
    CONDITION ( TEST_architecture_arch STREQUAL mips ) AND TEST_arch_${TEST_architecture_arch}_subarch_dsp
)
bobui_feature_definition("mips_dsp" "BOBUI_COMPILER_SUPPORTS_MIPS_DSP" VALUE "1")
bobui_feature_config("mips_dsp" QMAKE_PRIVATE_CONFIG)
bobui_feature("mips_dspr2" PRIVATE
    LABEL "DSPr2"
    CONDITION ( TEST_architecture_arch STREQUAL mips ) AND TEST_arch_${TEST_architecture_arch}_subarch_dspr2
)
bobui_feature_definition("mips_dspr2" "BOBUI_COMPILER_SUPPORTS_MIPS_DSPR2" VALUE "1")
bobui_feature_config("mips_dspr2" QMAKE_PRIVATE_CONFIG)
bobui_feature("neon" PRIVATE
    LABEL "NEON"
    CONDITION ( ( ( TEST_architecture_arch STREQUAL arm ) OR
        ( TEST_architecture_arch STREQUAL arm64 ) ) AND
        TEST_arch_${TEST_architecture_arch}_subarch_neon ) OR BOBUI_FORCE_FEATURE_neon
)
bobui_feature_definition("neon" "BOBUI_COMPILER_SUPPORTS_NEON" VALUE "1")
bobui_feature_config("neon" QMAKE_PRIVATE_CONFIG)
bobui_feature("arm_crc32" PRIVATE
    LABEL "CRC32"
    CONDITION ( ( TEST_architecture_arch STREQUAL arm ) OR ( TEST_architecture_arch STREQUAL arm64 ) ) AND TEST_arch_${TEST_architecture_arch}_subarch_crc32
)
bobui_feature_definition("arm_crc32" "BOBUI_COMPILER_SUPPORTS_CRC32" VALUE "1")
bobui_feature_config("arm_crc32" QMAKE_PRIVATE_CONFIG)
bobui_feature("arm_crypto" PRIVATE
    LABEL "AES"
    CONDITION ( ( TEST_architecture_arch STREQUAL arm ) OR ( TEST_architecture_arch STREQUAL arm64 ) ) AND ( TEST_arch_${TEST_architecture_arch}_subarch_crypto OR TEST_subarch_crypto )
)
bobui_feature_definition("arm_crypto" "BOBUI_COMPILER_SUPPORTS_CRYPTO" VALUE "1")
bobui_feature_definition("arm_crypto" "BOBUI_COMPILER_SUPPORTS_AES" VALUE "1")
bobui_feature_config("arm_crypto" QMAKE_PRIVATE_CONFIG)

bobui_feature("arm_sve" PRIVATE
    LABEL "SVE"
    CONDITION ( TEST_architecture_arch STREQUAL arm64 ) AND ( TEST_arch_${TEST_architecture_arch}_subarch_sve OR TEST_subarch_sve )
)
bobui_feature_definition("arm_sve" "BOBUI_COMPILER_SUPPORTS_SVE" VALUE "1")
bobui_feature_config("arm_sve" QMAKE_PRIVATE_CONFIG)

bobui_feature("wasm-simd128" PUBLIC
    LABEL "WebAssembly SIMD128"
    PURPOSE "Enables WebAssembly SIMD"
    AUTODETECT OFF
)
bobui_feature_definition("wasm-simd128" "BOBUI_COMPILER_SUPPORTS_WASM_SIMD128" VALUE "1")
bobui_feature_config("wasm-simd128" QMAKE_PRIVATE_CONFIG)

bobui_feature("wasm-exceptions" PUBLIC
    LABEL "WebAssembly Exceptions"
    PURPOSE "Enables WebAssembly Exceptions"
    AUTODETECT OFF
)
bobui_feature_definition("wasm-exceptions" "BOBUI_WASM_EXCEPTIONS" VALUE "1")
bobui_feature_config("wasm-exceptions" QMAKE_PRIVATE_CONFIG)

bobui_feature("wasm-jspi" PUBLIC
    LABEL "WebAssembly JSPI"
    PURPOSE "Enables WebAssembly JavaScript Promise Integration (JSPI)"
    AUTODETECT OFF
    CONDITION BOBUI_FEATURE_wasm_exceptions # JSPI requires wasm-exceptions
)
bobui_feature_definition("wasm-jspi" "BOBUI_WASM_JSPI" VALUE "1")
bobui_feature_config("wasm-jspi" QMAKE_PRIVATE_CONFIG)

bobui_feature("localtime_r" PRIVATE
    LABEL "localtime_r()"
    CONDITION TEST_localtime_r
)
bobui_feature("localtime_s" PRIVATE
    LABEL "localtime_s()"
    CONDITION TEST_localtime_s
)
bobui_feature("posix_fallocate" PRIVATE
    LABEL "POSIX fallocate()"
    CONDITION TEST_posix_fallocate
)
bobui_feature("force-system-libs" PRIVATE
    LABEL "Force the usage of system libraries"
    AUTODETECT OFF
)
bobui_feature("force-bundled-libs" PRIVATE
    LABEL "Force the usage of bundled libraries"
    AUTODETECT OFF
)
bobui_feature("system-zlib" PRIVATE SYSTEM_LIBRARY
    LABEL "Using system zlib"
    CONDITION WrapSystemZLIB_FOUND
)
bobui_feature("zstd" PUBLIC
    LABEL "Zstandard support"
    CONDITION WrapZSTD_FOUND
)
bobui_feature("stdlib-libcpp" PRIVATE
    LABEL "Using stdlib=libc++"
    AUTODETECT OFF
    CONDITION MINGW OR (LINUX AND NOT ANDROID)
)
# Check whether CMake was built with zstd support.
# See https://gitlab.kitware.com/cmake/cmake/-/issues/21552
if(NOT DEFINED CACHE{BOBUI_CMAKE_ZSTD_SUPPORT})
    set(BOBUI_CMAKE_ZSTD_SUPPORT FALSE CACHE INTERNAL "")
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.18")
        execute_process(COMMAND "${CMAKE_COMMAND}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/config.tests/cmake_zstd/check_zstd.cmake"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/config.tests"
            OUTPUT_QUIET ERROR_QUIET
            RESULT_VARIABLE bobui_check_zstd_exit_code)
        if(bobui_check_zstd_exit_code EQUAL 0)
            set(BOBUI_CMAKE_ZSTD_SUPPORT TRUE CACHE INTERNAL "")
        endif()
        unset(bobui_check_zstd_exit_code)
    endif()
endif()
bobui_feature("thread" PUBLIC
    SECTION "Kernel"
    LABEL "Thread support"
    PURPOSE "Provides BOBUIhread and related classes."
    AUTODETECT NOT WASM
)
bobui_feature("future" PUBLIC
    SECTION "Kernel"
    LABEL "QFuture"
    PURPOSE "Provides QFuture and related classes."
)
bobui_feature("concurrent" PUBLIC
    SECTION "Kernel"
    LABEL "BobUI Concurrent"
    PURPOSE "Provides a high-level multi-threading API."
    CONDITION BOBUI_FEATURE_future
)
bobui_feature_definition("concurrent" "BOBUI_NO_CONCURRENT" NEGATE VALUE "1")
bobui_feature("dbus" PUBLIC PRIVATE
    LABEL "BobUI D-Bus"
    AUTODETECT NOT UIKIT AND NOT ANDROID AND NOT VXWORKS
    CONDITION BOBUI_FEATURE_thread AND NOT WASM
)
bobui_feature_definition("dbus" "BOBUI_NO_DBUS" NEGATE VALUE "1")
bobui_feature("dbus-linked" PRIVATE
    LABEL "BobUI D-Bus directly linked to libdbus"
    CONDITION BOBUI_FEATURE_dbus AND DBus1_FOUND
    ENABLE INPUT_dbus STREQUAL 'linked'
    DISABLE INPUT_dbus STREQUAL 'runtime'
)
bobui_feature("qreal"
    LABEL "Type for qreal"
    CONDITION DEFINED BOBUI_COORD_TYPE AND NOT BOBUI_COORD_TYPE STREQUAL "double"
)
bobui_feature_definition("qreal" "BOBUI_COORD_TYPE" VALUE "${BOBUI_COORD_TYPE}")
bobui_feature_definition("qreal" "BOBUI_COORD_TYPE_STRING" VALUE "\"${BOBUI_COORD_TYPE}\"")
if(BOBUI_COORD_TYPE STREQUAL "double")
    bobui_feature_definition("qreal" "BOBUI_COORD_TYPE_IS_DOUBLE" VALUE "1")
elseif(BOBUI_COORD_TYPE STREQUAL "float")
    bobui_feature_definition("qreal" "BOBUI_COORD_TYPE_IS_FLOAT" VALUE "1")
endif()
bobui_feature("gui" PRIVATE
    LABEL "BobUI Gui"
    VCPKG_DEFAULT
)
bobui_feature_config("gui" QMAKE_PUBLIC_BOBUI_CONFIG
    NEGATE)
bobui_feature("network" PRIVATE
    LABEL "BobUI Network"
    SECTION "Module"
    PURPOSE "Provides the BobUI Network module."
    VCPKG_DEFAULT
)
bobui_feature("printsupport" PRIVATE
    LABEL "BobUI PrintSupport"
    CONDITION BOBUI_FEATURE_widgets
    SECTION "Module"
    PURPOSE "Provides the BobUI PrintSupport module."
)
bobui_feature("sql" PRIVATE
    LABEL "BobUI Sql"
    SECTION "Module"
    PURPOSE "Provides the Sql module."
    VCPKG_OPTIONAL
)
bobui_feature("testlib" PRIVATE
    LABEL "BobUI Testlib"
    SECTION "Module"
    PURPOSE "Provides the BobUI Testlib module."
)
bobui_feature("widgets" PRIVATE
    LABEL "BobUI Widgets"
    AUTODETECT NOT TVOS AND NOT WATCHOS
    CONDITION BOBUI_FEATURE_gui
)
bobui_feature_definition("widgets" "BOBUI_NO_WIDGETS" NEGATE)
bobui_feature_config("widgets" QMAKE_PUBLIC_BOBUI_CONFIG
    NEGATE)
bobui_feature("xml" PRIVATE
    LABEL "BobUI Xml"
    SECTION "Module"
    PURPOSE "Provides the BobUI Xml module."
)
bobui_feature("libudev" PRIVATE
    LABEL "udev"
    CONDITION Libudev_FOUND AND NOT INTEGRITY
)
bobui_feature("openssl" PRIVATE
    LABEL "OpenSSL"
    CONDITION BOBUI_FEATURE_openssl_runtime OR BOBUI_FEATURE_openssl_linked
    ENABLE false
    VCPKG_DEFAULT
)
bobui_feature_definition("openssl" "BOBUI_NO_OPENSSL" NEGATE)
bobui_feature_config("openssl" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("openssl-runtime"
    AUTODETECT NOT WASM
    CONDITION TEST_opensslv11_headers OR TEST_opensslv30_headers
    ENABLE INPUT_openssl STREQUAL 'yes' OR INPUT_openssl STREQUAL 'runtime'
    DISABLE INPUT_openssl STREQUAL 'no' OR INPUT_openssl STREQUAL 'linked' OR INPUT_ssl STREQUAL 'no'
)
bobui_feature("openssl-linked" PUBLIC
    LABEL "  BobUI directly linked to OpenSSL"
    AUTODETECT OFF
    CONDITION TEST_opensslv11 OR TEST_opensslv30
    ENABLE INPUT_openssl STREQUAL 'linked'
)
bobui_feature_definition("openssl-linked" "BOBUI_LINKED_OPENSSL")
bobui_feature("opensslv11" PUBLIC
    LABEL "OpenSSL 1.1"
    CONDITION TEST_opensslv11 OR TEST_opensslv11_headers
    DISABLE INPUT_openssl STREQUAL 'no' OR INPUT_ssl STREQUAL 'no'
)
bobui_feature("opensslv30" PUBLIC
    LABEL "OpenSSL 3.0"
    CONDITION TEST_opensslv30 OR TEST_opensslv30_headers
    DISABLE INPUT_openssl STREQUAL 'no' OR INPUT_ssl STREQUAL 'no'
)
bobui_feature("ccache"
    LABEL "Using ccache"
    AUTODETECT 1
    CONDITION BOBUI_USE_CCACHE
)
bobui_feature_config("ccache" QMAKE_PRIVATE_CONFIG)
bobui_feature("static_runtime"
    LABEL "Statically link the C/C++ runtime library"
    AUTODETECT OFF
    CONDITION NOT BOBUI_FEATURE_shared
    EMIT_IF WIN32
)
bobui_feature_config("static_runtime" QMAKE_PUBLIC_CONFIG)
bobui_feature_config("static_runtime" QMAKE_PUBLIC_BOBUI_CONFIG)
bobui_feature("dlopen" PRIVATE
    LABEL "dlopen()"
    CONDITION UNIX AND NOT INTEGRITY
)
bobui_feature("relocatable" PRIVATE
    LABEL "Relocatable"
    PURPOSE "Enable the BobUI installation to be relocated."
    AUTODETECT BOBUI_FEATURE_shared
    CONDITION BOBUI_FEATURE_dlopen OR WIN32 OR NOT BOBUI_FEATURE_shared
)
# hardening features
bobui_feature("intelcet" PRIVATE
    LABEL "Using Intel Control-flow Enforcement Technology (CET)"
    AUTODETECT ON
    CONDITION TEST_intelcet
)
bobui_feature_config("intelcet" QMAKE_PUBLIC_CONFIG)
bobui_feature("glibc_fortify_source" PRIVATE
    LABEL "Using Glibc function fortification"
    AUTODETECT ON
    CONDITION TEST_glibc
)
bobui_feature_config("glibc_fortify_source" QMAKE_PUBLIC_CONFIG)
bobui_feature("trivial_auto_var_init_pattern" PRIVATE
    LABEL "Using -ftrivial-auto-var-init=pattern"
    AUTODETECT ON
    CONDITION TEST_trivial_auto_var_init_pattern
)
bobui_feature_config("trivial_auto_var_init_pattern" QMAKE_PUBLIC_CONFIG)
bobui_feature("stack_protector" PRIVATE
    LABEL "Using -fstack-protector-strong"
    AUTODETECT ON
    CONDITION TEST_stack_protector
)
bobui_feature_config("stack_protector" QMAKE_PUBLIC_CONFIG)
bobui_feature("stack_clash_protection" PRIVATE
    LABEL "Using -fstack-clash-protection"
    AUTODETECT ON
    CONDITION TEST_stack_clash_protection
)
bobui_feature_config("stack_clash_protection" QMAKE_PUBLIC_CONFIG)
bobui_feature("libstdcpp_assertions" PRIVATE
    LABEL "Using libstdc++ assertions"
    AUTODETECT ON
    CONDITION (GCC OR (CLANG AND NOT MSVC AND NOT BOBUI_FEATURE_stdlib_libcpp AND NOT TEST_using_stdlib_libcpp))
)
bobui_feature_config("libstdcpp_assertions" QMAKE_PUBLIC_CONFIG)
bobui_feature("libcpp_hardening" PRIVATE
    LABEL "Using libc++ hardening"
    AUTODETECT ON
    CONDITION (BOBUI_FEATURE_stdlib_libcpp OR TEST_using_stdlib_libcpp)
)
bobui_feature_config("libcpp_hardening" QMAKE_PUBLIC_CONFIG)
bobui_feature("relro_now_linker" PRIVATE
    LABEL "Using -z relro -z now when linking"
    AUTODETECT ON
    CONDITION TEST_relro_now_linker
)
bobui_feature_config("relro_now_linker" QMAKE_PUBLIC_CONFIG)


if("${INPUT_coverage}" STREQUAL "gcov")
    bobui_config_compile_test(gcov
        LABEL "gcov compiler flags"
        COMPILE_OPTIONS "--coverage"
        CODE
    "int main(void)
    {
        /* BEGIN TEST: */
        /* END TEST: */
        return 0;
    }
    ")
endif()

bobui_feature("coverage-gcov"
    LABEL "Gcov"
    ENABLE INPUT_coverage STREQUAL "gcov"
    CONDITION TEST_gcov AND
        ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release )
)

bobui_feature("coverage"
    LABEL "Coverage"
    CONDITION BOBUI_FEATURE_coverage_gcov
)

bobui_feature("android_16kb_pages"
    LABEL "Using 16KB page sizes in Android"
    CONDITION ANDROID AND (((CMAKE_ANDROID_NDK_VERSION VERSION_GREATER_EQUAL "25.0.0"))
                          AND ((CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a") OR
                              (CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")))
)

bobui_configure_add_summary_build_type_and_config()
bobui_configure_add_summary_section(NAME "Build options")
bobui_configure_add_summary_build_mode(Mode)
bobui_configure_add_summary_entry(
    ARGS "optimize_debug"
    CONDITION NOT MSVC AND NOT CLANG AND ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release )
)
bobui_configure_add_summary_entry(
    ARGS "optimize_size"
    CONDITION NOT BOBUI_FEATURE_debug OR BOBUI_FEATURE_debug_and_release
)
bobui_configure_add_summary_entry(
    ARGS "optimize_full"
)
bobui_configure_add_summary_entry(ARGS "force_asserts" CONDITION BOBUI_FEATURE_force_asserts)
bobui_configure_add_summary_entry(ARGS "shared")
bobui_configure_add_summary_entry(
    ARGS "ccache"
    CONDITION UNIX
)
bobui_configure_add_summary_entry(
    TYPE "message" ARGS "Unity Build" MESSAGE "yes" CONDITION BOBUI_UNITY_BUILD
)
bobui_configure_add_summary_entry(
    TYPE "message" ARGS "Unity Build" MESSAGE "no" CONDITION NOT BOBUI_UNITY_BUILD
)
bobui_configure_add_summary_entry(
    TYPE "message"
    ARGS "Unity Build Batch Size"
    MESSAGE "${BOBUI_UNITY_BUILD_BATCH_SIZE}"
    CONDITION BOBUI_UNITY_BUILD
)
bobui_configure_add_summary_entry(
    TYPE "firstAvailableFeature"
    ARGS "use_bfd_linker use_gold_linker use_lld_linker use_mold_linker"
    MESSAGE "Linker"
    CONDITION BOBUI_FEATURE_use_bfd_linker OR BOBUI_FEATURE_use_gold_linker OR BOBUI_FEATURE_use_lld_linker
              OR BOBUI_FEATURE_use_mold_linker
)
bobui_configure_add_summary_entry(
    ARGS "enable_new_dtags"
    CONDITION BOBUI_FEATURE_elf
)
bobui_configure_add_summary_entry(
    ARGS "enable_gdb_index"
    CONDITION GCC AND NOT CLANG AND ( BOBUI_FEATURE_debug OR BOBUI_FEATURE_force_debug_info OR BOBUI_FEATURE_debug_and_release )
)
bobui_configure_add_summary_entry(ARGS "relocatable")
bobui_configure_add_summary_entry(ARGS "ltcg")
bobui_configure_add_summary_entry(ARGS "intelcet")
bobui_configure_add_summary_entry(ARGS "glibc_fortify_source")
bobui_configure_add_summary_entry(ARGS "trivial_auto_var_init_pattern")
bobui_configure_add_summary_entry(ARGS "stack_protector")
bobui_configure_add_summary_entry(ARGS "stack_clash_protection")
bobui_configure_add_summary_entry(ARGS "libstdcpp_assertions")
bobui_configure_add_summary_entry(ARGS "libcpp_hardening")
bobui_configure_add_summary_entry(ARGS "relro_now_linker")
bobui_configure_add_summary_entry(
    ARGS "wasm-simd128"
    CONDITION ( TEST_architecture_arch STREQUAL wasm )
)
bobui_configure_add_summary_entry(
    ARGS "wasm-exceptions"
    CONDITION ( TEST_architecture_arch STREQUAL wasm )
)
bobui_configure_add_summary_entry(
    ARGS "wasm-jspi"
    CONDITION ( TEST_architecture_arch STREQUAL wasm )
)
bobui_configure_add_summary_section(NAME "Target compiler supports")
bobui_configure_add_summary_entry(
    TYPE "featureList"
    ARGS "x86intrin vaes avx512vbmi2"
    MESSAGE "x86 Intrinsics"
    CONDITION ( ( TEST_architecture_arch STREQUAL i386 ) OR ( TEST_architecture_arch STREQUAL x86_64 ) )
)
bobui_configure_add_summary_entry(
    TYPE "featureList"
    ARGS "neon arm_crc32 arm_crypto arm_sve"
    MESSAGE "ARM Extensions"
    CONDITION ( TEST_architecture_arch STREQUAL arm ) OR ( TEST_architecture_arch STREQUAL arm64 )
)
bobui_configure_add_summary_entry(
    TYPE "featureList"
    ARGS "lsx lasx"
    MESSAGE "LOONGARCH Extensions"
    CONDITION ( TEST_architecture_arch STREQUAL loongarch64 )
)
bobui_configure_add_summary_entry(
    ARGS "mips_dsp"
    CONDITION ( TEST_architecture_arch STREQUAL mips )
)
bobui_configure_add_summary_entry(
    ARGS "mips_dspr2"
    CONDITION ( TEST_architecture_arch STREQUAL mips )
)
bobui_configure_end_summary_section() # end of "Target compiler supports" section
bobui_configure_add_summary_section(NAME "Sanitizers")
bobui_configure_add_summary_entry(ARGS "sanitize_address")
bobui_configure_add_summary_entry(ARGS "sanitize_thread")
bobui_configure_add_summary_entry(ARGS "sanitize_memory")
bobui_configure_add_summary_entry(ARGS "sanitize_fuzzer_no_link")
bobui_configure_add_summary_entry(ARGS "sanitize_undefined")
bobui_configure_end_summary_section() # end of "Sanitizers" section
bobui_configure_add_summary_build_parts("Build parts")
if(BOBUI_INSTALL_EXAMPLES_SOURCES)
    set(_examples_sources_entry_message "yes")
else()
    set(_examples_sources_entry_message "no")
endif()
bobui_configure_add_summary_entry(ARGS "Install examples sources" TYPE "message"
    MESSAGE "${_examples_sources_entry_message}")
bobui_configure_add_summary_entry(
    ARGS "appstore-compliant"
    CONDITION APPLE OR ANDROID OR WIN32
)
bobui_configure_end_summary_section() # end of "Build options" section
bobui_configure_add_summary_section(NAME "BobUI modules and options")
bobui_configure_add_summary_entry(ARGS "concurrent")
bobui_configure_add_summary_entry(ARGS "dbus")
bobui_configure_add_summary_entry(ARGS "dbus-linked")
bobui_configure_add_summary_entry(ARGS "gui")
bobui_configure_add_summary_entry(ARGS "network")
bobui_configure_add_summary_entry(ARGS "printsupport")
bobui_configure_add_summary_entry(ARGS "sql")
bobui_configure_add_summary_entry(ARGS "testlib")
bobui_configure_add_summary_entry(ARGS "widgets")
bobui_configure_add_summary_entry(ARGS "xml")
bobui_configure_end_summary_section() # end of "BobUI modules and options" section
bobui_configure_add_summary_section(NAME "Support enabled for")
bobui_configure_add_summary_entry(ARGS "pkg-config")

if(BOBUI_USE_VCPKG AND (DEFINED ENV{VCPKG_ROOT} OR VCPKG_TARGET_TRIPLET))
    set(_vcpkg_entry_message "yes")
else()
    set(_vcpkg_entry_message "no")
endif()
bobui_configure_add_summary_entry(ARGS "Using vcpkg" TYPE "message" MESSAGE "${_vcpkg_entry_message}")

bobui_configure_add_summary_entry(ARGS "libudev")
bobui_configure_add_summary_entry(ARGS "liburing")
bobui_configure_add_summary_entry(ARGS "openssl")
bobui_configure_add_summary_entry(ARGS "openssl-linked")
bobui_configure_add_summary_entry(ARGS "opensslv11")
bobui_configure_add_summary_entry(ARGS "opensslv30")
bobui_configure_add_summary_entry(ARGS "system-zlib")
bobui_configure_add_summary_entry(ARGS "zstd")
bobui_configure_add_summary_entry(ARGS "thread")
bobui_configure_add_summary_entry(ARGS "android_16kb_pages")

bobui_configure_end_summary_section() # end of "Support enabled for" section
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "Using static linking will disable the use of dynamically loaded plugins. Make sure to import all needed static plugins, or compile needed modules into the library."
    CONDITION NOT BOBUI_FEATURE_shared
)
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "-debug-and-release is only supported on Darwin and Windows platforms.  BobUI can be built in release mode with separate debug information, so -debug-and-release is no longer necessary."
    CONDITION INPUT_debug_and_release STREQUAL 'yes' AND NOT APPLE AND NOT WIN32
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Static builds don't support RPATH"
    CONDITION ( BOBUI_FEATURE_rpath OR BOBUI_EXTRA_RPATHS ) AND NOT BOBUI_FEATURE_shared
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Command line option -sanitize fuzzer-no-link is only supported with clang compilers."
    CONDITION BOBUI_FEATURE_sanitize_fuzzer_no_link AND NOT CLANG
)
if (TEST_architecture_arch STREQUAL x86_64 OR TEST_architecture_arch STREQUAL i386)
    if ((TEST_architecture_arch STREQUAL i386) OR QNX OR WASM)
        # Warn only
        bobui_configure_add_report_entry(
            TYPE WARNING
            CONDITION (NOT BOBUI_FEATURE_x86intrin)
            MESSAGE [=[
All x86 intrinsics and SIMD support were disabled. If this was in error, check
the result of the build in config.tests/x86intrin and report at https://bugreports.bobui.io.
]=]
        )
    elseif (MSVC AND CLANG)
        # Warn only
        bobui_configure_add_report_entry(
            TYPE WARNING
            CONDITION (NOT BOBUI_FEATURE_x86intrin)
            MESSAGE [=[
x86 intrinsics support is disabled for clang-cl build. This might be necessary due to
https://github.com/llvm/llvm-project/issues/53520
]=]
        )
    else()
        string(CONCAT x86_intrin_error_message
            "x86 intrinsics support missing. Check your compiler settings.\n"
            "If this is a problem for you, report at https://bugreports.bobui.io with your compiler ID and "
            "version, and the TEST_x86intrin compile test output.\n"
        )
        bobui_configure_add_report_entry(
            TYPE ERROR
            CONDITION (NOT BOBUI_FEATURE_x86intrin)
            COMPILE_TESTS_TO_SHOW_ON_ERROR TEST_x86intrin
            MESSAGE "${x86_intrin_error_message}"
        )
    endif()
endif()
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Setting a library infix is not supported for framework builds."
    CONDITION BOBUI_FEATURE_framework AND DEFINED BOBUI_LIBINFIX
)
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "WASM Thread support enabled."
    CONDITION BOBUI_FEATURE_thread AND WASM
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "You should use the recommended Emscripten version ${BOBUI_EMCC_RECOMMENDED_VERSION} with this BobUI. You have ${EMCC_VERSION}."
    CONDITION WASM AND ${EMCC_VERSION} VERSION_LESS ${BOBUI_EMCC_RECOMMENDED_VERSION}
)
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Using Emscripten version ${EMCC_VERSION} with this BobUI
    may have issues. The recommended version is ${BOBUI_EMCC_RECOMMENDED_VERSION}."
    CONDITION WASM AND ${EMCC_VERSION} VERSION_GREATER ${BOBUI_EMCC_RECOMMENDED_VERSION}
)
bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Some tests might fail to build when targeting WASM without -feature-thread."
    CONDITION WASM AND BOBUI_BUILD_TESTS AND NOT BOBUI_FEATURE_thread
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "Building BobUI with C++20 is not supported with MSVC 2019."
    CONDITION BOBUI_FEATURE_cxx20 AND MSVC AND MSVC_VERSION LESS "1930"
)
bobui_configure_add_report_entry(
    TYPE ERROR
    MESSAGE "You cannot force both system and bundled libraries."
    CONDITION BOBUI_FEATURE_force_bundled_libs AND BOBUI_FEATURE_force_system_libs
)
bobui_configure_add_report_entry(
    TYPE NOTE
    MESSAGE "Building BobUI for Android and user projects with 16KB page sizes."
    CONDITION BOBUI_FEATURE_android_16kb_pages
)
if(WASM)
    bobui_extra_definition("BOBUI_EMCC_VERSION" "\"${EMCC_VERSION}\"" PUBLIC)
endif()
bobui_extra_definition("BOBUI_VERSION_STR" "\"${PROJECT_VERSION}\"" PUBLIC)
bobui_extra_definition("BOBUI_VERSION_MAJOR" ${PROJECT_VERSION_MAJOR} PUBLIC)
bobui_extra_definition("BOBUI_VERSION_MINOR" ${PROJECT_VERSION_MINOR} PUBLIC)
bobui_extra_definition("BOBUI_VERSION_PATCH" ${PROJECT_VERSION_PATCH} PUBLIC)

bobui_extra_definition("BOBUI_COPYRIGHT" \"${BOBUI_COPYRIGHT}\" PUBLIC)

bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "BOBUI_ALLOW_SYMLINK_IN_PATHS is enabled. This is not recommended, and it may lead to unexpected issues.
E.g., When building BobUIWebEngine, enabling this option may result in build issues in certain platforms.
See https://bugreports.bobui.io/browse/BOBUIBUG-59769."
    CONDITION BOBUI_ALLOW_SYMLINK_IN_PATHS
)

# BobUIGuiTest interface
bobui_feature_definition("test_gui" "BOBUI_GUI_TEST" VALUE "1")
bobui_feature("test_gui" PUBLIC
    LABEL "Build BobUIGuiTest namespace"
)

# Run squish based tests
bobui_feature_definition("test_squish" "BOBUI_SQUISH_TEST" VALUE "0")
bobui_feature("test_squish" PUBLIC
    LABEL "Run Squish based tests"
)

bobui_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Starting with BobUI 6.10, a -no-prefix or -developer-build
BobUI build might not find all necessary BobUI packages when using CMake < 3.26.
Please use a newer CMake version."
    CONDITION BOBUI_FEATURE_no_prefix AND CMAKE_VERSION VERSION_LESS "3.26"
)
