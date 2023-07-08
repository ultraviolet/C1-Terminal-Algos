#pragma once
#include <bits/stdc++.h>
#include <structure.cpp>

class Turret : public Structure {

    public:
        Turret(Player* p, std::pair<int, int> loc);

        void attack();
        void handle();
};