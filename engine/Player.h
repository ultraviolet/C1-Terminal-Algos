#pragma once
#include <bits/stdc++.h>
#include <entity.h>

class Player {

    private:
        float structurePoints, mobilePoints;
        int health;

    public:
        Player();
        
        void place(Entity e, std::pair<int, int> location);
        void remove(std::pair<int, int> location);

        float getStructurePoints();
        float getMobilePoints();

        void setStructurePoints(float p);
        void setMobilePoints(float p);

        int number;
};