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

# to be eliminated
test1: all
	g++ -shared -fPIC test/function.cpp -o test/function.so
	./bin/integral3D test/function.so

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# $(LIB_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h $(INCLUDE_DIR)/error.h
# 	$(CC) $(CFLAGS) $< -o $@

$(LIB_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)