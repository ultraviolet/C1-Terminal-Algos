#include <bits/stdc++.h>
#include <entity.h>
#include <Player.h>



Entity::Entity(Player* p, TYPE type, float health, int cost, std::pair<int, int> loc) {
    this->type = type;
    this->health = health;
    this->cost = cost;
    this->player = p;
    this->loc = loc;
    this->maxHealth = health;
}

Entity* Entity::getTheoreticalTarget(std::vector<Entity*> possible) {

    int d = 5;

    sort(possible.begin(), possible.end(), 
        
        [](const Entity& a, const Entity& b) {
            if(a.isMobile() && !b.isMobile()) {
                return a;
            } else if (!a.isMobile() && b.isMobile()) {
                return b;
            } else {
                
                d++;

            }
        }

    );

}

bool Entity::isMobile() {
    if(this->getType() == SCOUT || this->getType() == DEMOLISHER || this->getType() == INTERCEPTOR) return true;
    return false;
}

int Entity::damage(int by) {
    this->health -= by;
    return this->health;
}


float Entity::getHealth() {
    return this->health;
}

TYPE Entity::getType() {
    return this->type;
}

int Entity::getCost() {
    return this->cost;
}

float Entity::getMaxHealth() {
    return this->maxHealth;
}

std::pair<int, int> Entity::getLocation() {
    return this->loc;
}

void Entity::destroy() {
    delete this;
}