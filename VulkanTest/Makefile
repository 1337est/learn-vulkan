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
CFLAGS = -ggdb -std=c++23 -pedantic -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion

# Linker flags
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	$(CC) $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

# The 'clean' target, which is used to remove generated files and start fresh
# Runnings 'make clean' will remove the executable and all object files
clean:
	rm -f VulkanTest
