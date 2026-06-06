QT += widgets

TEMPLATE = app
TARGET = harmonies_qt

CONFIG += c++11

INCLUDEPATH += include

SOURCES += \
    src/qt/main_qt.cpp \
    src/ui/qt/GameControllerQt.cpp \
    src/ui/qt/MainWindow.cpp \
    src/core/AnimalCubePlacementService.cpp \
    src/core/DefaultDeckFactory.cpp \
    src/core/Game.cpp \
    src/core/NatureSpiritPlacementService.cpp \
    src/core/SlotSelectionService.cpp \
    src/core/TokenPlacementService.cpp \
    src/core/TurnManager.cpp \
    src/model/AnimalCard.cpp \
    src/model/AnimalCardDeck.cpp \
    src/model/BoardCell.cpp \
    src/model/CentralBoard.cpp \
    src/model/GameConfig.cpp \
    src/model/NatureSpiritCard.cpp \
    src/model/NatureSpiritDeck.cpp \
    src/model/Pattern.cpp \
    src/model/PersonalBoard.cpp \
    src/model/Player.cpp \
    src/model/PlayerCardCollection.cpp \
    src/model/ScoreReport.cpp \
    src/model/TokenBag.cpp \
    src/model/TokenSlot.cpp \
    src/rules/EndGameChecker.cpp \
    src/rules/PatternMatcher.cpp \
    src/rules/PlacementValidator.cpp \
    src/rules/StackRule.cpp \
    src/scoring/AnimalCardScoreCalculator.cpp \
    src/scoring/LandscapeScoreCalculator.cpp \
    src/scoring/NatureSpiritScoreCalculator.cpp \
    src/scoring/ScoreCalculator.cpp \
    src/scoring/SoloScoreEvaluator.cpp \
    src/ui/qt/SetupWindow.cpp \
    src/utils/hexCoord.cpp

HEADERS += \
    include/ui/qt/GameControllerQt.h \
    include/ui/qt/MainWindow.h \
    include/ui/qt/SetupWindow.h

# Version Qt Widgets minimale :
# - garde tout le moteur existant
# - n'inclut pas l'UI console
# - sert de base pour brancher progressivement les widgets de jeu

