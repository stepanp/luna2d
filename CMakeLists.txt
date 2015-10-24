project(luna2d)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
	message(FATAL_ERROR "CMAKE_BUILD_TYPE is not set, it must be Debug or Release")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

# Debug config
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(LIB_NAME ${PROJECT_NAME}d)

# Release config
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(LIB_NAME ${PROJECT_NAME})
endif()


# Set platform name
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	set(PLATFROM_NAME "android")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "WindowsPhone")
	set(PLATFROM_NAME "wp")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(PLATFROM_NAME "qt")

endif()


# luna2d sources
set(LUNA2D_DIR ${PROJECT_SOURCE_DIR}/luna2d)

set(LUNA2D_DIRS
	${LUNA2D_DIR}	
	${LUNA2D_DIR}/assets
	${LUNA2D_DIR}/audio
	${LUNA2D_DIR}/common
	${LUNA2D_DIR}/graphics
	${LUNA2D_DIR}/graphics/shaders
	${LUNA2D_DIR}/graphics/particles
	${LUNA2D_DIR}/lua
	${LUNA2D_DIR}/lua/scripts
	${LUNA2D_DIR}/math
	${LUNA2D_DIR}/modules
	${LUNA2D_DIR}/physics
	${LUNA2D_DIR}/physics/shapes
	${LUNA2D_DIR}/physics/joints
	${LUNA2D_DIR}/platform
	${LUNA2D_DIR}/utils
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
	${THIRDPARTY_DIR}/freetype
	${THIRDPARTY_DIR}/json11
	${THIRDPARTY_DIR}/libpng
	${THIRDPARTY_DIR}/utf8cpp
	${THIRDPARTY_DIR}/libzip
	${THIRDPARTY_DIR}/zlib
	${THIRDPARTY_DIR}/ogg
	${THIRDPARTY_DIR}/vorbis
)

include_directories(${THIRDPARTY_DIRS})


# Lua
aux_source_directory(${THIRDPARTY_DIR}/lua THIRDPARTY_SOURCES)

# libpng
aux_source_directory(${THIRDPARTY_DIR}/libpng THIRDPARTY_SOURCES)

# Box2D
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Collision THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Collision/Shapes THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Common THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Contacts THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Joints THIRDPARTY_SOURCES)
aux_source_directory(${THIRDPARTY_DIR}/Box2D/Dynamics/Rope THIRDPARTY_SOURCES)

# Freetype
set(FREETYPE_PATH ${THIRDPARTY_DIR}/freetype)
include(${FREETYPE_PATH}/freetype.cmake)
list(APPEND THIRDPARTY_SOURCES ${FREETYPE_SOURCES})

# json11
aux_source_directory(${THIRDPARTY_DIR}/json11 THIRDPARTY_SOURCES)

# zlib
aux_source_directory(${THIRDPARTY_DIR}/zlib THIRDPARTY_SOURCES)

# libogg
aux_source_directory(${THIRDPARTY_DIR}/ogg THIRDPARTY_SOURCES)

# libvorbis
aux_source_directory(${THIRDPARTY_DIR}/vorbis THIRDPARTY_SOURCES)


# Build Qt static library
if(${PLATFROM_NAME} STREQUAL "qt")
	# Qt5 support
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5OpenGL REQUIRED)
	find_package(Qt5Multimedia REQUIRED)

	# luna2d Qt source
	set(LUNA2D_QT_DIR ${LUNA2D_DIR}/platform/qt)
	include_directories(${LUNA2D_QT_DIR})
	aux_source_directory(${LUNA2D_QT_DIR} LUNA2D_SOURCES)
	file (GLOB LUNA2D_QT_HEADERS "${LUNA2D_QT_DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${LUNA2D_QT_HEADERS})

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME})

	add_library(${LIB_NAME} STATIC ${LUNA2D_SOURCES} ${LUNA2D_HEADERS} ${THIRDPARTY_SOURCES})

	qt5_use_modules(${LIB_NAME} Widgets)
	qt5_use_modules(${LIB_NAME} OpenGL)
	qt5_use_modules(${LIB_NAME} Multimedia)


# Build Android shared library
elseif(${PLATFROM_NAME} STREQUAL "android")
	# Set optimization level same as in ndk-build
	string(REPLACE "-O3" "-Os" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
	string(REPLACE "-O3" "-Os" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

	# Disable RTTI and exceptions
	string(REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	string(REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

	# libzip
	aux_source_directory(${THIRDPARTY_DIR}/libzip THIRDPARTY_SOURCES)

	# Add JNI includes
	include_directories(${JNI_INCLUDE_DIRS})

	# Add luna2d Android sources
	set(LUNA2D_ANDROID_DIR ${LUNA2D_DIR}/platform/android)
	include_directories(${LUNA2D_ANDROID_DIR})
	aux_source_directory(${LUNA2D_ANDROID_DIR} LUNA2D_SOURCES)
	file (GLOB LUNA2D_ANDROID_HEADERS "${LUNA2D_ANDROID_DIR}/*.h")
	list(APPEND LUNA2D_HEADERS ${LUNA2D_ANDROID_HEADERS})
	list(APPEND LUNA2D_SOURCES ${LUNA2D_ANDROID_DIR}/jni/lunajnibridge.cpp)

	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFROM_NAME}/${ANDROID_ABI})

	add_library(${LIB_NAME} SHARED ${LUNA2D_SOURCES} ${LUNA2D_HEADERS} ${THIRDPARTY_SOURCES})

	target_link_libraries(${LIB_NAME} GLESv2 log android)


# Build Windows Phone shared library
elseif(${PLATFROM_NAME} STREQUAL "wp")
	cmake_minimum_required(VERSION 3.3)

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

	# Add xaml files
	set(LUNA2D_WP_XAML ${LUNA2D_WP_APP_DIR}/OpenGLESPage.xaml)

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

	foreach(DIR ${THIRDPARTY_DIRS})
		STRING(REPLACE "${PROJECT_SOURCE_DIR}/" "" GROUP_NAME ${DIR})
		STRING(REPLACE "/" "\\" GROUP_NAME ${GROUP_NAME})		
		source_group(${GROUP_NAME} ${DIR})
	endforeach()

	source_group("luna2d\\platform\\wp" ${LUNA2D_WP_DIR})
	source_group("luna2d\\platform\\wp\\application" ${LUNA2D_WP_DIR}/application)

	add_library(${LIB_NAME} SHARED ${LUNA2D_SOURCES} ${LUNA2D_HEADERS} ${THIRDPARTY_SOURCES}
		${LUNA2D_WP_SOURCES} ${LUNA2D_WP_XAML})

	# Link angle static libs
	target_link_libraries(${LIB_NAME} ${ANGLE_LIB_PATH}/libEGL.lib)
	target_link_libraries(${LIB_NAME} ${ANGLE_LIB_PATH}/libGLESv2.lib)

	set_property(TARGET ${LIB_NAME} PROPERTY VS_GLOBAL_ROOTNAMESPACE "luna2d")

	set_property(TARGET ${LIB_NAME} PROPERTY FOLDER "luna2d")

	set_property(TARGET ${LIB_NAME} PROPERTY VS_WINRT_EXTENSIONS TRUE)
	set_property(TARGET ${LIB_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)

else()
	message(FATAL_ERROR "Unknown platform")
endif()
