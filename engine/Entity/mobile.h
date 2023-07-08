#pragma once
#include <bits/stdc++.h>
#include <entity.h>

class Mobile : public Entity {

    private:
        int speed, range;
        float damageAmount, counter;

    public:
        Mobile(Player* p, TYPE type, int health, int cost, std::pair<int, int> loc, int speed, float range, float damageAmount);
        BoardEdge targetEdge;
        bool previousDirectionWasHorizontal;

        int getSpeed();

        void move();
        void damage();

        void selfDestruct();
};

