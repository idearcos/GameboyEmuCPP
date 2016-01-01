cmake_minimum_required (VERSION 2.8)
project (iniGB)

if(WIN32)
	if (MINGW)
		add_definitions("-std=c++14")
	endif()
	if(MSVC)
		add_definitions("-DUNICODE -D_UNICODE")
	endif()
elseif(UNIX)
	add_definitions("-std=c++14")
endif()

set(GLEW_LIB_NAME "libglew32" CACHE STRING "Name of GLEW lib file")
set(GLEW_INCLUDE_DIR "" CACHE PATH "")
set(GLEW_LIB_DIR "" CACHE PATH "")
set(GLFW_LIB_NAME "glfw3" CACHE STRING "Name of GLFW lib file")
set(GLFW_INCLUDE_DIR "" CACHE PATH "")
set(GLFW_LIB_DIR "" CACHE PATH "")

add_subdirectory(iniGB)

