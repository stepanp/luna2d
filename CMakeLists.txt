project(luna2d)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
	message(FATAL_ERROR "CMAKE_BUILD_TYPE is not set, it must be Debug or Release")
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)


# Debug config
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(LIB_NAME ${PROJECT_NAME}d)

# Release config
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(LIB_NAME ${PROJECT_NAME})
endif()


# luna2d sources
set(LUNA2D_DIR ${PROJECT_SOURCE_DIR}/luna2d)

set(LUNA2D_DIRS
	${LUNA2D_DIR}
	${LUNA2D_DIR}/lua
	${LUNA2D_DIR}/lua/scripts
	${LUNA2D_DIR}/assets
	${LUNA2D_DIR}/graphics
	${LUNA2D_DIR}/graphics/shaders
	${LUNA2D_DIR}/modules
	${LUNA2D_DIR}/physics
	${LUNA2D_DIR}/physics/shapes
	${LUNA2D_DIR}/physics/joints
	${LUNA2D_DIR}/platform
	${LUNA2D_DIR}/utils
	${LUNA2D_DIR}/utils/math
)

foreach(DIR ${LUNA2D_DIRS})
	aux_source_directory(${DIR} LUNA2D_SOURCES)
	file (GLOB TEMP "${DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${TEMP})
endforeach()

include_directories(${LUNA2D_DIRS})


# Thirdparty sources
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

set(THIRDPARTY_DIRS
	${THIRDPARTY_DIR}
	${THIRDPARTY_DIR}/lua
	${THIRDPARTY_DIR}/Box2D
	${THIRDPARTY_DIR}/json11
	${THIRDPARTY_DIR}/libpng
	${THIRDPARTY_DIR}/utf8cpp
	${THIRDPARTY_DIR}/zlib
)

include_directories(${THIRDPARTY_DIRS})


# Lua
aux_source_directory(${THIRDPARTY_DIR}/lua LUA_SOURCES)
add_library(lua OBJECT ${LUA_SOURCES})

# libpng
aux_source_directory(${THIRDPARTY_DIR}/libpng PNG_SOURCES)
add_library(png OBJECT ${PNG_SOURCES})

# Box2D
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Collision BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Collision/Shapes BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Common BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Contacts BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Joints BOX2D_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Rope BOX2D_SOURCES)
add_library(Box2D OBJECT ${BOX2D_SOURCES})

# Freetype
set(FREETYPE_PATH ${THIRDPARTY_DIR}/freetype)
include(${FREETYPE_PATH}/freetype.cmake)

# json11
aux_source_directory(${THIRDPARTY_DIR}/json11 JSON11_SOURCES)
add_library(json11 OBJECT ${JSON11_SOURCES})

# zlib
aux_source_directory(${THIRDPARTY_DIR}/zlib ZLIB_SOURCES)
add_library(z OBJECT ${ZLIB_SOURCES})


# Set platform name
if(${CMAKE_SYSTEM_NAME} STREQUAL "WindowsPhone")
	set(PLATFROM_NAME "wp")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(PLATFROM_NAME "qt")
endif()


# Build Qt static library
if(${PLATFROM_NAME} STREQUAL "qt")
	# Qt5 support
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5OpenGL REQUIRED)

	# luna2d Qt source
	set(LUNA2D_QT_DIR ${LUNA2D_DIR}/platform/qt)
	include_directories(${LUNA2D_QT_DIR})
	aux_source_directory(${LUNA2D_QT_DIR} LUNA2D_SOURCES)
	file (GLOB LUNA2D_QT_HEADERS "${LUNA2D_QT_DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${TEMP})

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME})

	add_library(${LIB_NAME} STATIC ${LUNA2D_SOURCES} ${LUNA2D_HEADERS}
		$<TARGET_OBJECTS:lua>
		$<TARGET_OBJECTS:png>
		$<TARGET_OBJECTS:json11>
		$<TARGET_OBJECTS:Box2D>
		$<TARGET_OBJECTS:freetype>
		$<TARGET_OBJECTS:z>
	)

	qt5_use_modules(${LIB_NAME} Widgets)
	qt5_use_modules(${LIB_NAME} OpenGL)


# Build Windows Phone shared library
elseif(${PLATFROM_NAME} STREQUAL "wp")
	cmake_minimum_required(VERSION 3.2)

	set(ANGLE_PATH ${THIRDPARTY_DIR}/angle)
	set(ANGLE_INCLUDE_PATH ${ANGLE_PATH}/include)
	set(ANGLE_LIB_PATH ${ANGLE_PATH}/prebuilt/wp/arm)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm200 /wd\"4251\" /wd\"4457\" /wd\"4458\"")
	#C4244

	# Include angle
	include_directories(${ANGLE_INCLUDE_PATH})

	# luna2d Windows Phone source
	set(LUNA2D_WP_DIR ${LUNA2D_DIR}/platform/wp)
	include_directories(${LUNA2D_WP_DIR})
	aux_source_directory(${LUNA2D_WP_DIR} LUNA2D_WP_SOURCES)
	file (GLOB LUNA2D_WP_HEADERS "${LUNA2D_WP_DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${LUNA2D_WP_HEADERS})

	set(LUNA2D_WP_APP_DIR ${LUNA2D_WP_DIR}/application)
	include_directories(${LUNA2D_WP_APP_DIR})
	aux_source_directory(${LUNA2D_WP_APP_DIR} LUNA2D_WP_SOURCES)
	file (GLOB LUNA2D_WP_APP_HEADERS "${LUNA2D_WP_APP_DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${LUNA2D_WP_APP_HEADERS})
	file (GLOB LUNA2D_WP_APP_XAML "${LUNA2D_WP_APP_DIR}/*.xaml")
	list(APPEND LUNA2D_HEADERS ${LUNA2D_WP_APP_XAML})

	# Add xaml files
	#set(LUNA2D_WP_XAML ${LUNA2D_WP_APP_DIR}/App.xaml ${LUNA2D_WP_APP_DIR}/OpenGLESPage.xaml)
	set_property(SOURCE ${LUNA2D_WP_APP_DIR}/App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")

	# Set same ouput directory for debug and release configurations
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME})
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME})
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME})

	# Enable WinRT extesion only for luna2d wp files
	set_source_files_properties(${LUNA2D_WP_SOURCES} PROPERTIES COMPILE_FLAGS -ZW)

	# Make source groups
	foreach(DIR ${LUNA2D_DIRS})
		STRING(REPLACE "${PROJECT_SOURCE_DIR}/" "" GROUP_NAME ${DIR})
		STRING(REPLACE "/" "\\" GROUP_NAME ${GROUP_NAME})		
		source_group(${GROUP_NAME} ${DIR})
	endforeach()
	source_group("luna2d\\platform\\wp" ${LUNA2D_WP_DIR})
	source_group("luna2d\\platform\\wp\\application" ${LUNA2D_WP_DIR}/application)

	add_library(${LIB_NAME} SHARED ${LUNA2D_SOURCES} ${LUNA2D_HEADERS} ${LUNA2D_WP_SOURCES} #${LUNA2D_WP_XAML}
		$<TARGET_OBJECTS:lua>
		$<TARGET_OBJECTS:png>
		$<TARGET_OBJECTS:json11>
		$<TARGET_OBJECTS:Box2D>
		$<TARGET_OBJECTS:freetype>
		$<TARGET_OBJECTS:z>
	)

	# Link angle static libs
	target_link_libraries(${LIB_NAME} ${ANGLE_LIB_PATH}/libEGL.lib)
	target_link_libraries(${LIB_NAME} ${ANGLE_LIB_PATH}/libGLESv2.lib)

	# Set groups for projects
	set_property(TARGET ${LIB_NAME} PROPERTY FOLDER "luna2d")
	set_property(TARGET lua PROPERTY FOLDER "thirdparty")
	set_property(TARGET png PROPERTY FOLDER "thirdparty")
	set_property(TARGET json11 PROPERTY FOLDER "thirdparty")
	set_property(TARGET Box2D PROPERTY FOLDER "thirdparty")
	set_property(TARGET z PROPERTY FOLDER "thirdparty")

	set_property(TARGET ${LIB_NAME} PROPERTY VS_WINRT_EXTENSIONS TRUE)
	set_property(TARGET ${LIB_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)

else()
	message(FATAL_ERROR "Unknown platform")
endif()
