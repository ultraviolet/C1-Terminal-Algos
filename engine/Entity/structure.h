#pragma once
#include <bits/stdc++.h>
#include <entity.h>

class Structure : public Entity {

    protected:
        int upgradeCost;
        bool upgraded;

    public:
        Structure(Player* p, int type, int health, int cost, int upgradeCost, std::pair<int, int> loc);
        Structure(Player* p, int type, int health, int cost, int upgradeCost, bool upgraded, std::pair<int, int> loc);

        int upgrade();
        virtual void handle();
        void remove();
        
        int getUpgradeCost();
        bool getUpgraded();


};