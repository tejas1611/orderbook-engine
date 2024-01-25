CC = g++
CFLAGS =
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj

HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXECUTABLE = matching_engine

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)
