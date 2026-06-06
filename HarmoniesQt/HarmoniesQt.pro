QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 1. Prevent Qt from hijacking the word 'slots' globally, protecting backend models
DEFINES += QT_NO_KEYWORDS

# 2. Standard search paths for headers
INCLUDEPATH += $$PWD/../include

# 3. Clean list of production source files (With DefaultDeckFactory.cpp added!)
SOURCES += main.cpp \
           mainwindow.cpp \
           setupdialog.cpp \
           cardmarketwidget.cpp \
           centralboardwidget.cpp \
           gamestagewidget.cpp \
           personalboardwidget.cpp \
           playerinfoswidget.cpp \
           $$PWD/../src/core/Game.cpp \
           $$PWD/../src/core/TurnManager.cpp \
           $$PWD/../src/core/DefaultDeckFactory.cpp \
           $$PWD/../src/core/SlotSelectionService.cpp \
           $$PWD/../src/core/TokenPlacementService.cpp \
           $$PWD/../src/core/AnimalCubePlacementService.cpp \
           $$PWD/../src/core/NatureSpiritPlacementService.cpp \
           $$PWD/../src/model/AnimalCard.cpp \
           $$PWD/../src/model/AnimalCardDeck.cpp \
           $$PWD/../src/model/BoardCell.cpp \
           $$PWD/../src/model/CentralBoard.cpp \
           $$PWD/../src/model/GameConfig.cpp \
           $$PWD/../src/model/NatureSpiritCard.cpp \
           $$PWD/../src/model/NatureSpiritDeck.cpp \
           $$PWD/../src/model/Pattern.cpp \
           $$PWD/../src/model/PersonalBoard.cpp \
           $$PWD/../src/model/Player.cpp \
           $$PWD/../src/model/PlayerCardCollection.cpp \
           $$PWD/../src/model/ScoreReport.cpp \
           $$PWD/../src/model/TokenBag.cpp \
           $$PWD/../src/model/TokenSlot.cpp \
           $$PWD/../src/rules/EndGameChecker.cpp \
           $$PWD/../src/rules/PatternMatcher.cpp \
           $$PWD/../src/rules/PlacementValidator.cpp \
           $$PWD/../src/rules/StackRule.cpp \
           $$PWD/../src/scoring/AnimalCardScoreCalculator.cpp \
           $$PWD/../src/scoring/LandscapeScoreCalculator.cpp \
           $$PWD/../src/scoring/NatureSpiritScoreCalculator.cpp \
           $$PWD/../src/scoring/ScoreCalculator.cpp \
           $$PWD/../src/scoring/SoloScoreEvaluator.cpp \
           $$PWD/../src/utils/hexCoord.cpp

# 4. Target UI Headers layout view
HEADERS += mainwindow.h \
           setupdialog.h \
           cardmarketwidget.h \
           centralboardwidget.h \
           gamestagewidget.h \
           personalboardwidget.h \
           playerinfoswidget.h
