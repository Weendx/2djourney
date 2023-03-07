cmake_minimum_required(VERSION 3.21)
project(2djourney LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_executable(2djourney src/main.cpp)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(2djourney PRIVATE sfml-graphics)
target_compile_features(2djourney PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET 2djourney POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:2djourney> $<TARGET_FILE_DIR:2djourney> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS 2djourney)

#message(NOTICE "dir loc: ${CMAKE_BINARY_DIR}")
