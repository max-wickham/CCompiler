LINK_TARGET = bin/c_compiler
CC = g++

CPPFLAGS += -std=c++17 -W -Wall -g #-Wno-unused-parameter
CPPFLAGS += -I include -I src #if not work add -I src/implementations

CPPALLTEST = ast_declaration.cpp ast_expressions.cpp ast_functions.cpp ast_operators.cpp ast_primitives.cpp ast_specifiers.cpp ast_statement.cpp c_compiler.cpp

HPPFILES = $(wildcard include/*.hpp) 
CPPFILES = $(wildcard src/implementations/*.cpp) $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))

all : $(LINK_TARGET)

$(LINK_TARGET) : src/c_lexer.yy.o src/c_parser.tab.o $(OBJS) 
	$(CC) $(CPPFLAGS) $^ -o $@

src/%.o: src/%.cpp $(HPPFILES)
	$(CC) $(CPPFLAGS) -c -o $@ $<

src/c_lexer.yy.cpp : src/c_lexer.flex src/c_parser.tab.hpp
	flex -o src/c_lexer.yy.cpp src/c_lexer.flex

src/c_parser.tab.cpp src/c_parser.tab.hpp: src/c_parser.y
	yacc -v -d src/c_parser.y -o src/c_parser.tab.cpp
	mkdir -p bin;


makeobj:
	$(CC) $(CPPFLAGS) src/$(CPPALLTEST) -o bin/testout

lexer: src/c_lexer.yy.cpp

parser: src/c_parser.tab.cpp src/c_parser.tab.hpp

bin/compiler: src/c_compiler.output
	
	
.PHONY: clean
clean :
	rm -rf bin/*
	rm -f src/*.tab.hpp
	rm -f src/*.tab.cpp
	rm -f src/*.yy.cpp
	rm -f src/*.output
	rm -f src/*.o
	rm -f src/implementations/*.o
	
