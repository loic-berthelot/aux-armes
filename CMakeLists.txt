cmake_minimum_required(VERSION 3.18.4)

project(Aux_armes)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})