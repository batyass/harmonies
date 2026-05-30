#include "scoring/ScoreCalculator.h"
#include "scoring/LandscapeScoreCalculator.h"
#include "scoring/AnimalCardScoreCalculator.h"
#include "scoring/NatureSpiritScoreCalculator.h"

namespace harmonies
{
    namespace scoring
    {
        model::ScoreReport calculateTotalScore(const model::Player& player)
        {
            model::ScoreReport report;
            const auto* board = player.getBoard();
            
            if (board == nullptr)
            {
                return report;
            }

            std::size_t landscapeScore = LandscapeScoreCalculator(*board);
            report.addTreeScore(landscapeScore); 

            std::size_t totalAnimalScore = 0;
            const auto& completedCards = player.getCompletedAnimalCards().getCards();
            for (std::size_t i = 0; i < completedCards.size(); ++i)
            {
                totalAnimalScore += AnimalCardScoreCalculator(completedCards[i]);
            }
            report.addAnimalsScore(totalAnimalScore);

            const auto* spirit = player.getNatureSpiritCard();
            if (spirit != nullptr)
            {
                std::size_t spiritScore = natureSpiritScoreCalculator(*spirit, *board);
                report.addSpiritScore(spiritScore);
            }

            return report;
        }
    }
}