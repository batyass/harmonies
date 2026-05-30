#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include <string>
#include <vector>
#include "model/CentralBoard.h"
#include "model/PersonalBoard.h"
#include "model/Player.h"
#include "model/ScoreReport.h"
#include "model/TokenType.h"

namespace harmonies
{
    namespace ui
    {

        class ConsoleRenderer
        {
        public:
            void displayCentralBoard(const model::CentralBoard &board) const;
            void displayPersonalBoard(const model::PersonalBoard &board) const;
            void displayScoreReport(const model::ScoreReport &report, const std::string &playerName) const;
            void displayEndGame(const std::vector<model::Player *> &players) const;

        private:
            static char tokenToChar(model::TokenType type);
        };

    } // namespace ui
} // namespace harmonies

#endif
