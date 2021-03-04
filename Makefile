  
CPPFLAGS += -std=c++17 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I inc

COMPILERINC := -Ic_compiler/include
COMPILERBUILDDIR := c_compiler/build
COMPILERSRCDIR := c_compiler/src

bin/c_lexer.yy.cpp : src/c_lexer.flex
	flex -o src/c_lexer.yy.cpp src/c_lexer.flex


bin/compiler : src/compiler.cpp
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/compiler $^

bin/c_compiler : bin/compiler src/wrapper.sh
	cp src/wrapper.sh bin/c_compiler
	chmod u+x bin/c_compiler
	
clean :
	rm -f src/*.o
	rm -f src/c_lexer.yy.cpp
	rm -f bin/*
	rm -f test_program.s
	rm -f test_program
	rm -f test_program.o
	
