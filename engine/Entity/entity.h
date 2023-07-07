#pragma once
#include <bits/stdc++.h>
#include <Player.h>

enum TYPE { WALL, SUPPORT, TURRET, SCOUT, DEMOLISHER, INTERCEPTOR };

class Entity {

    protected:
        float health, maxHealth;
        TYPE type;
        int cost;

        std::pair<int, int> loc;

        long long id;

        Player* player;

    public:

        

        Entity(Player* p, TYPE type, float health, int cost, std::pair<int, int> loc);

        int damage(int by);
        int upgrade();                

        std::pair<int, int> getLocation();
        float getHealth();
        float getMaxHealth();
        void handle();
        TYPE getType();
        int getCost();

        void destroy();

        bool isMobile();

        Entity* getTheoreticalTarget(std::vector<Entity*> possible);


};