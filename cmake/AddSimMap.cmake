# 
#
# Downloads operndrive-parser and provides a helper macro to add tests. Add make check, as well, which
# gives output on failed tests without having to set an environment variable.
#
#

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(SIMMAP_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
message("Downloading SimMap ...")

if (CMAKE_VERSION VERSION_LESS 3.11)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

    include(DownloadProject)
    download_project(PROJ simmap
            GIT_REPOSITORY https://github.com/JensKlimke/SimMap.git
            UPDATE_DISCONNECTED 1
            QUIET
            )

    # CMake warning suppression will not be needed in version 1.9
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
    add_subdirectory(${simmap_SOURCE_DIR} ${simmap_BINARY_DIR} EXCLUDE_FROM_ALL)
    unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)

else ()
    include(FetchContent)
    FetchContent_Declare(simmap
            GIT_REPOSITORY https://github.com/JensKlimke/SimMap.git
            )
    FetchContent_GetProperties(simmap)
    if (NOT simmap_POPULATED)
        FetchContent_Populate(simmap)
        set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
        add_subdirectory(${simmap_SOURCE_DIR} ${simmap_BINARY_DIR} EXCLUDE_FROM_ALL)
        unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    endif ()
endif ()


message(${simmap_SOURCE_DIR})
set(simmap_INCLUDE_DIR ${simmap_SOURCE_DIR}/include/ CACHE DIRECTORY "Include Directory of the OpneDRIVE parser" FORCE)
include_directories(${simmap_INCLUDE_DIR})


set_target_properties(simmap
        PROPERTIES FOLDER "Extern")
