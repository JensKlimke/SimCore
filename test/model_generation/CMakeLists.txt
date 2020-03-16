# source files
set(SOURCE_FILES
        ModelTest.cpp
        ModelClass_Interface.h)

add_executable(ModelGenerationTest ${SOURCE_FILES})

add_gtest(ModelGenerationTest)
