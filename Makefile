# --- Compiler Settings ---
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# --- Target and Folders ---
TARGET = prog
SRC_DIR = src
TEST_DIR = tests

# --- Source Discovery ---
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/model/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp) \
       $(wildcard $(SRC_DIR)/core/*.cpp) \
       $(wildcard $(SRC_DIR)/rules/*.cpp) \
       $(wildcard $(SRC_DIR)/scoring/*.cpp) \
       $(wildcard $(SRC_DIR)/ui/*.cpp)

OBJS = $(SRCS:.cpp=.o)

TEST_HEXCOORD = $(TEST_DIR)/testHexCoord
TEST_BOARDCELL = $(TEST_DIR)/testBoardCell
TEST_PERSONALBOARD = $(TEST_DIR)/testPersonalBoard
TEST_TOKENBAG = $(TEST_DIR)/testTokenBag
TEST_TOKENSLOT = $(TEST_DIR)/testTokenSlot
TEST_CENTRALBOARD = $(TEST_DIR)/testCentralBoard
TEST_PATTERN = $(TEST_DIR)/testPattern
TEST_ANIMALCARD = $(TEST_DIR)/testAnimalCard
TEST_ANIMALCARDSCORECALCULATOR = $(TEST_DIR)/testAnimalCardScoreCalculator
TEST_ANIMALCARDDECK = $(TEST_DIR)/testAnimalCardDeck
TEST_NATURESPIRITCARD = $(TEST_DIR)/testNatureSpiritCard
TEST_NATURESPIRITDECK = $(TEST_DIR)/testNatureSpiritDeck
TEST_NATURESPIRITSCORECALCULATOR = $(TEST_DIR)/testNatureSpiritScoreCalculator
TEST_PLAYER = $(TEST_DIR)/testPlayer
TEST_PLAYERCARDCOLLECTION = $(TEST_DIR)/testPlayerCardCollection
TEST_GAMECONFIG = $(TEST_DIR)/testGameConfig
TEST_SCOREREPORT = $(TEST_DIR)/testScoreReport
TEST_PLACEMENTVALIDATOR = $(TEST_DIR)/testPlacementValidator
TEST_STACKRULE = $(TEST_DIR)/testStackRule
TEST_LANDSCAPESCORECALCULATOR = $(TEST_DIR)/testLandscapeScoreCalculator
TEST_ENDGAMECHECKER = $(TEST_DIR)/testEndGameChecker
TEST_PATTERNMATCHER = $(TEST_DIR)/testPatternMatcher
TEST_TURNMANAGER = $(TEST_DIR)/testTurnManager
TEST_ANIMALCUBEPLACEMENTSERVICE = $(TEST_DIR)/testAnimalCubePlacementService
TEST_NATURESPIRITPLACEMENTSERVICE = $(TEST_DIR)/testNatureSpiritPlacementService
TEST_SLOTSELECTIONSERVICE = $(TEST_DIR)/testSlotSelectionService
TEST_TOKENPLACEMENTSERVICE = $(TEST_DIR)/testTokenPlacementService
TEST_GAME = $(TEST_DIR)/testGame
TEST_CONSOLERENDERER = $(TEST_DIR)/testConsoleRenderer
TEST_SETUPMENU = $(TEST_DIR)/testSetupMenu
TEST_SCORECALCULATOR = $(TEST_DIR)/testScoreCalculator
TEST_SOLOSCOREEVALUATOR = $(TEST_DIR)/testSoloScoreEvaluator
TEST_SOLORULES = $(TEST_DIR)/testSoloRules

TESTS = $(TEST_HEXCOORD) $(TEST_BOARDCELL) $(TEST_PERSONALBOARD) $(TEST_TOKENBAG) $(TEST_TOKENSLOT) $(TEST_CENTRALBOARD) $(TEST_PATTERN) $(TEST_ANIMALCARD) $(TEST_ANIMALCARDSCORECALCULATOR) $(TEST_ANIMALCARDDECK) $(TEST_NATURESPIRITCARD) $(TEST_NATURESPIRITDECK) $(TEST_NATURESPIRITSCORECALCULATOR) $(TEST_PLAYER) $(TEST_PLAYERCARDCOLLECTION) $(TEST_GAMECONFIG) $(TEST_SCOREREPORT) $(TEST_PLACEMENTVALIDATOR) $(TEST_STACKRULE) $(TEST_LANDSCAPESCORECALCULATOR) $(TEST_ENDGAMECHECKER) $(TEST_PATTERNMATCHER) $(TEST_TURNMANAGER) $(TEST_ANIMALCUBEPLACEMENTSERVICE) $(TEST_NATURESPIRITPLACEMENTSERVICE) $(TEST_SLOTSELECTIONSERVICE) $(TEST_TOKENPLACEMENTSERVICE) $(TEST_GAME) $(TEST_CONSOLERENDERER) $(TEST_SCORECALCULATOR) $(TEST_SOLOSCOREEVALUATOR) $(TEST_SOLORULES)

# --- Build Rules ---
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Utility Rules ---
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(TARGET)
	rm -f $(TESTS) $(TEST_SETUPMENU)

.PHONY: all clean run test test-setup

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

$(TEST_ANIMALCARDSCORECALCULATOR): $(TEST_DIR)/testAnimalCardScoreCalculator.cpp $(SRC_DIR)/scoring/AnimalCardScoreCalculator.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
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

$(TEST_NATURESPIRITSCORECALCULATOR): $(TEST_DIR)/testNatureSpiritScoreCalculator.cpp $(SRC_DIR)/scoring/NatureSpiritScoreCalculator.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PLAYER): $(TEST_DIR)/testPlayer.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PLAYERCARDCOLLECTION): $(TEST_DIR)/testPlayerCardCollection.cpp $(SRC_DIR)/model/PlayerCardCollection.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_GAMECONFIG): $(TEST_DIR)/testGameConfig.cpp $(SRC_DIR)/model/GameConfig.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SCOREREPORT): $(TEST_DIR)/testScoreReport.cpp $(SRC_DIR)/model/ScoreReport.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PLACEMENTVALIDATOR): $(TEST_DIR)/testPlacementValidator.cpp $(SRC_DIR)/rules/PlacementValidator.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_STACKRULE): $(TEST_DIR)/testStackRule.cpp $(SRC_DIR)/rules/StackRule.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_LANDSCAPESCORECALCULATOR): $(TEST_DIR)/testLandscapeScoreCalculator.cpp $(SRC_DIR)/scoring/LandscapeScoreCalculator.cpp $(SRC_DIR)/rules/StackRule.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_ENDGAMECHECKER): $(TEST_DIR)/testEndGameChecker.cpp $(SRC_DIR)/rules/EndGameChecker.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/model/TokenBag.cpp $(SRC_DIR)/utils/hexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_PATTERNMATCHER): $(TEST_DIR)/testPatternMatcher.cpp $(SRC_DIR)/rules/PatternMatcher.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/utils/hexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TURNMANAGER): $(TEST_DIR)/testTurnManager.cpp $(SRC_DIR)/core/TurnManager.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_ANIMALCUBEPLACEMENTSERVICE): $(TEST_DIR)/testAnimalCubePlacementService.cpp $(SRC_DIR)/core/AnimalCubePlacementService.cpp $(SRC_DIR)/rules/PatternMatcher.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/hexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_NATURESPIRITPLACEMENTSERVICE): $(TEST_DIR)/testNatureSpiritPlacementService.cpp $(SRC_DIR)/core/NatureSpiritPlacementService.cpp $(SRC_DIR)/rules/PatternMatcher.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/hexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SLOTSELECTIONSERVICE): $(TEST_DIR)/testSlotSelectionService.cpp $(SRC_DIR)/core/SlotSelectionService.cpp $(SRC_DIR)/model/CentralBoard.cpp $(SRC_DIR)/model/TokenSlot.cpp $(SRC_DIR)/model/TokenBag.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TOKENPLACEMENTSERVICE): $(TEST_DIR)/testTokenPlacementService.cpp $(SRC_DIR)/core/TokenPlacementService.cpp $(SRC_DIR)/rules/PlacementValidator.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_GAME): $(TEST_DIR)/testGame.cpp $(SRC_DIR)/core/Game.cpp $(SRC_DIR)/core/DefaultDeckFactory.cpp $(SRC_DIR)/core/TurnManager.cpp $(SRC_DIR)/core/SlotSelectionService.cpp $(SRC_DIR)/core/TokenPlacementService.cpp $(SRC_DIR)/core/AnimalCubePlacementService.cpp $(SRC_DIR)/core/NatureSpiritPlacementService.cpp $(SRC_DIR)/rules/EndGameChecker.cpp $(SRC_DIR)/rules/PlacementValidator.cpp $(SRC_DIR)/rules/PatternMatcher.cpp $(SRC_DIR)/rules/StackRule.cpp $(SRC_DIR)/scoring/LandscapeScoreCalculator.cpp $(SRC_DIR)/scoring/AnimalCardScoreCalculator.cpp $(SRC_DIR)/scoring/NatureSpiritScoreCalculator.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PlayerCardCollection.cpp $(SRC_DIR)/model/AnimalCardDeck.cpp $(SRC_DIR)/model/NatureSpiritDeck.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/model/GameConfig.cpp $(SRC_DIR)/model/TokenBag.cpp $(SRC_DIR)/model/CentralBoard.cpp $(SRC_DIR)/model/TokenSlot.cpp $(SRC_DIR)/model/ScoreReport.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SCORECALCULATOR): $(TEST_DIR)/testScoreCalculator.cpp $(SRC_DIR)/scoring/ScoreCalculator.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/PlayerCardCollection.cpp $(SRC_DIR)/scoring/LandscapeScoreCalculator.cpp $(SRC_DIR)/scoring/AnimalCardScoreCalculator.cpp $(SRC_DIR)/scoring/NatureSpiritScoreCalculator.cpp $(SRC_DIR)/rules/StackRule.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/utils/HexCoord.cpp $(SRC_DIR)/model/ScoreReport.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SOLOSCOREEVALUATOR): $(TEST_DIR)/testSoloScoreEvaluator.cpp $(SRC_DIR)/scoring/SoloScoreEvaluator.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SOLORULES): $(TEST_DIR)/testSoloRules.cpp $(SRC_DIR)/core/Game.cpp $(SRC_DIR)/core/DefaultDeckFactory.cpp $(SRC_DIR)/core/TurnManager.cpp $(SRC_DIR)/core/SlotSelectionService.cpp $(SRC_DIR)/core/TokenPlacementService.cpp $(SRC_DIR)/core/AnimalCubePlacementService.cpp $(SRC_DIR)/core/NatureSpiritPlacementService.cpp $(SRC_DIR)/rules/EndGameChecker.cpp $(SRC_DIR)/rules/PlacementValidator.cpp $(SRC_DIR)/rules/PatternMatcher.cpp $(SRC_DIR)/rules/StackRule.cpp $(SRC_DIR)/scoring/LandscapeScoreCalculator.cpp $(SRC_DIR)/scoring/AnimalCardScoreCalculator.cpp $(SRC_DIR)/scoring/NatureSpiritScoreCalculator.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/PlayerCardCollection.cpp $(SRC_DIR)/model/AnimalCardDeck.cpp $(SRC_DIR)/model/NatureSpiritDeck.cpp $(SRC_DIR)/model/AnimalCard.cpp $(SRC_DIR)/model/NatureSpiritCard.cpp $(SRC_DIR)/model/Pattern.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/model/GameConfig.cpp $(SRC_DIR)/model/TokenBag.cpp $(SRC_DIR)/model/CentralBoard.cpp $(SRC_DIR)/model/TokenSlot.cpp $(SRC_DIR)/model/ScoreReport.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_CONSOLERENDERER): $(TEST_DIR)/testConsoleRenderer.cpp $(SRC_DIR)/ui/ConsoleRenderer.cpp $(SRC_DIR)/model/CentralBoard.cpp $(SRC_DIR)/model/TokenSlot.cpp $(SRC_DIR)/model/PersonalBoard.cpp $(SRC_DIR)/model/BoardCell.cpp $(SRC_DIR)/model/Player.cpp $(SRC_DIR)/model/ScoreReport.cpp $(SRC_DIR)/utils/HexCoord.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_SETUPMENU): $(TEST_DIR)/testSetupMenu.cpp $(SRC_DIR)/ui/SetupMenu.cpp $(SRC_DIR)/model/GameConfig.cpp
	@echo "Building $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

test-setup: $(TEST_SETUPMENU)
	./$(TEST_SETUPMENU)

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
	./$(TEST_ANIMALCARDSCORECALCULATOR)
	./$(TEST_ANIMALCARDDECK)
	./$(TEST_NATURESPIRITCARD)
	./$(TEST_NATURESPIRITDECK)
	./$(TEST_NATURESPIRITSCORECALCULATOR)
	./$(TEST_PLAYER)
	./$(TEST_PLAYERCARDCOLLECTION)
	./$(TEST_GAMECONFIG)
	./$(TEST_SCOREREPORT)
	./$(TEST_PLACEMENTVALIDATOR)
	./$(TEST_STACKRULE)
	./$(TEST_LANDSCAPESCORECALCULATOR)
	./$(TEST_ENDGAMECHECKER)
	./$(TEST_PATTERNMATCHER)
	./$(TEST_TURNMANAGER)
	./$(TEST_ANIMALCUBEPLACEMENTSERVICE)
	./$(TEST_NATURESPIRITPLACEMENTSERVICE)
	./$(TEST_SLOTSELECTIONSERVICE)
	./$(TEST_TOKENPLACEMENTSERVICE)
	./$(TEST_GAME)
	./$(TEST_CONSOLERENDERER)
	./$(TEST_SCORECALCULATOR)
	./$(TEST_SOLOSCOREEVALUATOR)
	./$(TEST_SOLORULES)
