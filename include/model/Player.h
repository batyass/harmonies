#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "model/PersonalBoard.h"

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

        public:
            Player(const std::string &playerName, BoardSide side);

            const std::string &getName() const;
            int getScore() const;
            PersonalBoard *getBoard();
            const PersonalBoard *getBoard() const;
            void addPoints(int points);
        };

    }
}

#endif
