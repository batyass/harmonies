#include "core/SlotSelectionService.h"

#include <stdexcept>

namespace harmonies
{
    namespace core
    {
        void resetSoloMarket(model::CentralBoard &board, model::TokenBag &bag)
        {
            if (board.getNbSlots() != 3)
            {
                return;
            }

            for (std::size_t i = 0; i < board.getNbSlots(); ++i)
            {
                model::TokenSlot *slot = board.getSlot(i);
                if (slot == nullptr)
                {
                    continue;
                }

                if (!slot->isEmpty())
                {
                    slot->takeAll(); // discard without returning to bag
                }
                slot->fill(bag.drawTokens(3));
            }
        }

        std::vector<model::TokenType> takeTokensFromSlot(model::CentralBoard &board,
                                                         model::TokenBag &bag,
                                                         std::size_t slotIndex)
        {
            model::TokenSlot *slot = board.getSlot(slotIndex);
            if (slot == nullptr)
            {
                throw std::out_of_range("takeTokensFromSlot: slot index is out of range");
            }

            if (slot->isEmpty())
            {
                throw std::logic_error("takeTokensFromSlot: cannot take tokens from an empty slot");
            }

            std::vector<model::TokenType> tokens = slot->takeAll();

            if (board.getNbSlots() == 3)
            {
                // In solo, the other 6 tokens are discarded only at end of turn.
            }
            else
            {
                slot->fill(bag.drawTokens(3));
            }

            return tokens;
        }
    }
}
