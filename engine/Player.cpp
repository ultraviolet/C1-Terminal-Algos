#include <bits/stdc++.h>
#include <Player.h>
#include <entity.h>

Player::Player(Board* b) {
    health = 30;
    structurePoints = 40;
    mobilePoints = 5;

    this->b = b;
}

int Player::placeStructure( std::pair<int, int> location, Structure* e) {
    
    if(e->isMobile()) return -1;
    
    if(Board::validPlace(location) && !this->getBoard()->containsUnit(location)) {
        if(this->getStructurePoints() >= e->getCost()) {
            this->setStructurePoints(this->getStructurePoints()-e->getCost());
            this->getBoard()->put(e, location);
            return 0;
        } 
    }

    return -1;
}

int Player::placeMobile(std::pair<int, int> location, Mobile* e) {
    
    if(!e->isMobile()) return -1;

    if(Board::validPlace(location) && !this->getBoard()->containsUnit(location) && this->getBoard()->onEdge(location)) {
        if(this->getMobilePoints() >= e->getCost()) {
            this->setMobilePoints(this->getMobilePoints()-e->getCost());
            this->getBoard()->put(e, location);
            this->getBoard()->getMobileLeft().push_back(e);
            return 0;
        }
    }

    return -1;
}

int Player::remove(std::pair<int, int> location) {
    if(this->getBoard()->validPlace(location) && !this->getBoard()->containsStructure(location)) {
        ((Structure*)(this->getBoard()->entitiesAt(location)[0]))->remove();
    }
}

float Player::getStructurePoints() {
    return this->structurePoints;
}

float Player::getMobilePoints() {
    return this->mobilePoints;
}

void Player::setStructurePoints(float p) {
    this->structurePoints = p;
}

void Player::setMobilePoints(float p){
    this->mobilePoints = p;
}

Board* Player::getBoard() {
    return this->b;
}
