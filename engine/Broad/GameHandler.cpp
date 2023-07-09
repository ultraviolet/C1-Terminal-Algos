#include <bits/stdc++.h>
#include <GameHandler.h>
#include <Support.h>

GameHandler::GameHandler(Player* p1, Player* p2) {
    this->p1 = p1;
    this->p2 = p2;
}

Player* GameHandler::getP1() {
    return this->p1;
}

Player* GameHandler::getP2() {
    return this->p2;
}

void GameHandler::nextTurn() {

    while(this->getP1()->getBoard()->getMobileLeft().size() > 0) {
        frame();
    }
}

void GameHandler::frame() {

    Board* curBoard = this->getP1()->getBoard();

    //Each Support grants a Shield
    for(int i = 0; i < 28; i++) {
        for(int j = 0; j < 28; j++) {

            for(Entity* e : curBoard->entitiesAt(std::make_pair(i, j))) {

                if(e->getType() == SUPPORT) {
                    ((Support) e)->heal();
                }

            }

        }
    }
    
    // all units move or self destruct

    // all units attack

    // remove dead

}