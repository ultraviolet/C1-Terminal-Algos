#pragma once
#include <bits/stdc++.h>
#include <structure.h>

class Wall : Structure {

    public:
        Wall(Player* p, std::pair<int, int> loc);

        void damage(float f);


};