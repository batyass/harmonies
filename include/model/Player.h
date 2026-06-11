#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"
#include "model/PlayerCardCollection.h"

#include "model/PlayerCardCollection.h"
#include "model/NatureSpiritCard.h"

namespace harmonies
{
    namespace model
    {

        class Player
        {
        private:
            std::string name;
            PersonalBoard board;
            int score;
            PlayerCardCollection animalCards;
            std::vector<NatureSpiritCard> natureSpiritCards;

            // Added to bridge the gap for ScoreCalculator to access card assets
            // without altering existing constructors or breaking dependencies.
            PlayerCardCollection activeAnimalCards;    // cartes actives
            PlayerCardCollection completedAnimalCards; // cartes complétées
            const NatureSpiritCard* natureSpiritCard = nullptr; // carte Esprit

        public:
            Player(const std::string &playerName, BoardSide side);

            const std::string &getName() const;
            int getScore() const;
            PersonalBoard *getBoard();
            const PersonalBoard *getBoard() const;
            PlayerCardCollection *getAnimalCards();
            const PlayerCardCollection *getAnimalCards() const;
            void addNatureSpiritCard(const NatureSpiritCard &card);
            std::size_t getNatureSpiritCardCount() const;
            std::size_t getActiveAnimalCardCount() const;
            bool hasUnplacedChosenNatureSpiritCard() const;
            NatureSpiritCard *getNatureSpiritCard(std::size_t index);
            const std::vector<NatureSpiritCard> &getNatureSpiritCards() const;
            bool chooseNatureSpiritCard(std::size_t index);
            void addPoints(int points);

            // Accessors for scoring module implementation
            const PlayerCardCollection& getActiveAnimalCards() const { return activeAnimalCards; }
            const PlayerCardCollection& getCompletedAnimalCards() const { return completedAnimalCards; }
            const NatureSpiritCard* getNatureSpiritCard() const { return natureSpiritCard; }
            void setScore(int points);
        };

    }
}

#endif
