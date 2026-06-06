#ifndef GAMECONTROLLERQT_H
#define GAMECONTROLLERQT_H

#include <memory>

#include <QObject>
#include <QString>

#include "core/Game.h"

namespace harmonies
{
    namespace ui
    {
        class GameControllerQt : public QObject
        {
            Q_OBJECT

        private:
            std::unique_ptr<core::Game> game;

        public:
            explicit GameControllerQt(QObject *parent = 0);

            core::Game *getGame() const;

        public slots:
            void startDemoGame();

        signals:
            void gameStateChanged();
            void actionFailed(const QString &message);
        };
    }
}

#endif
