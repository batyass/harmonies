# --- Compiler Settings ---
CXX = g++
# -Iinclude tells the compiler to look for header files in the include/ directory
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# --- Target and Folders ---
TARGET = prog
SRC_DIR = src

# --- File Discovery (Windows Compatible) ---
# We manually specify the subfolders to avoid the 'find' command conflict on Windows
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/model/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp) \
       $(wildcard $(SRC_DIR)/core/*.cpp) \
       $(wildcard $(SRC_DIR)/rules/*.cpp) \
       $(wildcard $(SRC_DIR)/scoring/*.cpp) \
       $(wildcard $(SRC_DIR)/ui/*.cpp)

# Convert the list of .cpp files into a list of .o (object) files
OBJS = $(SRCS:.cpp=.o)

# --- Build Rules ---

# Default rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) -o $(TARGET)

# Compile .cpp files into .o files
# $< is the source file (.cpp)
# $@ is the target file (.o)
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Utility Rules ---

# Clean up build artifacts
# Use 'del' for Windows or 'rm' if using MinGW/Git Bash
clean:
	@echo "Cleaning up..."
	-del /s /q *.o
	-del /q $(TARGET).exe

# Prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean


# Run the application
run: $(TARGET)
	./$(TARGET)