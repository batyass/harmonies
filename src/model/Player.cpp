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

        PlayerCardCollection *Player::getAnimalCards()
        {
            return &animalCards;
        }

        const PlayerCardCollection *Player::getAnimalCards() const
        {
            return &animalCards;
        }

        void Player::addNatureSpiritCard(const NatureSpiritCard &card)
        {
            natureSpiritCards.push_back(card);
        }

        std::size_t Player::getNatureSpiritCardCount() const
        {
            return natureSpiritCards.size();
        }

        NatureSpiritCard *Player::getNatureSpiritCard(std::size_t index)
        {
            if (index >= natureSpiritCards.size())
            {
                return nullptr;
            }

            return &natureSpiritCards[index];
        }

        const std::vector<NatureSpiritCard> &Player::getNatureSpiritCards() const
        {
            return natureSpiritCards;
        }

        bool Player::chooseNatureSpiritCard(std::size_t index)
        {
            if (index >= natureSpiritCards.size())
            {
                return false;
            }

            NatureSpiritCard chosenCard = natureSpiritCards[index];
            natureSpiritCards.clear();
            natureSpiritCards.push_back(chosenCard);
            return true;
        }

        void Player::addPoints(int points)
        {
            if (points > 0)
            {
                score += points;
            }
        }

        void Player::setScore(int points)
        {
            score = points < 0 ? 0 : points;
        }

    }
}
