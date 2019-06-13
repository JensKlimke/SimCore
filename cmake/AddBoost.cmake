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

IF(NOT EXISTS ${boost_SOURCE_DIR}/boost)

    # ./boostrap.sh && ./b2 headers"

    # TODO: Windows
    message("creating headers ...")

    set(MAKE_CMD ./bootstrap.sh)
    message("COMMAND: ${MAKE_CMD}")

    execute_process(COMMAND ${MAKE_CMD}
            WORKING_DIRECTORY ${boost_SOURCE_DIR}
            RESULT_VARIABLE CMD_ERROR
            OUTPUT_FILE CMD_OUTPUT)

    set(MAKE_CMD ./b2 headers)
    message("COMMAND: ${MAKE_CMD}")

    execute_process(COMMAND ${MAKE_CMD}
            WORKING_DIRECTORY ${boost_SOURCE_DIR}
            RESULT_VARIABLE CMD_ERROR
            OUTPUT_FILE CMD_OUTPUT)

endif()

set(Boost_INCLUDE_DIRS ${boost_SOURCE_DIR} CACHE DIRECTORY "Include Directory of the OpneDRIVE parser" FORCE)
include_directories(${boost_SOURCE_DIR})