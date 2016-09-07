# NOTE: This Makefile is only for computers running Mac OS X
CC=g++
mkfile_path=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
EXE=$(current_dir)
INC=-I headers/ -I rapidxml/
LIB=-F/Library/Frameworks -framework SDL2
OBJ_DIR=obj/
SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES))
DEBUG=

all: $(EXE)

debug: clean 
debug: DEBUG+=-g 
debug: $(EXE)	

$(EXE): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(DEBUG) $(LIB) $(OBJECTS) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(DEBUG) $(INC) -c $^ -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJ_DIR)

