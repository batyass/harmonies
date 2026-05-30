#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"
#include "model/PlayerCardCollection.h"

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
            NatureSpiritCard *getNatureSpiritCard(std::size_t index);
            const std::vector<NatureSpiritCard> &getNatureSpiritCards() const;
            bool chooseNatureSpiritCard(std::size_t index);
            void addPoints(int points);
            void setScore(int points);
        };

    }
}

#endif
