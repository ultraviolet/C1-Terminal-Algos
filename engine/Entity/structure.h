#pragma once
#include <bits/stdc++.h>
#include <entity.h>

class Structure : public Entity {

    protected:
        int upgradeCost;
        bool upgraded;

    public:
        Structure(EntityData entityData, int upgradeCost);

        int upgrade();
        virtual void handle();
        void remove();
        
        int getUpgradeCost();
        bool getUpgraded();
};