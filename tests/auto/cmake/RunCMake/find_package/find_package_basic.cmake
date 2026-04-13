find_package(BobUI6 REQUIRED COMPONENTS Core)

bobui_add_library(foo)
target_sources(foo PRIVATE dummy.cpp)
target_link_libraries(foo PRIVATE BobUI6::Core)
