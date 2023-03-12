# Makefile of integral3D program.
# The functionalities are:
# - "all"->compiles everything needed and create the executable
# - "test1"->compiles and execute the test function in test/function.cpp
# - "$(EXECUTABLE)"->compiles the executable
# - "$(LIB_DIR)/%.o"->create the object file of the required file
# - "clean"->removes all object files and the executable

CC = g++
CFLAGS = -c -Wall
LDFLAGS = -ldl

INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin
SRC_DIR = src

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(LIB_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/integral3D

all: $(SOURCES) $(EXECUTABLE)

test: all
	g++ -shared -fPIC test/function.cpp -o test/function.so
	./bin/integral3D test/function.so 0.1 5 3

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(LIB_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)