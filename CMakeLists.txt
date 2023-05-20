cmake_minimum_required(VERSION 3.21)
project(2djourney LANGUAGES CXX)

set(src_dir src)
set(obj_dir objects)
set(inc_dir "include")

set(MAIN_SRC
    ${src_dir}/main.cpp
    ${src_dir}/core.cpp
	${inc_dir}/core.h
    ${src_dir}/object.cpp
	${inc_dir}/object.h
    ${src_dir}/actor.cpp
	${inc_dir}/actor.h
    ${src_dir}/text.cpp
	${inc_dir}/text.h
	${src_dir}/utils.cpp
	${inc_dir}/utils.h
	${inc_dir}/b2DebugDraw.h
	${src_dir}/b2DebugDraw.cpp
	${inc_dir}/generator.h
	${src_dir}/generator.cpp
)

set(OBJECTS_SRC
	${src_dir}/${obj_dir}/fpsCounter.cpp
	${inc_dir}/${obj_dir}/fpsCounter.h
	${src_dir}/${obj_dir}/debugInformer.cpp
	${inc_dir}/${obj_dir}/debugInformer.h
    ${src_dir}/${obj_dir}/player.cpp
	${inc_dir}/${obj_dir}/player.h
	${src_dir}/${obj_dir}/tile.cpp
	${inc_dir}/${obj_dir}/tile.h
	${src_dir}/${obj_dir}/map.cpp
	${inc_dir}/${obj_dir}/map.h
	${src_dir}/${obj_dir}/pauseScreen.cpp
	${inc_dir}/${obj_dir}/pauseScreen.h
	${src_dir}/${obj_dir}/background.cpp
	${inc_dir}/${obj_dir}/background.h
)
# message(NOTICE "${MAIN_SRC}")

add_executable(${PROJECT_NAME} ${MAIN_SRC} ${OBJECTS_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_Declare(box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG v2.4.1)
FetchContent_MakeAvailable(SFML box2d)



target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE box2d)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND_EXPAND_LISTS)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    			PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(
    	${PROJECT_NAME} PROPERTIES 
    	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/Debug"
    )
	
	## Add files into the solution explorer
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	function(groupFiles prefix sources)
		foreach(FILE ${sources}) 
			# Group into "Source Files" and "Header Files"
			if ("${FILE}" MATCHES ".*\\.c")
			   set(GROUP "${prefix}Source Files")
			elseif ("${FILE}" MATCHES ".*\\.cpp")
			   set(GROUP "${prefix}Source Files")
			elseif("${FILE}" MATCHES ".*\\.h")
			   set(GROUP "${prefix}Header Files")
			elseif("${FILE}" MATCHES ".*\\.hpp")
			   set(GROUP "${prefix}Header Files")
			endif()
			message(NOTICE "Found: [${GROUP}] :: ${FILE}")
			source_group("${GROUP}" FILES "${FILE}")
		endforeach()
	endfunction()
	groupFiles("Main " "${MAIN_SRC}" )
	groupFiles("Game objects " "${OBJECTS_SRC}")
endif()

install(TARGETS ${PROJECT_NAME})
