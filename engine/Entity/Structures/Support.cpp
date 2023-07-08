#include <bits/stdc++.h>
#include <Support.h>
#include <Player.h>

Support::Support(Player* p, std::pair<int, int> loc) : Structure(p, SUPPORT, 30, 4, 4, loc) {};

void Support::heal() {
    int range = this->getUpgraded() ? 7 : 3.5;

    int healAmount = 0;
    if(this->getUpgraded()){
        healAmount = 4 + 0.3 * (float)(this->getLocation().second);
    }else{
        healAmount = 3;
    }

    std::vector<Entity*> vec = this->getInRange(range);
    for(Entity* e : vec) { //CHECK
        if(e->getPlayer()->number == this->player->number && e->isMobile() && this->getSet().find(e) == this->getSet().end()) {
            this->getSet().insert(e);
            e->addHealth(healAmount);
        }
    }
}

void Support::handle() {};

std::unordered_set<Entity*> Support::getSet() {
    return this->buffed;
}