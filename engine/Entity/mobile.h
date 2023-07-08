#pragma once
#include <bits/stdc++.h>
#include <entity.h>

class Mobile : public Entity {

    private:
        int speed, counter;
        float damageAmount, range;

    public:
        Mobile(EntityData entityData, int speed, float range, int damageAmount);
        BoardEdge targetEdge;
        bool previousDirectionWasHorizontal;

        int getSpeed();

        void move();
        void attack();

        void selfDestruct();
};

