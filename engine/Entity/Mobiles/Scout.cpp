#include <bits/stdc++.h>
#include <Scout.h>
#include <entity.h>
#include <Player.h>

Scout::Scout(Player* p, std::pair<int, int> location) : Mobile(p, SCOUT, 15, 1, location, 1, 3.5f, 2) {};