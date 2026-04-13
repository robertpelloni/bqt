# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Defers the connection 'dependent' -> 'dependency'
#
# The actual connection can be made by calling bobui_internal_add_deferred_dependencies.
#
function(bobui_internal_defer_dependency dependent dependency)
    set_property(GLOBAL APPEND PROPERTY BOBUI_DEFERRED_DEPENDENCIES ${doc_target} ${tool_target})
endfunction()

# Adds dependencies between targets that have been deferred by calling bobui_internal_defer_dependency.
#
# This function checks whether the connection can be made (the dependency target exists).
# If the connection cannot be made, the deferred connection is left in the global property.
# Potentially, some later call to bobui_internal_add_deferred_dependencies will add it.
#
function(bobui_internal_add_deferred_dependencies)
    unset(unknown_deps)
    get_property(deferred_deps GLOBAL PROPERTY BOBUI_DEFERRED_DEPENDENCIES)
    while(deferred_deps)
        list(POP_FRONT deferred_deps dependent)
        list(POP_FRONT deferred_deps dependency)
        if (TARGET ${dependency})
            add_dependencies(${dependent} ${dependency})
        else()
            list(APPEND unknown_deps ${dependent} ${dependency})
        endif()
    endwhile()
    set_property(GLOBAL PROPERTY BOBUI_DEFERRED_DEPENDENCIES ${unknown_deps})
endfunction()
