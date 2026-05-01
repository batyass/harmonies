# --- Compiler Settings ---
CXX = g++
# -Iinclude tells the compiler to look for header files in the include/ directory
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# --- Target and Folders ---
TARGET = prog
SRC_DIR = src
TEST_DIR = tests

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

TEST_HEXCOORD = $(TEST_DIR)/testHexCoord
TEST_BOARDCELL = $(TEST_DIR)/testBoardCell
TEST_PERSONALBOARD = $(TEST_DIR)/testPersonalBoard
TEST_TOKENBAG = $(TEST_DIR)/testTokenBag
TEST_TOKENSLOT = $(TEST_DIR)/testTokenSlot
TEST_CENTRALBOARD = $(TEST_DIR)/testCentralBoard
TEST_PATTERN = $(TEST_DIR)/testPattern
TEST_ANIMALCARD = $(TEST_DIR)/testAnimalCard
TEST_ANIMALCARDDECK = $(TEST_DIR)/testAnimalCardDeck
TEST_NATURESPIRITCARD = $(TEST_DIR)/testNatureSpiritCard
TEST_NATURESPIRITDECK = $(TEST_DIR)/testNatureSpiritDeck
TESTS = $(TEST_HEXCOORD) $(TEST_BOARDCELL) $(TEST_PERSONALBOARD) $(TEST_TOKENBAG) $(TEST_TOKENSLOT) $(TEST_CENTRALBOARD) $(TEST_PATTERN) $(TEST_ANIMALCARD) $(TEST_ANIMALCARDDECK) $(TEST_NATURESPIRITCARD) $(TEST_NATURESPIRITDECK)

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
# Use 'rm' on macOS/Linux
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(TARGET)
	rm -f $(TEST_HEXCOORD) $(TEST_BOARDCELL) $(TEST_PERSONALBOARD) $(TEST_TOKENBAG) $(TEST_TOKENSLOT) $(TEST_CENTRALBOARD) $(TEST_PATTERN) $(TEST_ANIMALCARD) $(TEST_ANIMALCARDDECK) $(TEST_NATURESPIRITCARD) $(TEST_NATURESPIRITDECK)

# Prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean test


# Run the application
run: $(TARGET)
	./$(TARGET)

# --- Tests ---

$(TEST_HEXCOORD): $(TEST_DIR)/testHexCoord.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_BOARDCELL): $(TEST_DIR)/testBoardCell.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PERSONALBOARD): $(TEST_DIR)/testPersonalBoard.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TOKENBAG): $(TEST_DIR)/testTokenBag.cpp $(SRC_DIR)/model/TokenBag.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TOKENSLOT): $(TEST_DIR)/testTokenSlot.cpp $(SRC_DIR)/model/TokenSlot.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_CENTRALBOARD): $(TEST_DIR)/testCentralBoard.cpp $(SRC_DIR)/model/CentralBoard.cpp $(SRC_DIR)/model/TokenSlot.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PATTERN): $(TEST_DIR)/testPattern.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_ANIMALCARD): $(TEST_DIR)/testAnimalCard.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_ANIMALCARDDECK): $(TEST_DIR)/testAnimalCardDeck.cpp $(SRC_DIR)/model/AnimalCardDeck.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_NATURESPIRITCARD): $(TEST_DIR)/testNatureSpiritCard.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_NATURESPIRITDECK): $(TEST_DIR)/testNatureSpiritDeck.cpp $(SRC_DIR)/model/NatureSpiritDeck.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TESTS)
	@echo "Running tests..."
	./$(TEST_HEXCOORD)
	./$(TEST_BOARDCELL)
	./$(TEST_PERSONALBOARD)
	./$(TEST_TOKENBAG)
	./$(TEST_TOKENSLOT)
	./$(TEST_CENTRALBOARD)
	./$(TEST_PATTERN)
	./$(TEST_ANIMALCARD)
	./$(TEST_ANIMALCARDDECK)
	./$(TEST_NATURESPIRITCARD)
	./$(TEST_NATURESPIRITDECK)
