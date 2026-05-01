#include "model/Player.h"

namespace harmonies
{
    namespace model
    {

        Player::Player(const std::string &playerName, BoardSide side) : name(playerName), score(0)
        {
            board = new PersonalBoard(side);
        }

        Player::~Player()
        {
            delete board;
        }

        std::string Player::getName() const
        {
            return name;
        }

        int Player::getScore() const
        {
            return score;
        }

        PersonalBoard *Player::getBoard()
        {
            return board;
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