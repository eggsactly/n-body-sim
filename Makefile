# NOTE: This Makefile is only for computers running Mac OS X
CC=g++
mkfile_path=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
EXE=$(current_dir)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	INC=-I headers/ -I rapidxml/ -F/Library/Frameworks -framework SDL2
	LIB=-F/Library/Frameworks -framework SDL2
endif
ifeq ($(UNAME_S),Linux)
	INC=-I headers/ -I rapidxml/ 
	LIB=-lSDL2
endif
OBJ_DIR=obj/
SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES))
TEST_OBJECTS=$(patsubst $(OBJ_DIR)main.o, , $(OBJECTS))
DEBUG=
TESTDIR=tests

all: $(EXE) $(TESTDIR)/test
	./$(TESTDIR)/test

debug: clean 
debug: DEBUG+=-g 
debug: $(EXE)	

$(TESTDIR)/test : $(TESTDIR)/UnitTests.o $(OBJ_DIR) $(TEST_OBJECTS) 
	$(CC) $(DEBUG) $(INC) $(LIB) $< $(TEST_OBJECTS) -o $@

$(TESTDIR)/UnitTests.o : $(TESTDIR)/UnitTests.cpp
	$(CC) $(DEBUG) $(INC) -c $^ -o $@

$(EXE): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(DEBUG) $(LIB) $(OBJECTS) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(DEBUG) $(INC) -c $^ -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJ_DIR) $(TESTDIR)/UnitTests.o $(TESTDIR)/test

