PROJECT(First)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

aux_source_directory(src DIR_SRCS)
add_executable(out ${DIR_SRCS})