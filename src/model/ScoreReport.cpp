#include "model/ScoreReport.h"

namespace harmonies
{
    namespace model
    {
        void ScoreReport::addTreeScore(std::size_t s)
        {
            treeScore += s;
        }

        void ScoreReport::addMountainScore(std::size_t s)
        {
            mountainScore += s;
        }

        void ScoreReport::addFieldScore(std::size_t s)
        {
            fieldScore += s;
        }

        void ScoreReport::addBuildingScore(std::size_t s)
        {
            buildingScore += s;
        }

        void ScoreReport::addWaterScore(std::size_t s)
        {
            waterScore += s;
        }

        void ScoreReport::addAnimalsScore(std::size_t s)
        {
            animalsScore += s;
        }

        void ScoreReport::addSpiritScore(std::size_t s)
        {
            spiritScore += s;
        }

        std::size_t ScoreReport::getTotalScore() const
        {
            return treeScore + mountainScore + fieldScore + buildingScore + waterScore + animalsScore + spiritScore;
        }

        void ScoreReport::resetScore()
        {
            treeScore = 0;
            mountainScore = 0;
            fieldScore = 0;
            buildingScore = 0;
            waterScore = 0;
            animalsScore = 0;
            spiritScore = 0;
        }
    }
}
