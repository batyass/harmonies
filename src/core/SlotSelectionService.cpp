#include "core/SlotSelectionService.h"

#include <stdexcept>

namespace harmonies
{
    namespace core
    {
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
                // Solo mode: discard unchosen slots, then refill all slots
                for (std::size_t i = 0; i < board.getNbSlots(); ++i)
                {
                    model::TokenSlot *s = board.getSlot(i);
                    if (s && !s->isEmpty())
                        s->takeAll(); // discard without returning to bag
                    s->fill(bag.drawTokens(3));
                }
            }
            else
            {
                slot->fill(bag.drawTokens(3));
            }

            return tokens;
        }
    }
}
