# NOTE: This Makefile is only for computers running Mac OS X and Linux
CXX?=g++
mkfile_path:=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir:=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
EXE:=$(current_dir)
UNAME_S:=$(shell uname -s)
# These if statements check to see which of the supported operating systems are 
# used and sets the correct include and library paths

# For Mac OS X
ifeq ($(UNAME_S),Darwin) 
	INC:=-I headers/ -I rapidxml/ -F/Library/Frameworks -framework SDL2
	LIB:=-F/Library/Frameworks -framework SDL2
	TEST_LIB=-lpthread -lgtest
endif
# For Linux
ifeq ($(UNAME_S),Linux) 
	INC:=-Iheaders/ -Irapidxml/
	LIB:=-lSDL2 -lpthread
	TEST_LIB=-lpthread -lgtest
endif
OBJ_DIR:=obj/
SRC_DIR:=src/
SOURCES:=$(wildcard $(SRC_DIR)*.cpp)
OBJECTS:=$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SOURCES))
TEST_OBJECTS:=$(patsubst $(OBJ_DIR)main.o, , $(OBJECTS))
DEBUG:=
TESTDIR:=tests

PREFIX?=/usr/local/bin

.PHONY: all
all: $(EXE) test

# The debug option cleans and builds the application with the -g compile flag
.PHONY: debug
debug: clean 
debug: DEBUG+=-g 
debug: all

$(EXE): $(OBJ_DIR) $(OBJECTS)
	$(CXX) $(DEBUG)$(OBJECTS) $(LIB) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(DEBUG)$(INC) -c $^ -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# test runs unit tests, which are used to verify the requirements 
.PHONY: test
test: $(OBJ_DIR) $(TESTDIR)/test
	./$(TESTDIR)/test

$(TESTDIR)/test : $(TESTDIR)/UnitTests.o $(TEST_OBJECTS) 
	$(CXX) $(DEBUG)$^ $(TEST_LIB) -o $@

$(TESTDIR)/UnitTests.o : $(TESTDIR)/UnitTests.cpp
	$(CXX) $(DEBUG)$(INC) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJ_DIR) $(TESTDIR)/UnitTests.o $(TESTDIR)/test

.PHONY: install
install: all
# For Mac OS X
ifeq ($(UNAME_S),Darwin) 
	install $(EXE) $(PREFIX) 
endif
# For Linux
ifeq ($(UNAME_S),Linux) 
	install -t $(PREFIX) $(EXE)
endif
	
	

