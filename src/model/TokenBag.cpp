#include "model/TokenBag.h"

namespace harmonies {
namespace model {

TokenBag::TokenBag() {
    for (int i = 0; i < 20; ++i) {
        tokens.push_back(new Token(TokenType::BlueWater));
        tokens.push_back(new Token(TokenType::GrayStone));
        tokens.push_back(new Token(TokenType::BrownEarth));
        tokens.push_back(new Token(TokenType::GreenTree));
        tokens.push_back(new Token(TokenType::YellowField));
        tokens.push_back(new Token(TokenType::RedBuilding));
    }
    shuffle();
}

void TokenBag::shuffle() {
    std::srand(std::time(nullptr));
    std::random_shuffle(tokens.begin(), tokens.end());
}

std::vector<Token*> TokenBag::drawTokens(int count) {
    std::vector<Token*> drawn;
    for (int i = 0; i < count && !tokens.empty(); ++i) {
        drawn.push_back(tokens.back());
        tokens.pop_back();
    }
    return drawn;
}

TokenBag::~TokenBag() {
    for (auto t : tokens) delete t;
}

} 
}