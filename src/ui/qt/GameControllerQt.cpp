#include "ui/qt/GameControllerQt.h"

#include <exception>
#include <vector>

namespace harmonies
{
    namespace ui
    {
        GameControllerQt::GameControllerQt(QObject *parent)
            : QObject(parent)
        {
        }

        core::Game *GameControllerQt::getGame() const
        {
            return game.get();
        }

        void GameControllerQt::startDemoGame()
        {
            try
            {
                model::GameConfig config(1, model::BoardSide::A, false);
                std::vector<std::string> playerNames;
                playerNames.push_back("Joueur 1");

                game.reset(new core::Game(config, playerNames));
                game->initGame();

                emit gameStateChanged();
            }
            catch (const std::exception &e)
            {
                emit actionFailed(QString::fromUtf8(e.what()));
            }
        }
    }
}
