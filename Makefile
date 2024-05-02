#CXX Make variable for compiler
CC = g++

# Compiler flags
CCFLAGS=-std=c++11 -Wall -g3 -c

# Object files
OBJS = main.o readvocab.o readlines.o countvocabstrings.o trie.o SHARED_DATA.o

# Target executable
PROGRAM = countvocabstrings

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp

trie.o : trie.cpp trie.h
	$(CC) $(CCFLAGS) trie.cpp

readvocab.o : readvocab.cpp readvocab.h
	$(CC) $(CCFLAGS) readvocab.cpp

readlines.o: readlines.cpp readlines.h
	$(CC) $(CCFLAGS) readlines.cpp

countvocabstrings.o: countvocabstrings.cpp countvocabstrings.h
	$(CC) $(CCFLAGS) countvocabstrings.cpp

SHARED_DATA.o: SHARED_DATA.cpp SHARED_DATA.h
	$(CC) $(CCFLAGS) SHARED_DATA.cpp

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
clean :
	rm -f *.o *~ $(PROGRAM)
