#include <Turret.h>
#include <bits/stdc++.h>


Turret::Turret(Player* p, std::pair<int, int> loc) : Structure(EntityData(p, TURRET, loc, 75, 2), 4) {};

void Turret::attack() {
    int range = this->getUpgraded() ? 3.5 : 2.5;
    int damage = this->getUpgraded() ? 15 : 5;

    std::vector<Entity*> vec = this->getInRange(2.5);
    Entity* e = this->getTheoreticalTarget(vec);

    e->takeDamage(5);
}

void Turret::handle() {}