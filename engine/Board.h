using Coordinate = std::pair<int,int>;

#include <bits/stdc++.h>
#include <entity.h>
#include <structure.h>
#include <mobile.h>

enum BoardEdge {
    NONE = 0,
    TOPLEFT = 1,
    TOPRIGHT = 2,
    BOTTOMLEFT = 3,
    BOTTOMRIGHT = 4
};

class Board{
    
    private:
        vector<Entity*> map[28][28];

        std::vector<Entity*> frameDeceased;
        std::vector<Mobile*> mobileLeft; 
    
    public:

        std::vector<Entity*> entitiesAt(std::pair<int, int> location);
        
        static bool validSpot(Coordinate coordinate);
        static bool validPlace(Coordinate coordinate);
        static BoardEdge onEdge(Coordinate coordinate);
        bool containsUnit(Coordinate coordinate);
        bool containsStructure(Coordinate coordinate);

        void remove(Coordinate coordinate, Entity* e);

        void put(Entity* e, Coordinate c);

        std::vector<Mobile*> getMobileLeft();
        std::vector<Entity*> getFrameDeceased();
};