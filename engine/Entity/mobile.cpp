#include <bits/stdc++.h>
#include <mobile.h>
#include <pathfinding.h>

Mobile::Mobile(EntityData entityData, int speed, float range, int damageAmount) : Entity(entityData) {
    assert(Board::onEdge(loc) != BoardEdge::NONE);
    
    if(Board::onEdge(loc) == BoardEdge::TOPLEFT){
        targetEdge = BoardEdge::BOTTOMRIGHT;
    }
    if(Board::onEdge(loc) == BoardEdge::TOPRIGHT){
        targetEdge = BoardEdge::BOTTOMLEFT;
    }
    if(Board::onEdge(loc) == BoardEdge::BOTTOMLEFT){
        targetEdge = BoardEdge::TOPRIGHT;
    }
    if(Board::onEdge(loc) == BoardEdge::BOTTOMRIGHT){
        targetEdge = BoardEdge::TOPLEFT;
    }

    this->speed = speed;
    this->range = range;
    this->damageAmount = damageAmount;
    this->counter = 0;

    this->previousDirectionWasHorizontal = true;
}

void Mobile::move() {
    counter++;
    if(counter % speed == 0){
        std::pair<int, int> originalLocation = getLocation();
        this->getBoard()->remove(originalLocation, this);

        pair<int,int> moveLocation = PathFinding::calculateNextMove(this->getBoard(), this);
    }
}

void Mobile::attack() {
    std::vector<Entity*> vec = this->getInRange(this->range);
    Entity* e = this->getTheoreticalTarget(vec);

    e->takeDamage(this->damageAmount);
}

void Mobile::selfDestruct(){
    int selfDestructDamage = this->getMaxHealth();

    if(this->counter >= 5*speed){
        vector<Entity*> entities = this->getInRange(1.5);
        for(Entity* e : entities){
            if(Util::sameSide(e, this)) e->takeDamage(selfDestructDamage);
        }
    }

    this->health = 0;
    this->getBoard()->remove(this->getLocation(), this);
}

int Mobile::getSpeed() {
    return this->speed;
}