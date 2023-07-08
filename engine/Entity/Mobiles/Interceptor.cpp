#include <bits/stdc++.h>
#include <Interceptor.h>
#include <entity.h>
#include <Player.h>

Interceptor::Interceptor(Player* p, std::pair<int, int> location) : Mobile(EntityData(p, INTERCEPTOR, location, 40, 1), 4, 4.5, 20) {};