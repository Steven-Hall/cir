cmake_minimum_required(VERSION 3.16)
project(cir VERSION 0.1)
include_directories(include)
add_library(cir src/lexer.c)

#target_compile_options(cir PRIVATE -Wall -Wextra -pedantic -Werror)
