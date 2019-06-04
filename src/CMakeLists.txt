add_subdirectory(core)
add_subdirectory(components)

add_library(simcore STATIC)
target_link_libraries(simcore PRIVATE core models)
