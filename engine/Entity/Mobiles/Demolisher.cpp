#include <Demolisher.h>

Demolisher::Demolisher(Player* p, std::pair<int, int> location) : Mobile(EntityData(p, DEMOLISHER, location, 5, 3), 2, 4.5, 8) {};