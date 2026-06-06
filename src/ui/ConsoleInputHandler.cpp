#include "ui/ConsoleInputHandler.h"
#include "utils/hexCoord.h"
#include "model/TokenType.h"

#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace harmonies {
    namespace ui {
        namespace
        {
            char tokenToChar(model::TokenType type)
            {
                switch (type)
                {
                case model::TokenType::BlueWater: return 'W';
                case model::TokenType::GrayStone: return 'S';
                case model::TokenType::BrownEarth: return 'E';
                case model::TokenType::GreenTree: return 'T';
                case model::TokenType::YellowField: return 'F';
                case model::TokenType::RedBuilding: return 'B';
                default: return '?';
                }
            }

            bool readInt(const std::string &prompt, int &value)
            {
                std::cout << prompt;
                if (std::cin >> value)
                {
                    return true;
                }

                if (std::cin.eof())
                {
                    return false;
                }

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entree invalide, veuillez entrer un nombre.\n";
                return readInt(prompt, value);
            }

            void displayVisibleAnimalCards(const core::Game &game)
            {
                const std::vector<model::AnimalCard> &visibleCards = game.getAnimalCardDeck()->getVisible();
                std::cout << "Cartes animales visibles :\n";
                for (std::size_t i = 0; i < visibleCards.size(); ++i)
                {
                    std::cout << "  [" << i << "] " << visibleCards[i].getName()
                              << " (cubes restants: " << visibleCards[i].getCubesOnCard() << ")\n";
                }
            }

            void displayOwnedAnimalCards(const model::Player &player)
            {
                const std::vector<model::AnimalCard> &cards = player.getAnimalCards()->getCards();
                std::cout << "Vos cartes animales :\n";
                if (cards.empty())
                {
                    std::cout << "  (aucune)\n";
                    return;
                }

                for (std::size_t i = 0; i < cards.size(); ++i)
                {
                    std::cout << "  [" << i << "] " << cards[i].getName()
                              << " (cubes restants: " << cards[i].getCubesOnCard() << ")\n";
                }
            }

            void displayNatureSpiritCards(const model::Player &player)
            {
                const std::vector<model::NatureSpiritCard> &cards = player.getNatureSpiritCards();
                std::cout << "Vos cartes esprit de la nature :\n";
                if (cards.empty())
                {
                    std::cout << "  (aucune)\n";
                    return;
                }

                for (std::size_t i = 0; i < cards.size(); ++i)
                {
                    std::cout << "  [" << i << "] " << cards[i].getName()
                              << " (cube " << (cards[i].isCubePlaced() ? "deja pose" : "encore sur la carte") << ")\n";
                }
            }

            void displayPendingTokens(const core::Game &game)
            {
                const std::vector<model::TokenType> &pendingTokens = game.getPendingTokens();
                std::cout << "Jetons a poser :\n";
                for (std::size_t i = 0; i < pendingTokens.size(); ++i)
                {
                    std::cout << "  [" << i << "] " << tokenToChar(pendingTokens[i]) << '\n';
                }
            }

            void displayTurnHeader(const core::Game &game)
            {
                const model::Player *player = game.getCurrentPlayer();
                if (player == nullptr)
                {
                    return;
                }

                std::cout << "\n========================================\n";
                std::cout << "Tour " << game.getTurnManager()->getTurnCount()
                          << " - Joueur courant : " << player->getName() << '\n';
                if (game.isFinalRoundTriggered())
                {
                    std::cout << "Dernier tour : la partie se terminera a la fin de cette ronde.\n";
                }
                std::cout << "========================================\n";
            }
        }

        ConsoleInputHandler::ConsoleInputHandler(core::Game& gameInstance, const ConsoleRenderer& rendererInstance)
            : game(gameInstance), renderer(rendererInstance) {}

        void ConsoleInputHandler::processInput() {
            core::GameState state = game.getState();

            if (state == core::GameState::WaitingForSlotChoice) {
                while (true) {
                    displayTurnHeader(game);
                    renderer.displayCentralBoard(*game.getCentralBoard());
                    renderer.displayPersonalBoard(*game.getCurrentPlayer()->getBoard());
                    displayVisibleAnimalCards(game);
                    displayOwnedAnimalCards(*game.getCurrentPlayer());
                    if (game.isNatureSpiritEnabled())
                    {
                        displayNatureSpiritCards(*game.getCurrentPlayer());
                    }

                    std::cout << "Actions disponibles :\n";
                    std::cout << "  1. Prendre un slot du marche\n";
                    std::cout << "  2. Prendre une carte animale visible\n";
                    if (game.isNatureSpiritEnabled())
                    {
                        std::cout << "  3. Choisir votre carte esprit de la nature\n";
                        std::cout << "  4. Poser un cube animal\n";
                        std::cout << "  5. Poser un cube esprit de la nature\n";
                    }
                    else
                    {
                        std::cout << "  3. Poser un cube animal\n";
                    }

                    int choice;
                    if (!readInt("Votre choix : ", choice))
                    {
                        std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                        return;
                    }

                    try
                    {
                        if (choice == 1)
                        {
                            int slotIndexRaw;
                            if (!readInt("Choisissez un slot du marche : ", slotIndexRaw))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (slotIndexRaw < 0)
                            {
                                std::cout << "Valeur negative invalide, veuillez entrer un nombre positif.\n";
                                continue;
                            }
                            if (game.takeTokensFromSlot(static_cast<std::size_t>(slotIndexRaw))) {
                                return;
                            }
                            std::cout << "Choix refuse par le jeu.\n";
                        }
                        else if (choice == 2)
                        {
                            int cardIndex;
                            if (!readInt("Index de la carte animale visible : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.takeVisibleAnimalCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte animale prise.\n";
                                return;
                            }
                            std::cout << "Prise de carte refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 3)
                        {
                            int cardIndex;
                            if (!readInt("Index de la carte esprit a conserver : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.chooseNatureSpiritCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte esprit choisie.\n";
                                return;
                            }
                            std::cout << "Choix de carte esprit refuse.\n";
                        }
                        else if ((game.isNatureSpiritEnabled() && choice == 4) ||
                                 (!game.isNatureSpiritEnabled() && choice == 3))
                        {
                            int cardIndex;
                            int q;
                            int r;
                            if (!readInt("Index de votre carte animale : ", cardIndex) ||
                                !readInt("Coordonnee q de l'ancrage : ", q) ||
                                !readInt("Coordonnee r de l'ancrage : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.placeAnimalCube(static_cast<std::size_t>(cardIndex), utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube animal pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube animal refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 5)
                        {
                            int q;
                            int r;
                            if (!readInt("Coordonnee q de l'ancrage esprit : ", q) ||
                                !readInt("Coordonnee r de l'ancrage esprit : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (game.placeNatureSpiritCube(utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube esprit pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube esprit refusee.\n";
                        }
                        else
                        {
                            std::cout << "Choix invalide.\n";
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Action impossible : " << e.what() << '\n';
                    }
                }
            } else if (state == core::GameState::WaitingForPlacement) {
                while (true) {
                    displayTurnHeader(game);
                    renderer.displayPersonalBoard(*game.getCurrentPlayer()->getBoard());
                    displayPendingTokens(game);
                    displayOwnedAnimalCards(*game.getCurrentPlayer());
                    if (game.isNatureSpiritEnabled())
                    {
                        displayNatureSpiritCards(*game.getCurrentPlayer());
                    }

                    std::cout << "Actions disponibles :\n";
                    std::cout << "  1. Poser un jeton obligatoire\n";
                    std::cout << "  2. Prendre une carte animale visible\n";
                    if (game.isNatureSpiritEnabled())
                    {
                        std::cout << "  3. Choisir votre carte esprit de la nature\n";
                        std::cout << "  4. Poser un cube animal\n";
                        std::cout << "  5. Poser un cube esprit de la nature\n";
                    }
                    else
                    {
                        std::cout << "  3. Poser un cube animal\n";
                    }

                    int choice;
                    if (!readInt("Votre choix : ", choice))
                    {
                        std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                        return;
                    }

                    try
                    {
                        if (choice == 1)
                        {
                            int pendingIndex;
                            int q;
                            int r;
                            if (!readInt("Index du jeton a poser : ", pendingIndex) ||
                                !readInt("Coordonnee q : ", q) ||
                                !readInt("Coordonnee r : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (pendingIndex < 0 ||
                                static_cast<std::size_t>(pendingIndex) >= game.getPendingTokens().size())
                            {
                                std::cout << "Index de jeton invalide.\n";
                                continue;
                            }
                            model::TokenType token = game.getPendingTokens()[static_cast<std::size_t>(pendingIndex)];
                            if (game.placeTokenOnBoard(utils::HexCoord(q, r), token)) {
                                return;
                            }
                            std::cout << "Placement refuse par le jeu.\n";
                        }
                        else if (choice == 2)
                        {
                            int cardIndex;
                            displayVisibleAnimalCards(game);
                            if (!readInt("Index de la carte animale visible : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.takeVisibleAnimalCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte animale prise.\n";
                                return;
                            }
                            std::cout << "Prise de carte refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 3)
                        {
                            int cardIndex;
                            if (!readInt("Index de la carte esprit a conserver : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.chooseNatureSpiritCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte esprit choisie.\n";
                                return;
                            }
                            std::cout << "Choix de carte esprit refuse.\n";
                        }
                        else if ((game.isNatureSpiritEnabled() && choice == 4) ||
                                 (!game.isNatureSpiritEnabled() && choice == 3))
                        {
                            int cardIndex;
                            int q;
                            int r;
                            if (!readInt("Index de votre carte animale : ", cardIndex) ||
                                !readInt("Coordonnee q de l'ancrage : ", q) ||
                                !readInt("Coordonnee r de l'ancrage : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.placeAnimalCube(static_cast<std::size_t>(cardIndex), utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube animal pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube animal refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 5)
                        {
                            int q;
                            int r;
                            if (!readInt("Coordonnee q de l'ancrage esprit : ", q) ||
                                !readInt("Coordonnee r de l'ancrage esprit : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (game.placeNatureSpiritCube(utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube esprit pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube esprit refusee.\n";
                        }
                        else
                        {
                            std::cout << "Choix invalide.\n";
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Action impossible : " << e.what() << '\n';
                    }
                }
            } else if (state == core::GameState::WaitingForTurnEndChoice) {
                while (true) {
                    displayTurnHeader(game);
                    renderer.displayPersonalBoard(*game.getCurrentPlayer()->getBoard());
                    displayOwnedAnimalCards(*game.getCurrentPlayer());
                    if (game.isNatureSpiritEnabled())
                    {
                        displayNatureSpiritCards(*game.getCurrentPlayer());
                    }

                    std::cout << "Actions disponibles :\n";
                    std::cout << "  1. Terminer le tour et passer au joueur suivant\n";
                    std::cout << "  2. Prendre une carte animale visible\n";
                    if (game.isNatureSpiritEnabled())
                    {
                        std::cout << "  3. Choisir votre carte esprit de la nature\n";
                        std::cout << "  4. Poser un cube animal\n";
                        std::cout << "  5. Poser un cube esprit de la nature\n";
                    }
                    else
                    {
                        std::cout << "  3. Poser un cube animal\n";
                    }

                    int choice;
                    if (!readInt("Votre choix : ", choice))
                    {
                        std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                        return;
                    }

                    try
                    {
                        if (choice == 1)
                        {
                            if (game.endTurn())
                            {
                                std::cout << "Tour termine.\n";
                                return;
                            }
                            std::cout << "Fin de tour refusee.\n";
                        }
                        else if (choice == 2)
                        {
                            int cardIndex;
                            displayVisibleAnimalCards(game);
                            if (!readInt("Index de la carte animale visible : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.takeVisibleAnimalCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte animale prise.\n";
                                return;
                            }
                            std::cout << "Prise de carte refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 3)
                        {
                            int cardIndex;
                            if (!readInt("Index de la carte esprit a conserver : ", cardIndex))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.chooseNatureSpiritCard(static_cast<std::size_t>(cardIndex)))
                            {
                                std::cout << "Carte esprit choisie.\n";
                                return;
                            }
                            std::cout << "Choix de carte esprit refuse.\n";
                        }
                        else if ((game.isNatureSpiritEnabled() && choice == 4) ||
                                 (!game.isNatureSpiritEnabled() && choice == 3))
                        {
                            int cardIndex;
                            int q;
                            int r;
                            if (!readInt("Index de votre carte animale : ", cardIndex) ||
                                !readInt("Coordonnee q de l'ancrage : ", q) ||
                                !readInt("Coordonnee r de l'ancrage : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (cardIndex < 0)
                            {
                                std::cout << "Index invalide.\n";
                                continue;
                            }
                            if (game.placeAnimalCube(static_cast<std::size_t>(cardIndex), utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube animal pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube animal refusee.\n";
                        }
                        else if (game.isNatureSpiritEnabled() && choice == 5)
                        {
                            int q;
                            int r;
                            if (!readInt("Coordonnee q de l'ancrage esprit : ", q) ||
                                !readInt("Coordonnee r de l'ancrage esprit : ", r))
                            {
                                std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                                return;
                            }
                            if (game.placeNatureSpiritCube(utils::HexCoord(q, r)))
                            {
                                std::cout << "Cube esprit pose.\n";
                                return;
                            }
                            std::cout << "Pose du cube esprit refusee.\n";
                        }
                        else
                        {
                            std::cout << "Choix invalide.\n";
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Action impossible : " << e.what() << '\n';
                    }
                }
            } else if (state == core::GameState::GameOver) {
                std::cout << "La partie est terminee.\n";
            }
        }

    }
}
