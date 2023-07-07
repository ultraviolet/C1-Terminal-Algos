#include <bits/stdc++.h>
#include <Wall.h>
#include <Player.h>

Wall::Wall(Player* p, std::pair<int, int> loc) : Structure(p, Entity::WALL, 60, 1, loc);


void Wall::damage(float f) {
    float health = this->getHealth();
    if(health-f <= 0) {
        this->destroy()
    }
}

void Wall::handle() {
    this->maxHealth = 120;
    this-health += 60;
}