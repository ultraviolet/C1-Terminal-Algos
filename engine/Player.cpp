#include <bits/stdc++.h>
#include <Player.h>

Player::Player() {
    health = 30;
    structurePoints = 40;
    mobilePoints = 5;
}

float Player::getStructurePoints() {
    return this->structurePoints;
}

float Player::getMobilePoints() {
    return this->getMobilePoints;
}

void Player::setStructurePoints(float p) {
    this->structurePoints = p;
}

void Player::setMobilePoints(float p){
    this->mobilePoints = p;
}
