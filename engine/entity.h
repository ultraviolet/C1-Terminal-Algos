#pragma once

class Entity {

    protected:
        int health, type, cost, upgradeCost;
        bool upgraded;

        long long id;

    public:
        Entity(int type, int health, int cost);
        Entity(int type, int health, int cost, int upgradeCost, bool upgraded);

        int damage(int by);
        int upgrade();


            

};