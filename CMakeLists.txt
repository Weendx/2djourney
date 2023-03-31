cmake_minimum_required(VERSION 3.21)
set(project 2djourney)
project(${project} LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(src src)
add_executable(${project} 
    src/main.cpp
    src/core.cpp
    src/object.cpp
    src/actor.cpp
    src/text.cpp
)

# include_directories("${CMAKE_SOURCE_DIR}/src")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${project} PRIVATE sfml-graphics)
target_compile_features(${project} PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${project} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${project}> $<TARGET_FILE_DIR:${project}> COMMAND_EXPAND_LISTS)
endif()

# target_include_directories(${project} PUBLIC "${CMAKE_SOURCE_DIR}/src")
install(TARGETS ${project})


#message(NOTICE "dir loc: ${CMAKE_BINARY_DIR}")
