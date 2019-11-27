
CFLAGS=-lfl `llvm-config-6.0 --cxxflags --cppflags mcjit native --ldflags --libs core` -ltinfo -lpthread -ldl 

parser: scanner.l parser.y
	flex scanner.l
	bison -vd  parser.y
	g++ -g -Wall -o parser lex.yy.c parser.tab.c AST.cpp SemanticVisitor.cpp visit.cpp  $(CFLAGS) -std=c++14
	./test.sh
clean:
	rm lex.yy.c parser.tab.c parser.tab.h parser.output parser compiled_file output.o
