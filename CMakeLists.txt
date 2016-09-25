cmake_minimum_required(VERSION 3.5)
project(mish)

set(CMAKE_C_FLAGS "-std=c11")

set(SOURCE_FILES mish.c mish.c parser.c execute.c sighant.c)
add_executable(main ${SOURCE_FILES} mish.c)