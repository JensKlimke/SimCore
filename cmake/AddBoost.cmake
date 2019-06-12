# 
#
# Downloads boost library
#
#

message("Downloading boost ...")

include(FetchContent)


include(FetchContent)
FetchContent_Declare(boost
            GIT_REPOSITORY      https://github.com/boostorg/boost.git
            GIT_TAG             boost-1.69.0
            CONFIGURE_COMMAND   "echo installing"
        )
FetchContent_GetProperties(boost)

if(NOT boost_POPULATED)
    FetchContent_Populate(boost)
endif()

execute_process(
        COMMAND "echo installing && ./boostrap.sh && ./b2 headers"
        WORKING_DIRECTORY ${boost_SOURCE_DIR})


set(Boost_INCLUDE_DIRS ${boost_SOURCE_DIR}/include/ CACHE DIRECTORY "Include Directory of the OpneDRIVE parser" FORCE)
include_directories(${boost_INCLUDE_DIR})