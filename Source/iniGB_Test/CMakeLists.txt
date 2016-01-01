cmake_minimum_required (VERSION 2.8)
project (iniGB_Test)

set(HDR_DIR ${PROJECT_SOURCE_DIR}/Include)
file(GLOB HDR_FILES ${HDR_DIR}/*)

set(SRC_DIR ${PROJECT_SOURCE_DIR})
aux_source_directory(${SRC_DIR} SRC_FILES)

set(HDR_DIR
	${HDR_DIR}
	${iniGB_HDR_DEPS}
)
include_directories(${HDR_DIR})

add_executable(iniGB_Test ${SRC_FILES} ${HDR_FILES})
