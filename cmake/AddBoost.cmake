# 
#
# Downloads boost library
#
#

message("Downloading boost ...")

if (CMAKE_VERSION VERSION_LESS 3.11)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

    include(DownloadProject)
    download_project(PROJ boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG        boost-1.69.0
            UPDATE_DISCONNECTED 1
            QUIET
            )

else ()
    include(FetchContent)
    FetchContent_Declare(boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG        boost-1.69.0
            )
    FetchContent_GetProperties(boost)
    if (NOT boost_POPULATED)
        FetchContent_Populate(boost)
    endif ()
endif ()


set(Boost_INCLUDE_DIRS ${boost_SOURCE_DIR}/include/ CACHE DIRECTORY "Include Directory of the OpneDRIVE parser" FORCE)
include_directories(${boost_INCLUDE_DIR})