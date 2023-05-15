cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(Aux_armes)

aux_source_directory(. SRC_LIST)
aux_source_directory(./decisions SRC_LIST)
aux_source_directory(./monde SRC_LIST)
aux_source_directory(./ordres SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})