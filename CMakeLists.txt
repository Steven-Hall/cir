cmake_minimum_required(VERSION 3.16)
project(cir VERSION 0.1)
add_library(cir STATIC src/string_utils.c
			  src/stream.c
			  src/memory.c
			  src/types/list.c
		          src/types/cir.c
			  src/types/atom.c
			  src/types/function.c
			  src/types/statement.c
		          src/lexer.c
		          src/parser.c
			  src/tokens.c
			  src/cir.c)

target_compile_options(cir PRIVATE -Wall -Wextra -pedantic -Werror)

add_executable(cir_test src/test_app.c)
target_compile_options(cir_test PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(cir_test cir)
