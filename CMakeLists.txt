project(useLComp)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl")
INCLUDE_DIRECTORIES(include)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

FIND_FILE(MYLIB liblcomp.so ./)
IF(NOT MYLIB)
  MESSAGE(SEND_ERROR "Cannot find library xxxxlib.so")
ENDIF(NOT MYLIB)
EXEC_PROGRAM(/bin/ln ARGS "-s \"${MYLIB}\"
\"${CMAKE_CURRENT_BINARY_DIR}/libxxxxlib.so\""
  RETURN_VALUE ln_res)
IF("${ln_retval}" GREATER 0)
  MESSAGE(SEND_ERROR "Problem creating symlink from
\"${MYLIB}\" to \"${CMAKE_CURRENT_BINARY_DIR}/libxxxxlib.so\"")
ENDIF("${ln_retval}" GREATER 0)

#add_library(${PROJECT_NAME} liblcomp.so)
target_link_libraries(${PROJECT_NAME} "${CMAKE_DL_LIBS}")
TARGET_LINK_LIBRARIES(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/liblcomp.so")
