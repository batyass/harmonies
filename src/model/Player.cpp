#include "model/Player.h"

namespace harmonies
{
    namespace model
    {

        Player::Player(const std::string &playerName, BoardSide side) : name(playerName), board(side), score(0) {}

        const std::string &Player::getName() const
        {
            return name;
        }

        int Player::getScore() const
        {
            return score;
        }

        PersonalBoard *Player::getBoard()
        {
            return &board;
        }

        const PersonalBoard *Player::getBoard() const
        {
            return &board;
        }

        void Player::addPoints(int points)
        {
            if (points > 0)
            {
                score += points;
            }
        }

    }
}
