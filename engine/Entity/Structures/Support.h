#pragma once
#include <bits/stdc++.h>

class Support : public Structure {

    private:
        std::unordered_set<Entity*> buffed;

    public:
        Support(Player* p, std::pair<int, int> loc);

        void heal();
        void handle();
        std::unordered_set<Entity*> getSet();
};