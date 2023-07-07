#include <bits/stdc++.h>
#include <structure.h>

Structure::Structure(Player* p, int type, int health, int cost, int upgradeCost, std::pair<int, int> loc) : Entity(p, type, health, cost, loc) {
    this->upgradeCost = upgradeCost;
}

Structure::Structure(Player* p, int type, int health, int cost, int upgradeCost, bool upgraded, std::pair<int, int> loc) : Entity(p, type, health, cost, loc) {
    this->upgradeCost =  upgradeCost;
    this->upgraded = upgraded;
}

int Structure::upgrade() {
    int sp = this->player->getStructurePoints();
    if(sp >= this->getUpgradeCost()) {
        this->player->setStructurePoints(sp - this->getUpgradeCost());
        this->handle();
        return 0;
    }
    return -1;
}

void Structure::remove() {
    float s = this->player->getStructurePoints();
    float refund = 75 * this->getCost() * (this->getHealth() / this->getMaxHealth());
    refund = std::floor(10 * refund + 0.5f) / 10;

    delete this;
}

virtual void Structure::handle() {
    std::cout << "YOU FORGOT TO INHERIT HANDLE() IN A CLASS!!!!!" << "\n";
}