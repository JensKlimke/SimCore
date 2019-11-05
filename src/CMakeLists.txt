add_subdirectory(socket)

add_library(simcore STATIC core/Loop.cpp)
target_link_libraries(simcore PUBLIC websocket)
target_include_directories(simcore PUBLIC ./)
