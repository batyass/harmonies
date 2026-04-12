#ifndef TOKENBAG_H
#define TOKENBAG_H

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "model/Token.h"

namespace harmonies {
namespace model {

class TokenBag {
private:
    std::vector<Token*> tokens;

public:
    TokenBag();
    ~TokenBag();
    void shuffle();
    std::vector<Token*> drawTokens(int count);
    bool isEmpty() const { return tokens.empty(); }
    int getRemainingCount() const { return tokens.size(); }
};

} 
} 

#endif