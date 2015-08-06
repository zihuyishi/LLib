CC = clang++
CXXFLAGS = -std=c++11

all: btree

debug: CXXFLAGS += -g
debug: btree


btree: main.cpp binaryTree.h
	$(CC) -o btree main.cpp $(CXXFLAGS)

clean:
	rm -rf btree btree.dSYM
