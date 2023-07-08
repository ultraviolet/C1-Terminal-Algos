#include <bits/stdc++.h>
#include <Wall.h>
#include <Player.h>

Wall::Wall(Player* p, std::pair<int, int> loc) : Structure(p, WALL, 60, 1, 1, loc) {};


void Wall::handle() {
    this->maxHealth = 120;
    this->health += 60;
}