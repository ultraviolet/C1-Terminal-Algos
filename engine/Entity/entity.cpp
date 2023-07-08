#include <bits/stdc++.h>
#include <entity.h>
#include <Player.h>


Entity::Entity(Player* p, TYPE type, float health, int cost, std::pair<int, int> loc) {
    this->type = type;
    this->health = health;
    this->cost = cost;
    this->player = p;
    this->loc = loc;
    this->maxHealth = health;
}

std::vector<Entity*> Entity::getInRange(float radius) {

    std::vector<Entity*> vec;

    for(int i = 0; i < 27; i++) {
        for(int j = 0; j < 27; j++) {
            pair<int,int> position = std::make_pair(i, j);
            
            for(Entity* e : this->player->getBoard()->entitiesAt(position)) {
                if(Util::distance(position, this->getLocation()) <= radius) {
                    vec.push_back(e);
                }
            }
        }
    }

    return vec;
}

Entity* Entity::getTheoreticalTarget(std::vector<Entity*> possible) {

    std::pair<int, int> d = this->getLocation();
    

    sort(possible.begin(), possible.end(), 
        
        [d, this](Entity& a, Entity& b) {
            if(a.isMobile() && !b.isMobile()) {
                return true;
            } else if (!a.isMobile() && b.isMobile()) {
                return false;
            } else {
                
                if(Util::distance(a.getLocation(), d) == Util::distance(b.getLocation(), d)) {
                    
                    if(a.getHealth() == b.getHealth()) {
                        if(a.getLocation().second == b.getLocation().second) {
                            int x1 = a.getLocation().first;
                            int x2 = b.getLocation().first;

                            int dist1 = x1 <= 13 ? x1 : 27-x1;
                            int dist2 = x2 <= 13 ? x2 : 27-x2;

                            return dist1 < dist2;
                        } else {
                            bool lowerY = (a.getLocation().second < b.getLocation().second);
                            return (lowerY && this->getPlayer()->number == 1 || !lowerY && this->getPlayer()->number == 2);
                        }
                    } else {
                        return a.getHealth() < b.getHealth();
                    }

                } else {
                    return Util::distance(a.getLocation(), d) < Util::distance(b.getLocation(), d);
                }

            }
        }

    );

    if(possible.size() == 0) {
        return nullptr;
    }
   
    for(auto& e : possible) {

        if(this->getType() == TURRET && !e->isMobile()) {
            continue;
        }
        if(this->getType() == INTERCEPTOR && !e->isMobile()) {
            continue;
        }

        if(!Util::sameSide(this, e)) {
            return e;
        }
    }

    return nullptr;

}

void Entity::addHealth(float change) {
    this->health += change;
}

Player* Entity::getPlayer() {
    return this->player;
}

bool Entity::isMobile() {
    if(this->getType() == SCOUT || this->getType() == DEMOLISHER || this->getType() == INTERCEPTOR) return true;
    return false;
}

float Entity::damage(float by) {
    this->health -= by;

    if(this->health <= 0) {
        delete this;
    }

    return this->health;
}


float Entity::getHealth() {
    return this->health;
}

TYPE Entity::getType() {
    return this->type;
}

int Entity::getCost() {
    return this->cost;
}

float Entity::getMaxHealth() {
    return this->maxHealth;
}

std::pair<int, int> Entity::getLocation() {
    return this->loc;
}

Board* Entity::getBoard(){
    return this->getPlayer()->getBoard();
}

void Entity::destroy() {
    
    this->getBoard()->getFrameDeceased().push_back(this);
    
    //this->getBoard()->getFrameDeceased()->push_back(this);
}