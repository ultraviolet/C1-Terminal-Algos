#include <bits/stdc++.h>
#include <entity.h>

Entity::Entity(int type, int health, int cost) {
    this->type = type;
    this->health = health;
    this->cost = cost;
    this->upgraded = false;
    this->upgradeCost = INT_MAX;
}

Entity::Entity(int type, int health, int cost, int upgradeCost, bool upgraded) {
    this->type = type;
    this->health = health;
    this->cost = cost;
    this->upgraded = upgraded;
    this->upgradeCost = upgradeCost;
}

int Entity::damage(int by) {
    this->health -= by;
    return this->health;
}

int Entity::upgrade() {
    
}