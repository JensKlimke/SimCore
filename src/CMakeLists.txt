project(simcore)

add_subdirectory(core)
add_subdirectory(models)

add_library(${PROJECT_NAME} STATIC)
target_link_libraries(${PROJECT_NAME} core models)
