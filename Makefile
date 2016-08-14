# NOTE: This Makefile is only for computers running Mac OS X
CC=g++
mkfile_path=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
EXE=$(current_dir)
INC=-I headers/
LIB=-F/Library/Frameworks -framework SDL2
OBJ_DIR=obj/
SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES))

$(EXE): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(LIB) $(OBJECTS) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(INC) -c $^ -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJ_DIR)
