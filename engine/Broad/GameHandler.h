#pragma once
#include <bits/stdc++.h>
#include <Player.h>

class GameHandler {

    private:
        Player* p1;
        Player* p2;

    public:

        GameHandler(Player* p1, Player* p2);
        
        Player* getP1();
        Player* getP2();

        void nextTurn();
        void frame();



}