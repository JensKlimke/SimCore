# define and create log dir
set(PROJECT_LOG_DIR ${PROJECT_SOURCE_DIR}/log)
file(MAKE_DIRECTORY ${PROJECT_LOG_DIR})

# add definition
add_definitions(-DLOG_DIR=\"${PROJECT_LOG_DIR}\")

# add sub directories
add_subdirectory(simcore)
add_subdirectory(testing)
add_subdirectory(traffic)
add_subdirectory(blocks)