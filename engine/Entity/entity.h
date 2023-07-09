#pragma once
#include <bits/stdc++.h>
#include <Player.h>
#include <util.cpp>

enum TYPE { WALL, SUPPORT, TURRET, SCOUT, DEMOLISHER, INTERCEPTOR };

struct entityData{
    Player* player;
    TYPE type;
    Coordinate location;
    float maxHealth;
    int cost;

    entityData(Player* p, TYPE type, Coordinate loc, float maxHealth, int cost) 
        : player(p), type(type), location(loc), maxHealth(maxHealth), cost(cost){
        
    }
};
typedef struct entityData EntityData;

class Entity {

    protected:
        float health, maxHealth;
        TYPE type;
        int cost;

        std::pair<int, int> loc;

        long long id;

        Player* player;

    public:
        Entity(EntityData entityData);

        float takeDamage(float by);
        void handleDeath();

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