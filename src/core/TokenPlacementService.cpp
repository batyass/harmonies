#include "core/TokenPlacementService.h"

#include "rules/PlacementValidator.h"

namespace harmonies
{
    namespace core
    {
        bool placeTokenOnBoard(model::PersonalBoard &board, const utils::HexCoord &coord, model::TokenType token)
        {
            // Keep the service atomic: if the move is not valid, the board is left untouched.
            if (!rules::placementValidator(token, coord, board))
            {
                return false;
            }

            return board.placeToken(coord, token);
        }
    }
}
