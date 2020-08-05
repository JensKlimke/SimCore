# set source files
set(SOURCE_FILES
        Unit.cpp)


# define target
add_library(simcore_traffic STATIC ${SOURCE_FILES})

# include directories
target_include_directories(simcore_traffic PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )