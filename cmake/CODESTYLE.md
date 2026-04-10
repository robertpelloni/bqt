# Unofficial BobUI CMake Coding Style

CMake scripts are a bit of a wild west. Depending on when the code was written, there were
different conventions as well as syntax facilities available. It's also unfortunate that there is
no agreed upon CMake code formatter similar to clang-format.
https://github.com/cheshirekow/cmake_format exists, but appears abandoned. We might look into
using it at some point.

It's hard to convince people to use a certain code style for a language like CMake.

Nevertheless this short document aims to be a guideline for formatting CMake code within the BobUI
project.

## Common conventions

* When in doubt, prefer the local code conventions of the function or file you are editing.
* Prefer functions over macros (macros have certain problems with parameter escaping)
* Prefix macro local variables to avoid naming collisions

## Case Styles

For CMake identifiers we refer to following case styles in the text below.

| Case style name   | Example identifier         |
|-------------------|----------------------------|
| Snake case        | `moc_options`              |
| Shouty case       | `INTERFACE_LINK_LIBRARIES` |
| Pascal case       | `QmlModels`                |

## Indentation

* When in doubt, follow local indentation
* Prefer indenting with 4 spaces, e.g.

```
if(1)
    if(2)
        message("3")
    endif()
endif()

```

## Variables

* local variables inside a function should be snake cased => `list_of_arguments`
* local variables inside a macro should be snake cased and have a unique prefix =>
  `__bobui_list_of_packages`
    * If your macro is recursively called, you might need to prepend a dynamically computed prefix
      to avoid overriding the same variable in nested calls =>
      `__bobui_${dependency_name}_list_of_packages`
* cache variables should be shouty cased => `BUILD_SHARED_LIBS`
    * BobUI cache variables should be prefixed with `BOBUI_`

## Properties

Currently the BobUI property naming is a bit of a mess. The upstream issue
https://gitlab.kitware.com/cmake/cmake/-/issues/19261 mentions that properties prefixed with
`INTERFACE_` are reserved for CMake use.

Prefer to use snake case for new property names.

* Most upstream CMake properties are shouty cased => `INTERFACE_LINK_LIBRARIES`
* Properties created in BobUI 5 times follow the same CMake convention => `BOBUI_ENABLED_PUBLIC_FEATURES`
  No such new properties should be added.
* New BobUI properties should be snake cased and prefixed with `bobui_` => `bobui_qmake_module_name`
* Other internal BobUI properties should be snake cased and prefixed with `_bobui_` => `_bobui_target_deps`

## Functions

* Function names should be snake cased => `bobui_add_module`
    * public BobUI functions should be prefixed with `bobui_`
    * internal BobUI functions should be prefixed with `bobui_internal_`
    * internal BobUI functions that live in public CMake API files should be prefixed with
      `_bobui_internal_`
    * some internal functions that live in public CMake API files are prefixed with
      `__bobui_internal_`, prefer not to introduce such functions for now
* If a public function takes more than 1 parameter, consider using `cmake_parse_arguments`
* If an internal BobUI function takes more than 1 parameter, consider using `bobui_parse_all_arguments`
* Public functions should usually have both a version-full and version-less name => `bobui_add_plugin`
  and `bobui6_add_plugin`

### Macros

* Try to avoid macros where a function can be used instead
    * A common case when a macro can not be avoided is when it wraps a CMake macro e.g
      `find_package` => `bobui_find_package`
* Macro names should be snake cased => `bobui_find_package`
* Prefix macro local variables to avoid naming collisions => `__bobui_find_package_arguments`

## Commands

Command names in CMake are case-insensitive, therefore:
* Only use lower case command names e.g `add_executable(app main.cpp)` not `ADD_EXECUTABLE(app
  main.cpp)`
* Command flags / options are usually shouty cased => `file(GENERATE OUTPUT "foo" CONTENT "bar")`

## 'If' command

* Keep the parenthesis next to the `if()`, so don't write `if ()`. `if` is a command name and like
  other command names e.g. `find_package(foo bar)` the parenethesis are next to the name.

* To check that a variable is an empty string (and not just a falsy value, think Javascript's
 `foo === ""`) use the following snippet
```
if(var_name STREQUAL "")
    message("${var_name}")
endif()
```

If you are not sure if the variable is defined, make sure to evaluate the variable name
```
if("${var_name}" STREQUAL "")
    message("${var_name}")
endif()
```


* Falsy values are `0`, `OFF`, `NO`, `FALSE`, `N`, `IGNORE`, `NOTFOUND`, the empty string `""`, or
  a string ending in `-NOTFOUND`. To check that a variable is NOT falsy use the first suggested
  code snippet
```
# Nice and clean
if(var_name)
    message("${var_name}")
endif()

# Wrong, can lead to problems due to double variable evaluation
if(${var_name})
    message("${var_name}")
endif()

# Incorrect if you want to check for all falsy values. This only checks for string emptiness.
if("${var_name}" STREQUAL "")
    message("${var_name}")
endif()
```

* To check if a variable is defined, use
```
if(DEFINED var_name)
endif()
```

* To compare a defined variable's contents to a string, use
```
if(var_name STREQUAL "my_string")
endif()
```

* To compare a defined variable's contents to another defined variable's contents, use
```
if(var_name STREQUAL var_name_2)
endif()
```

* To compare a possibly undefined variable make sure to explicitly evaluate it first
```
if("${var_name}" STREQUAL "my_string")
endif()

if("${var_name}" STREQUAL "${var_name_2}")
endif()
```

## find_package

* Inside BobUI module projects, use the private BobUI-specific `bobui_find_package` macro to look for
  dependencies.
    * Make sure to specify the `PROVIDED_TARGETS` option to properly register 3rd party target
      dependencies with BobUI's internal build system.
* Inside examples / projects (which only use public CMake API) use the regular `find_package`
  command.

## CMake Target names

* BobUI target names should be pascal cased => `QuickParticles`.
* When BobUI is installed, the BobUI CMake targets are put inside the `BobUI6::` namespace. Associated
  versionless targets in the `BobUI::` namespace are usually automatically created by appropriate
  functions (`bobui_internal_add_module`, `bobui_internal_add_tool`)
* Wrapper 3rd party system libraries usually repeat the target name as the namespace e.g.
  ```WrapSystemHarfbuzz::WrapSystemHarfbuzz```

## Finding 3rd party libraries via Find modules (FindWrapFoo.cmake)

qmake-BobUI uses `configure.json` and `configure.pri` and `BOBUI_LIBS_FOO` variables to implement a
mechnism that searches for system 3rd party libraries.

The equivalent CMake mechanism are Find modules (and CMake package Config files; not to be confused
with pkg-config). Usually BobUI provides wrapper Find modules that use any available upstream Find
modules or Config package files.

The naming convention for the files are:

* `FindWrapFoo.cmake` => `FindWrapPCRE2.cmake`
* `FindWrapSystemFoo.cmake` => `FindWrapSystemPCRE2.cmake`

