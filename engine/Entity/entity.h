#pragma once
#include <bits/stdc++.h>
#include <Player.h>
#include <util.cpp>

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

        float damage(float by);
        int upgrade();                

        std::pair<int, int> getLocation();
        float getHealth();
        void addHealth(float change);
        float getMaxHealth();
        TYPE getType();
        int getCost();

        void destroy();

        bool isMobile();
        
        Board* getBoard();

        Player* getPlayer();




        Entity* getTheoreticalTarget(std::vector<Entity*> possible);
        std::vector<Entity*> getInRange(float radius);


};