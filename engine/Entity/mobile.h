#include <bits/stdc++.h>
#include <entity.h>

class Mobile : Entity {
    public:
        Mobile(Player* p, int type, int health, int cost, std::pair<int, int> loc);
        BoardEdge targetEdge;
};

