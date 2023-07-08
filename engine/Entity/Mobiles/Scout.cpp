#include <bits/stdc++.h>
#include <Scout.h>
#include <entity.h>
#include <Player.h>

Scout::Scout(Player* p, std::pair<int, int> location) : Mobile(EntityData(p, SCOUT, location, 15, 1), 1, 3.5, 2) {};