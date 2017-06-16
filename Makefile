CXX?=g++

# For windows
ifeq ($(OS),Windows_NT)
	UNAME_S:=Windows_NT
	EXE:=n-body-sim.exe
	INC:=-Iinclude -Irapidxml -IC:\MinGW\include\boost -IC:\MinGW\include\SDL2
	LIB:=-lboost_system -lboost_thread -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc 
	SLASH_CHAR:=\\
	TEST_LIB= $(LIB)
	TEST_EXE:=test.exe
else
	UNAME_S:=$(shell uname -s)
	EXE:=n-body-sim
	SLASH_CHAR:=/
	TEST_LIB=-lpthread -lgtest $(LIB)
	TEST_EXE:=test
	# For Mac OS X
	ifeq ($(UNAME_S),Darwin) 
		INC:=-Iinclude/ -Irapidxml/ -I/opt/local/include/ -F/Library/Frameworks -framework SDL2
		LIB:=-F/Library/Frameworks -framework SDL2 -L/opt/local/lib -lpthread -lboost_system-mt -lboost_thread-mt
	endif
	# For Linux
	ifeq ($(UNAME_S),Linux) 
		INC:=-Iinclude/ -Irapidxml/
		LIB:=-lSDL2 -lpthread -lboost_system -lboost_thread
	endif
endif
OBJ_DIR:=obj
SRC_DIR:=src
SOURCES:=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS:=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)$(SLASH_CHAR)%.o, $(SOURCES))
DEBUG:=
TEST_OBJECTS:=$(patsubst $(OBJ_DIR)main.o, , $(OBJECTS))
TESTDIR:=tests
PREFIX?=/usr/local/bin

.PHONY: all
all: $(OBJ_DIR) $(EXE)

# The debug option cleans and builds the application with the -g compile flag
.PHONY: debug
debug: clean 
debug: DEBUG+=-g 
debug: all

$(EXE): $(OBJECTS)
	$(CXX) $(DEBUG) $^ $(LIB) -o $@
	
$(OBJ_DIR)$(SLASH_CHAR)%.o: $(SRC_DIR)$(SLASH_CHAR)%.cpp
	$(CXX) $(DEBUG) $(INC) -c $^ -o $@
	
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# test runs unit tests, which are used to verify the requirements 
.PHONY: test
test: $(OBJ_DIR) $(TESTDIR)$(SLASH_CHAR)$(TEST_EXE)
ifeq ($(UNAME_S),Windows_NT) 
	$(TESTDIR)$(SLASH_CHAR)$(TEST_EXE)
else
	./$(TESTDIR)$(SLASH_CHAR)$(TEST_EXE)
endif

$(TESTDIR)$(SLASH_CHAR)$(TEST_EXE) : $(TESTDIR)$(SLASH_CHAR)UnitTests.o $(TEST_OBJECTS) 
	$(CXX) $(DEBUG) $^ $(TEST_LIB) -o $@

$(TESTDIR)$(SLASH_CHAR)UnitTests.o : $(TESTDIR)$(SLASH_CHAR)UnitTests.cpp
	$(CXX) $(DEBUG) $(INC) -c $^ -o $@	
	
.PHONY: clean
clean:
ifeq ($(UNAME_S),Windows_NT) 
	DEL /F /s $(EXE) $(TESTDIR)$(SLASH_CHAR)$(TEST_EXE)
	rd /q /s $(OBJ_DIR)
else
	rm -rf $(EXE) $(OBJ_DIR) $(TESTDIR)/UnitTests.o $(TESTDIR)$(SLASH_CHAR)$(TEST_EXE)
endif

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
