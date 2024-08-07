//
// Created by tibov on 02/12/23.
//

#include "RenderWindowSingleton.h"

#include <utility>

namespace View {
    std::shared_ptr<RenderWindowSingleton > RenderWindowSingleton::getInstance() {
        if (!_instance){
            _instance = std::shared_ptr<RenderWindowSingleton >(new RenderWindowSingleton);
        }
        return _instance;
    }

    RenderWindowSingleton::RenderWindowSingleton() {
        window = std::make_unique<RW>(sf::VideoMode(1000, 1000), "Pacman");
        window->setKeyRepeatEnabled(false);
    }



    void RenderWindowSingleton::draw(const std::weak_ptr<Logic::EntityModel>& e, std::shared_ptr<sf::Drawable> s) {
        if (e.expired()){
            throw std::bad_weak_ptr();
        }
        draw_buffer[e.lock()] = std::move(s);
    }

    void RenderWindowSingleton::display() {
        for (auto& [k, v]: draw_buffer){
            window->draw(*v);
        }
        draw_buffer.clear();
        window->display();

    }

    sf::Vector2<unsigned int> RenderWindowSingleton::getSize() const{
        return window->getSize();
    }

    void RenderWindowSingleton::drawBufferless(const std::shared_ptr<sf::Drawable>& s) {
        window->draw(*s);
    }

    bool RenderWindowSingleton::isOpen() {
        return window->isOpen();
    }

    bool RenderWindowSingleton::pollEvent(sf::Event &event) {
        return window->pollEvent(event);
    }

    void RenderWindowSingleton::close() {
        window->close();
    }

    void RenderWindowSingleton::clear() {
        window->clear();
    }


} // View