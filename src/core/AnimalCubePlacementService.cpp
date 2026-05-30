#include "core/AnimalCubePlacementService.h"
#include "rules/PatternMatcher.h"

namespace harmonies {
    namespace core {

        bool placeAnimalCube(model::AnimalCard& card,
                            model::PersonalBoard& board,
                            const utils::HexCoord& anchor) {
            if (card.isComplete())
                return false;

            if (!rules::matchesAnyRotation(board, anchor, card.getPattern()))
                return false;

            model::BoardCell* cell = board.getCell(anchor);
            if (!cell || !cell->canPlaceCube())
                return false;

            card.placeNextCube();
            cell->placeCube();
            return true;
    }

    } // namespace core
} // namespace harmonies
