//
// Created by tibov on 26/11/23.
//

#ifndef PROJECTPACMAN_VICTORYSTATE_H
#define PROJECTPACMAN_VICTORYSTATE_H
#include "State.h"
#include "../UIObjects/PositionAnimation.h"
namespace View {

    class VictoryState: public State{
    public:
        VictoryState();
        void runTick() override;
        void acceptCharacter(int input, bool pressed, const Logic::Vector2D& v) override;
    private:
        void renderUI();
        double key_press_delay = 1;
        sf::Texture texture;

        int animation_index = 0;
        double animation_delay = 0.25;
        double animation_position = -0.9;
        int animation_direction = 1;

        void createEatAnimation();

    };



} // View

#endif //PROJECTPACMAN_VICTORYSTATE_H
