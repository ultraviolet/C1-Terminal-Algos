#include <bits/stdc++.h>
#include <entity.h>

using namespace std;

class Util{

    public:

        static float distance(std::pair<int, int> f, std::pair<int, int> s) {
            float f1 = f.first, f2 = f.second, s1 = s.first, s2 = s.second;

            return std::sqrt(std::pow((f1-s1), 2)+std::pow((f2-s2), 2));
        }

        static bool sameSide(Entity* e1, Entity* e2) {

            return e1->getPlayer()->number == e2->getPlayer()->number;

        }
};

