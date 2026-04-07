#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"

namespace harmonies {
namespace model {

class Token {
private:
    TokenType type;
public:
    Token(TokenType t) : type(t) {}
    TokenType getType() const { return type; }
};

} // namespace model
} // namespace harmonies

#endif