#include "model/TokenBag.h"

#include <algorithm>
#include <random>

namespace harmonies
{
    namespace model
    {

        TokenBag::TokenBag()
        {
            for (int i = 0; i < 20; ++i)
            {
                tokens.push_back(TokenType::BlueWater);
                tokens.push_back(TokenType::GrayStone);
                tokens.push_back(TokenType::BrownEarth);
                tokens.push_back(TokenType::GreenTree);
                tokens.push_back(TokenType::YellowField);
                tokens.push_back(TokenType::RedBuilding);
            }
            shuffle();
        }

        void TokenBag::shuffle()
        {
            std::mt19937 randomEngine(std::random_device{}());
            std::shuffle(tokens.begin(), tokens.end(), randomEngine);
        }

        std::vector<TokenType> TokenBag::drawTokens(std::size_t count)
        {
            std::vector<TokenType> drawn;

            for (std::size_t i = 0; i < count && !tokens.empty(); ++i)
            {
                drawn.push_back(tokens.back());
                tokens.pop_back();
            }

            return drawn;
        }

        bool TokenBag::isEmpty() const
        {
            return tokens.empty();
        }

        std::size_t TokenBag::getRemainingCount() const
        {
            return tokens.size();
        }

    }
}
