//
// Created by tibov on 26/11/23.
//

#include "Game.h"
#include <iostream>

using namespace std;
namespace View {
    typedef sf::RenderWindow RW;
    Game::Game() {
        window = RenderWindowSingleton::getInstance();
        state_manager = make_shared<StateManager>(make_unique<MenuState>());
        doGameLoop();
    }

    void Game::doGameLoop() {
        window->getWindow()->setKeyRepeatEnabled(false);
        while (window->getWindow()->isOpen())
        {
            sf::Event event;
            while (window->getWindow()->pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
                    bool pressed = event.type == sf::Event::KeyPressed;
                    processInput(event.key.code, pressed);
                }

                if (event.type == sf::Event::Closed){
                    window->getWindow()->close();
                }

            }

            window->getWindow()->clear();

            state_manager->runTick();

            window->getWindow()->display();
        }

    }

    void Game::processInput(int input, bool pressed) {
        state_manager->acceptCharacter(input, pressed);
    }
} // View