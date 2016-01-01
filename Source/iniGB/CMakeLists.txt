cmake_minimum_required (VERSION 2.8)
project (iniGB)

set(HDR_DIR ${PROJECT_SOURCE_DIR}/Include)
file(GLOB HDR_FILES ${HDR_DIR}/*)

set(SRC_DIR ${PROJECT_SOURCE_DIR})
aux_source_directory(${SRC_DIR} SRC_FILES)

set(HDR_DIR
	${HDR_DIR}
	${GLEW_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
)
include_directories(${HDR_DIR})

link_directories(${GLEW_LIB_DIR})
link_directories(${GLFW_LIB_DIR})

add_executable(iniGB ${SRC_FILES} ${HDR_FILES})
target_link_libraries(iniGB opengl32)
target_link_libraries(iniGB ${GLEW_LIB_NAME})
target_link_libraries(iniGB ${GLFW_LIB_NAME})

set(iniGB_HDR_DEPS ${HDR_DIR} PARENT_SCOPE)
