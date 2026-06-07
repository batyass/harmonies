#ifndef VUEPARTIE_H
#define VUEPARTIE_H

#include <QWidget>
#include <QVBoxLayout>

namespace harmonies {
    namespace core {
        class Game;
    }
}

namespace harmonies {
namespace ui {

class VuePartie : public QWidget {
    Q_OBJECT
public:
    explicit VuePartie(harmonies::core::Game& gameEngine, QWidget* parent = nullptr);

private slots:
    void slotAfficherCarteEsprit();
    void slotChoisirJetons();
    void slotPlacerJetons();
    void slotPrendreCarteAnimal();
    void slotPoserCubeAnimal();

private:
    harmonies::core::Game& game;
    QVBoxLayout* mainLayout;
};

} // namespace ui
} // namespace harmonies

#endif // VUEPARTIE_H
