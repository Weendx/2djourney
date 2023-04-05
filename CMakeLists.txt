cmake_minimum_required(VERSION 3.21)
project(2djourney LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(src src)
set(objects objects)
add_executable(${PROJECT_NAME} 
    ${src}/main.cpp
    ${src}/core.cpp
    ${src}/object.cpp
    ${src}/actor.cpp
    ${src}/text.cpp
    ${src}/player.cpp
    ${src}/${objects}/fpsCounter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${project}> COMMAND_EXPAND_LISTS)
endif()

if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
endif()

# target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/src")
install(TARGETS ${PROJECT_NAME})


#message(NOTICE "dir loc: ${CMAKE_BINARY_DIR}")
