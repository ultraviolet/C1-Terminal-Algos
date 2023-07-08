#pragma once
#include <bits/stdc++.h>
#include <entity.h>
#include <Board.h>

class Player {

    private:
        float structurePoints, mobilePoints;
        int health;

        Board* b;

    public:
        Player(Board* b);
        
        void place(Entity e, std::pair<int, int> location);
        int remove(std::pair<int, int> location);

        int placeStructure(std::pair<int, int> location, Structure* e);
        int placeMobile(std::pair<int, int> location, Mobile* e);

        float getStructurePoints();
        float getMobilePoints();

        void setStructurePoints(float p);
        void setMobilePoints(float p);

        Board* getBoard();

        int number;
};