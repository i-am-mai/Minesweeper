# Compiler options
CC := g++
CFLAGS := -c -I./include -DSFML_STATIC
LDFLAGS := -L./lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32

# Directories
SRC_DIR := src
OBJ_DIR := obj

# Source files and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Output executable
EXECUTABLE := Minesweeper

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Create the obj directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)