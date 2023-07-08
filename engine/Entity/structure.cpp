#include <bits/stdc++.h>
#include <structure.h>

Structure::Structure(EntityData entityData, int upgradeCost) : Entity(entityData) {
    this->upgradeCost = upgradeCost;
    this->upgraded = false;
}

int Structure::upgrade() {

    if(this->getUpgraded()) return -1;

    int sp = this->player->getStructurePoints();
    if(sp >= this->getUpgradeCost()) {
        this->player->setStructurePoints(sp - this->getUpgradeCost());
        this->upgraded = true;
        this->handle();
        return 0;
    }
    return -1;
}

void Structure::remove() {
    float s = this->player->getStructurePoints();
    float refund = 75 * this->getCost() * (this->getHealth() / this->getMaxHealth());
    refund = std::floor(10 * refund + 0.5f) / 10;

    this->player->getBoard()->getFrameDeceased().push_back(this);
}
