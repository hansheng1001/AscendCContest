# npu
file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/*.cpp
)
set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE CCE)

add_executable(${smoke_testcase}_npu
    ${SRC_FILES}
)

target_compile_options(${smoke_testcase}_npu PRIVATE
    -O2
    -std=c++17
)

target_link_directories(${smoke_testcase}_npu PRIVATE
    ${ASCEND_INSTALL_PATH}/lib64
)

set_target_properties(${smoke_testcase}_npu PROPERTIES
    OUTPUT_NAME ${smoke_testcase}_${ASCEND_RUN_MODE}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(${smoke_testcase}_sim ALL)
add_dependencies(${smoke_testcase}_sim ${smoke_testcase}_npu)
