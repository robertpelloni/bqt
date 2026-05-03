# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


# WARNING must keep in sync with wasm-emscripten/qmake.conf!
function (bobui_internal_setup_wasm_target_properties wasmTarget)

    target_link_options("${wasmTarget}" INTERFACE
    "SHELL:-s MAX_WEBGL_VERSION=2"
    "SHELL:-s WASM_BIGINT=1"
    "SHELL:-s STACK_SIZE=5MB")

    set(executable_link_flags "-sFETCH")
    ## wasm64
    if (WASM64)
        target_compile_options("${wasmTarget}" INTERFACE "SHELL:-s MEMORY64=1" )
        target_link_options("${wasmTarget}" INTERFACE   "SHELL:-s MEMORY64=1" -mwasm64)
    endif()

    #simd
    if (BOBUI_FEATURE_wasm_simd128)
        target_compile_options("${wasmTarget}" INTERFACE -msimd128)
    endif()
    if (BOBUI_FEATURE_sse2)
        target_compile_options("${wasmTarget}" INTERFACE -O2 -msimd128 -msse -msse2)
    endif()

    # exceptions
    if (BOBUI_FEATURE_wasm_exceptions)
        target_compile_options("${wasmTarget}" INTERFACE -fwasm-exceptions)
        target_link_options("${wasmTarget}" INTERFACE -fwasm-exceptions)
    elseif(BOBUI_FEATURE_exceptions)
        # add link option only, compile option is added in cross-platform code
        target_link_options("${wasmTarget}" INTERFACE -fexceptions)
    endif()

    # setjmp/longjmp type. The type is "emscripten" by default, but must
    # be set to "wasm" when wasm-exceptions are used, for compatibility reasons.
    if (BOBUI_FEATURE_wasm_exceptions)
        target_compile_options("${wasmTarget}" INTERFACE -s SUPPORT_LONGJMP=wasm)
        target_link_options("${wasmTarget}" INTERFACE -s SUPPORT_LONGJMP=wasm)
    endif()

    if (BOBUI_FEATURE_thread)
        target_compile_options("${wasmTarget}" INTERFACE "SHELL:-pthread")
        target_link_options("${wasmTarget}" INTERFACE "SHELL:-pthread")
    endif()

    target_link_options("${wasmTarget}" INTERFACE "SHELL:-s ALLOW_MEMORY_GROWTH")

    # debug add_compile_options
    if ("BOBUI_WASM_SOURCE_MAP=1" IN_LIST BOBUI_QMAKE_DEVICE_OPTIONS)
        set(WASM_SOURCE_MAP_BASE "http://localhost:8000/")

        if(DEFINED BOBUI_WASM_SOURCE_MAP_BASE)
            set(WASM_SOURCE_MAP_BASE "${BOBUI_WASM_SOURCE_MAP_BASE}")
        endif()

        # Pass --source-map-base on the linker line. This informs the
        # browser where to find the source files when debugging.
        # -g4 to make source maps for debugging
        target_link_options("${wasmTarget}" INTERFACE  "-gsource-map" "--source-map-base" "${WASM_SOURCE_MAP_BASE}")

    endif()

    # a few good defaults to make console more verbose while debugging
    target_link_options("${wasmTarget}" INTERFACE $<$<CONFIG:Debug>:
        --profiling-funcs>)

    # target_link_options("${wasmTarget}" INTERFACE "SHELL:-s LIBRARY_DEBUG=1") # print out library calls, verbose
    # target_link_options("${wasmTarget}" INTERFACE "SHELL:-s SYSCALL_DEBUG=1") # print out sys calls, verbose
    # target_link_options("${wasmTarget}" INTERFACE "SHELL:-s FS_LOG=1") # print out filesystem ops, verbose
    # target_link_options("${wasmTarget}" INTERFACE "SHELL:-s SOCKET_DEBUG") # print out socket,network data transfer

    if ("BOBUI_EMSCRIPTEN_ASYNCIFY=1" IN_LIST BOBUI_QMAKE_DEVICE_OPTIONS)
        # Emscripten recommends building with optimizations when using asyncify
        # in order to reduce wasm file size, and may also generate broken wasm
        # (with "wasm validation error: too many locals" type errors) if optimizations
        # are omitted. Enable optimizations also for debug builds.
        set(BOBUI_CFLAGS_OPTIMIZE_DEBUG "-Os" CACHE STRING INTERNAL FORCE)
        set(BOBUI_FEATURE_optimize_debug ON CACHE BOOL INTERNAL FORCE)

        target_link_options("${wasmTarget}" INTERFACE "SHELL:-s ASYNCIFY" "-Os")
        target_compile_definitions("${wasmTarget}" INTERFACE BOBUI_HAVE_EMSCRIPTEN_ASYNCIFY)
    elseif ("BOBUI_EMSCRIPTEN_ASYNCIFY=2" IN_LIST BOBUI_QMAKE_DEVICE_OPTIONS OR BOBUI_FEATURE_wasm_jspi)
        # Enable JSPI (also known as asyncify 2). Unlike asyncify 1 this
        # is supported natively by the browsers, and does not require
        # enabling optimizations.
        target_link_options("${wasmTarget}" INTERFACE "SHELL:-s JSPI")
        target_compile_definitions("${wasmTarget}" INTERFACE BOBUI_HAVE_EMSCRIPTEN_ASYNCIFY)
    endif()

    _bobui_internal_handle_target_supports_shared_libs()

    if(BOBUI_FEATURE_shared)
        set(side_modules
            MODULE_LIBRARY SHARED_LIBRARY)
        set(enable_side_module_if_needed
            "$<$<IN_LIST:$<TARGET_PROPERTY:TYPE>,${side_modules}>:SHELL:-s SIDE_MODULE=1>")
        set(enable_main_module_if_needed
            "$<$<IN_LIST:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:SHELL:-sMAIN_MODULE=1 ${executable_link_flags}>")
        set(set_shared_module_type_if_needed
            "${enable_side_module_if_needed}"
            "${enable_main_module_if_needed}"
        )

        # Add BobUI libdir to linker library paths
        set(bobui_lib_location
            "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}/${INSTALL_LIBDIR}")
        target_link_options("${wasmTarget}" INTERFACE
            "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:SHELL:" -L${bobui_lib_location}/>)

        target_compile_options("${wasmTarget}" INTERFACE "${set_shared_module_type_if_needed}")
        target_link_options("${wasmTarget}" INTERFACE "${set_shared_module_type_if_needed}")

    else()
        target_link_options("${wasmTarget}" INTERFACE "SHELL:-sERROR_ON_UNDEFINED_SYMBOLS=1 ${executable_link_flags}")
    endif()

    # Suppress warnings for known issues for developer builds
    if(FEATURE_developer_build)
        target_link_options("${wasmTarget}" INTERFACE "SHELL:-Wno-pthreads-mem-growth")
    endif()

endfunction()

function(bobui_internal_wasm_add_finalizers target)
    bobui_add_list_file_finalizer(_bobui_internal_finalize_wasm_app ${target})
endfunction()
