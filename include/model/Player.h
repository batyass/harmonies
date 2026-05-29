#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "model/PersonalBoard.h"

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
            void addPoints(int points);

            // Accessors for scoring module implementation
            const PlayerCardCollection& getActiveAnimalCards() const { return activeAnimalCards; }
            const PlayerCardCollection& getCompletedAnimalCards() const { return completedAnimalCards; }
            const NatureSpiritCard* getNatureSpiritCard() const { return natureSpiritCard; }
        };

    }
}

#endif
