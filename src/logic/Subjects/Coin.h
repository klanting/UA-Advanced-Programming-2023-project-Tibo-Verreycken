//
// Created by tibov on 25/11/23.
//

#ifndef PROJECTPACMAN_COINVIEW_H
#define PROJECTPACMAN_COIN_H
#include "Collectable.h"
namespace Logic {

    class Coin: public Collectable{
    public:
        Coin(const Vector2D& position, std::shared_ptr<Move::ModeManager> move_manager);
    private:

    };

} // Logic

#endif //PROJECTPACMAN_COINVIEW_H
