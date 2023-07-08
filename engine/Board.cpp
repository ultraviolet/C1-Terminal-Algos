#include <bits/stdc++.h>
#include <Board.h>
#include <structure.h>
#include <mobile.h>
using Coordinate = std::pair<int,int>;


bool Board::validSpot(Coordinate coordinate){
    int x = coordinate.first;
    int y = coordinate.second;

    if(x > 27 || x < 0 || y > 27 || y < 0) return false;
    
    if(x <= 13 && y <= 13 && x+y >= 13) return true;
    if(x >= 14 && y >= 14 && x+y <= 41) return true;
    if(x >= 14 && y <= 13 && x-y <= 14) return true;
    if(x <= 13 && y >= 14 && y-x <= 14) return true;

    return false;
}

void Board::put(Entity* e, Coordinate c) {
    map[c.first][c.second].push_back(e);
}

bool Board::validPlace(Coordinate cd) {
    if(Board::validSpot(cd) && cd.second <= 13) return true;
    return false;
}

BoardEdge Board::onEdge(Coordinate coordinate){
    int x = coordinate.first;
    int y = coordinate.second;
    
    if(x > 27 || x < 0 || y > 27 || y < 0) return BoardEdge::NONE;

    if(y-x == 14) return BoardEdge::TOPLEFT;
    if(x+y == 41) return BoardEdge::TOPRIGHT;
    if(x-y == 14) return BoardEdge::BOTTOMRIGHT;
    if(x+y == 13) return BoardEdge::BOTTOMLEFT;

    return BoardEdge::NONE;
};

bool Board::containsUnit(Coordinate coordinate){
    return (entitiesAt(coordinate).size() > 0);
}

bool Board::containsStructure(Coordinate coordinate){
    if(!containsUnit(coordinate)) return false;

    Entity* e = map[coordinate.first][coordinate.second][0];

    if(e->isMobile()) return false;
    return true;
}

std::vector<Mobile*> Board::getMobileLeft() {
    return this->mobileLeft;
}

std::vector<Entity*> Board::getFrameDeceased() {
    return this->frameDeceased;
}

std::vector<Entity*> Board::entitiesAt(std::pair<int, int> location) {
    if(!this->validSpot(location)) return vector<Entity*>();

    return map[location.first][location.second];
}

void Board::remove(Coordinate coordinate, Entity* e) {
    auto it = std::find(this->map[coordinate.first][coordinate.second].begin(), this->map[coordinate.first][coordinate.second].end(), e);
    this->map[coordinate.first][coordinate.second].erase(it);
}

// int main(){
//     Board b = Board();

//     // std::cout << b.validSpot(13, 13) << std::endl;
//     // std::cout << b.validSpot(0, 13) << std::endl;
//     // std::cout << b.validSpot(0, 13) << std::endl;
//     // std::cout << b.validSpot(0, 14) << std::endl;
//     // std::cout << b.validSpot(14, 0) << std::endl;
//     // std::cout << b.validSpot(13, 27) << std::endl;
//     // std::cout << b.validSpot(27, 13) << std::endl;
//     // std::cout << b.validSpot(14, 27) << std::endl;
//     // std::cout << b.validSpot(27, 14) << std::endl;
//     // std::cout << b.validSpot(0, 15) << std::endl;
//     // std::cout << b.validSpot(0, 0) << std::endl;
//     // std::cout << b.validSpot(3, 4) << std::endl;
// }
