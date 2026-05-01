#pragma once

/*Contient :
score arbres
score montagnes
score champs
score bâtiments
score eau
score animaux
score esprit
total
*/

#include <cstddef>

namespace harmonies
{
    namespace model
    {
        class ScoreReport
        {
        private:
            std::size_t treeScore;
            std::size_t mountainScore;
            std::size_t fieldScore;
            std::size_t buildingScore;
            std::size_t waterScore;
            std::size_t animalsScore;
            std::size_t spiritScore;

        public:
            ScoreReport() : treeScore(0), mountainScore(0), fieldScore(0), buildingScore(0), waterScore(0), animalsScore(0), spiritScore(0) {}
            std::size_t getTreeScore() const { return treeScore; }
            std::size_t getMountainScore() const { return mountainScore; }
            std::size_t getFieldScore() const { return fieldScore; }
            std::size_t getBuildingScore() const { return buildingScore; }
            std::size_t getWaterScore() const { return waterScore; }
            std::size_t getAnimalsScore() const { return animalsScore; }
            std::size_t getSpiritScore() const { return spiritScore; }

            void addTreeScore(std::size_t s);
            void addMountainScore(std::size_t s);
            void addFieldScore(std::size_t s);
            void addBuildingScore(std::size_t s);
            void addWaterScore(std::size_t s);
            void addAnimalsScore(std::size_t s);
            void addSpiritScore(std::size_t s);
            std::size_t getTotalScore() const;
            void resetScore();
        };
    }
}
