#include "ui/VuePartie.h"
#include <iostream>
#include <QLabel>
#include <QVBoxLayout>

namespace harmonies {
namespace ui {

VuePartie::VuePartie(harmonies::core::Game& gameEngine, QWidget* parent)
    : QWidget(parent), game(gameEngine) {
    
    setWindowTitle("Harmonies - Interface");
    resize(1024, 768);

    mainLayout = new QVBoxLayout(this);
    QLabel* welcomeLabel = new QLabel("Bienvenue dans Harmonies !", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(welcomeLabel);
}

void VuePartie::slotAfficherCarteEsprit() {
    std::cout << "[DEBUG] Action : Afficher carte esprit cliqué." << std::endl;
}

void VuePartie::slotChoisirJetons() {
    std::cout << "[DEBUG] Action : Choisir jetons du marché cliqué." << std::endl;
}

void VuePartie::slotPlacerJetons() {
    std::cout << "[DEBUG] Action : Placer jetons sur le plateau cliqué." << std::endl;
}

void VuePartie::slotPrendreCarteAnimal() {
    std::cout << "[DEBUG] Action : Prendre carte animal cliqué." << std::endl;
}

void VuePartie::slotPoserCubeAnimal() {
    std::cout << "[DEBUG] Action : Poser cube animal cliqué." << std::endl;
}

} // namespace ui
} // namespace harmonies
