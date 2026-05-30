#ifndef CONSOLEINPUTHANDLER_H
#define CONSOLEINPUTHANDLER_H

#include "core/Game.h"
#include "ui/ConsoleRenderer.h"

namespace harmonies {
    namespace ui {

        class ConsoleInputHandler {
        private:
            core::Game& game;
            const ConsoleRenderer& renderer;

        public:
            ConsoleInputHandler(core::Game& gameInstance, const ConsoleRenderer& rendererInstance);

            void processInput();
        };

    }
}

#endif
