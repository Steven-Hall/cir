cmake_minimum_required(VERSION 3.16)
project(cir VERSION 0.1)
add_library(shared OBJECT src/stream.c
			  src/list.c
			  src/memory.c
		          src/types.c
		          src/lexer.c
		          src/parser.c)

 add_library(cir STATIC $<TARGET_OBJECTS:shared>)
target_compile_options(cir PRIVATE -Wall -Wextra -pedantic -Werror)

add_executable(cir_tester src/test_app.c $<TARGET_OBJECTS:shared>)
target_compile_options(cir_tester PRIVATE -Wall -Wextra -pedantic -Werror)
