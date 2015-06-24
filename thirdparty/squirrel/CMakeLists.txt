set(SQUIRREL_PATH, "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${SQUIRREL_PATH}/include)
aux_source_directory(${SQUIRREL_PATH}/squirrel THIRDPARTY_SOURCES)
aux_source_directory(${SQUIRREL_PATH}/sqstdlib THIRDPARTY_SOURCES)