CC = g++
CFLAGS = -ggdb -std=c++23 -pedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Werror
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Source files
CPP_FILES := $(wildcard *.cpp)

# Object files
OBJ_FILES := $(CPP_FILES:.cpp=.o)

# Dependency files
DEP_FILES := $(CPP_FILES:.cpp=.d)

# Target executable
TARGET = noob

all: $(TARGET)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Link .o files into final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)
	$(RM) $(OBJ_FILES) $(DEP_FILES)

# Include .d files
-include $(DEP_FILES)

test:
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJ_FILES) $(DEP_FILES)
