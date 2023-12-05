//
// Created by tibov on 25/11/23.
//

#include "EntityModel.h"
#include "../MoveStrategy/PassiveMode.h"
#include "../Observer.h"
#include <iostream>
#include <math.h>
#include <vector>
namespace Logic {
    template <typename T>
    bool between(T a, T b, T c){
        //returns true if c is between a and b
        return a <= c && c <= b;
    }

    EntityModel::EntityModel(const Vector2D &position, const Vector2D &size, double speed, std::shared_ptr<Move::ModeManager> move_manager): position{position}, speed{speed}, size{size},
                                                                                                                                             last_position{position} {
        EntityModel::move_manager = move_manager;
    }

    void EntityModel::move() {
        //move function

        std::shared_ptr<Stopwatch> stopwatch= Logic::Stopwatch::getInstance();

        Vector2D direction = move_manager->getDirection();

        last_position = position;

        position += direction*stopwatch->getDeltaTime()*speed;
        for (std::shared_ptr<Observer> observer: observers){
            observer->moved();
        }

    }


    void EntityModel::addObserver(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    const Vector2D &EntityModel::getPosition() const {
        return position;
    }

    const Vector2D &EntityModel::getSize() const {
        return size;
    }

    std::shared_ptr<Move::ModeManager> EntityModel::getMoveManager() {
        return move_manager;
    }

    std::pair<bool, std::pair<Vector2D, Vector2D>> EntityModel::collide(std::weak_ptr<Subject> other) {

        if (other.expired()){
            throw "expired";
        }

        auto other_lock = other.lock();

        Vector2D center_this_last = last_position + size*0.5;
        Vector2D center_this = position + size*0.5;
        Vector2D center_other = other_lock->getPosition() + other_lock->getSize()*0.5;

        Vector2D distance = (center_this_last-center_this);

        bool collided = false;

        double best = std::numeric_limits<double>::infinity();
        int best_index = -1;
        for (int i = 0; i < 2; i++){
            bool old_between = (center_other- other_lock->getSize()*0.5)[(i+1)%2] <= (center_this_last+ size*0.5)[(i+1)%2] && (center_other + other_lock->getSize()*0.5)[(i+1)%2] >= (center_this_last- size*0.5)[(i+1)%2];
            bool new_between = (center_other- other_lock->getSize()*0.5)[(i+1)%2] <= (center_this+ size*0.5)[(i+1)%2] && (center_other + other_lock->getSize()*0.5)[(i+1)%2] >= (center_this- size*0.5)[(i+1)%2];

            if (!(old_between || new_between)){
                continue;
            }

            Vector2D change = {0, 0};
            if (distance[i] > 0){
                change = size*0.5;
            }else{
                change = size*(-0.5);
            }

            Vector2D t1 = (center_other - (other_lock->getSize()*0.5) + change - center_this)/distance;
            Vector2D t2 = (center_other + (other_lock->getSize()*0.5) + change - center_this)/distance;

            for (auto t: {t1, t2}){
                if (t[i] > 0 && t[i] < best){
                    best = t[i];
                    best_index = i;
                }
            }

        }

        if (best < 1){
            collided = true;
        }

        Vector2D to_collision = (center_this_last*best)+center_this*(1-best);
        Vector2D bounce_direction = Vector2D{0, 0};
        if (best_index == 0){
            bounce_direction = Vector2D{1, 0};
        }else{
            bounce_direction = Vector2D{0, 1};
        }

        return std::make_pair(collided, std::make_pair(to_collision, bounce_direction));

        /*
        Vector2D a = position;
        Vector2D b = size + a;

        Vector2D c = other.lock()->getPosition();
        Vector2D d = other.lock()->getSize() + c;

        std::vector<Vector2D> baselines = {Vector2D{1, 0}, Vector2D{0, 1}};
        bool collided = true;
        Vector2D max_overlap = Vector2D{0, 0};
        for (auto& baseline: baselines){
            Vector2D a_c = a.projection(baseline);
            Vector2D b_c = b.projection(baseline);
            Vector2D c_c = c.projection(baseline);
            Vector2D d_c = d.projection(baseline);

            bool local_between = between<Logic::Vector2D>(a_c, b_c, c_c) || between<Logic::Vector2D>(c_c,d_c,a_c);
            if (!local_between){
                collided = false;
                break;
            }

            std::vector<Vector2D> distances = {d_c-a_c, c_c-b_c};

            Vector2D dir = move_manager->getDirection().projection(baseline);

            auto index = std::min_element(distances.begin(), distances.end(), [&dir](const Vector2D& a, const Vector2D& b){return a.getLength() < b.getLength();});
            max_overlap += *index;

        }


        return std::make_pair(collided, max_overlap);
        */
    }

    void EntityModel::handleImpassable(std::vector<std::weak_ptr<Subject>> others) {

        for (auto other:others){
            auto p = collide(other);

            if (!p.first){
                continue;
            }

            Vector2D travelled = (position-last_position);
            Vector2D travelled_before_collision = (p.second.first-(last_position + size*0.5));

            Vector2D mini = std::min(move_manager->getDirection() - p.second.second, move_manager->getDirection() + p.second.second, [](const Vector2D& a, const Vector2D& b) {return a.getLength() < b.getLength();});


            Vector2D to_do = mini*(travelled-travelled_before_collision).getLength();
            Vector2D a = (p.second.first - size*0.5);
            Vector2D b = position - (travelled - travelled_before_collision);
            std::cout << "before " << p.second.first[0] << " "<< p.second.first[1]<< std::endl;
            position -= (travelled - travelled_before_collision)*1.001;

            std::cout << "blocked" << std::endl;
            std::cout << "set back " << (travelled - travelled_before_collision)[0] << " "<< (travelled - travelled_before_collision)[1]<< std::endl;

            position += to_do;
            other.lock()->debug_green = true;
            /*
            if (!p.first){
                return;
            }*/

            //position += p.second.projection(move_manager->getDirection())*1.000001;
        }


    }

    bool EntityModel::isConsumable() {
        return consumable;
    }

    Vector2D EntityModel::getDirection() const {
        return move_manager->getDirection();
    }

    bool EntityModel::isUp() const {
        return getDirection() == Vector2D{0, -1};
    }

    bool EntityModel::isDown() const {
        return getDirection() == Vector2D{0, 1};
    }

    bool EntityModel::isLeft() const {
        return getDirection() == Vector2D{-1, 0};
    }

    bool EntityModel::isRight() const {
        return getDirection() == Vector2D{1, 0};
    }

    void EntityModel::changeMode() {

    }

    std::vector<Vector2D> EntityModel::splitDirection() {
        auto v = move_manager->getDirection();
        auto temp = {Vector2D{v[0], 0}, Vector2D{0, v[1]}};
        std::vector<Vector2D> output = {};

        for (auto t: temp){
            if (t == Vector2D{0, 0}){
                continue;
            }
            output.push_back(t);
        }

        return output;
    }

    void EntityModel::handleDead(std::vector<std::shared_ptr<Subject>> others) {

    }

    EntityModel::~EntityModel() {

    }

} // Logic