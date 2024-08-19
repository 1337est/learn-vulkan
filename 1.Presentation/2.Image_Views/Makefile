# the compiler CC: gcc for C programs, define as g++ for c++
CC = g++

# compiler flags CFLAGS:
#  -ggdb				- Includes debugging info in the executable,
#  						  allowing you to use tools like GDB (GNU Debugger)
#
#  -std=c++23			- Specifies the C++ language standard to be C++23
#
#  -pedantic			- Generates additional warning and errors to enforce
#  						  strict adherence to the C++ standard. It helps ensure
#  						  portability and compliance with language specifications
#
#  -Wall 				- Enables set of common warnings.
#  						  It stands for "all warnings"
#
#  -Weffc++				- Warns about violations of Effective C++ coding
#  						  standards. This set of guidelines aims to improve
#  						  C++ code quality and maintainability
#
#  -Wextra				- Enables additional warning messages not covered by -Wall
#
#  -Wconversion			- Warns about implicit type conversions that may
#  						  result in loss of data
#
#  -Wsign-conversion	- Warns about sign-conversion issues when converting
#  						  between signed and unsigned integers
#
#  -Werror				- Treats all warning messages as errors, making the
#  						  compilation fail if there are any warnings. Used to
#  						  enforce a strict policy on code quality
CFLAGS = -ggdb -std=c++23 -pedantic -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror

# Linker flags
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Get all .cpp files in the current directory
CPP_FILES := $(wildcard *.cpp)
# The ':=' operator is used to ensure variable assignment only once,
# even if the Makefile is executed multiple times

# Generate corresponding .o files corresponding to the .cpp source files
OBJ_FILES := $(CPP_FILES:.cpp=.o)
# $(CPP_FILES:.cpp=.o): Substitution reference that iterates over each item in
# the .cpp list and replaces the .cpp extension with .o. This essentially says
# for each item in CPP_FILES, replace the .cpp with .o

# The build target/our executable
TARGET = main

# The default target 'all' specifies that when you run 'make' without any target
# it should build the target specified in its dependency list, which is $(TARGET)
all: $(TARGET)

# Rule for the compilation stage:
# Compiles .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
#	%.o: This is the target pattern. It specifies a rule for building any file
#		with a .o extension. The % symbol is a wildcard character that matches
#		any string. So %.o matches any file with a .o extension
#	%.cpp: This is the prerequisite pattern. It specifies that for each .o file
#		target, the corresponding .cpp file is the prerequisite.
#	$(CC): Compiler command (g++)
#	$(CFLAGS): Compiler flags, including debuggin info, C++ standard, warnings, etc
#	-c: This flag tells the compiler to generate object files without linking.
#		It's used when compiling individual source files.
#	$<: Automatic variable that represents the first prerequisite, which is the
#		source file (%.cpp in this case). It's used to specify the input file
#		to be compiled.
#	-o $@: Automatic variable that specifies the name of the output file. In
#		this case, $@ represents the target name, which is the .o file being
#		generated. So, the output file will be named after the target file %.o

# Rule for the linking stage:
# Creates the final executable using the rule to build the $(TARGET) using the
# $(OBJ_FILES) by invoking the g++ compiler with the CFLAGS and then linking
# all the OBJ_FILES ($^) into an executable named main (-o $@).
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJ_FILES)
#	$(TARGET): Variable to build which is an executable file 'main'
#	$(OBJ_FILES): List of prerequisites for the target.
#		Here represents the object files needed to build the target.
#	$(CC): Compiler command (g++)
#	$(CFLAGS): Compiler flags
#	-o: -o specifies the output file name.
#	$@: automatic variable that represents the target name (main).
#	$^: automatic variable that represents all the prerequisites of the rule.
#		Here it represents all the object files needed to build the target.
#		The order of prerequisites is preserved but duplicates are removed.

debug: $(TARGET)
	gdb ./$(TARGET)

test:
	./main

# The 'clean' target, which is used to remove generated files and start fresh
# Runnings 'make clean' will remove the executable and all object files
clean:
	$(RM) $(TARGET) $(OBJ_FILES)
#	$(RM): Remove command
#	$(TARGET): The target file to be removed
#	$(OBJ_FILES): List of all object files

