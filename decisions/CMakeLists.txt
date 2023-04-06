cmake_minimum_required(VERSION 3.18.4)

project(decisions)
aux_source_directory(. SRC_LIST2)
add_executable(${PROJECT_NAME} ${SRC_LIST2})

#[[

set(MYPROJECT_DIR "..")
include_directories(${MY_PROJECT_DIR})
]]

