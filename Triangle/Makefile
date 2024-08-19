CC = g++
CFLAGS = -ggdb -std=c++23 -pedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Werror
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
TARGET = noob

all: $(TARGET)

%.o: %.cpp # Compile cpp files
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES) # Link obj files
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)
	$(RM) $(OBJ_FILES)

test:
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJ_FILES)
