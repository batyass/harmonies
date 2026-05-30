#include "core/NatureSpiritPlacementService.h"
#include "rules/PatternMatcher.h"

namespace harmonies {
    namespace core {

        bool placeNatureSpiritCube(model::NatureSpiritCard& card,
                                model::PersonalBoard& board,
                                const utils::HexCoord& anchor) {
            if (card.isCubePlaced())
                return false;

            if (!rules::matchesAnyRotation(board, anchor, card.getPattern()))
                return false;

            model::BoardCell* cell = board.getCell(anchor);
            if (!cell || !cell->canPlaceCube())
                return false;

            card.placeCube();
            cell->placeCube();
            return true;
        }

    } // namespace core
} // namespace harmonies
