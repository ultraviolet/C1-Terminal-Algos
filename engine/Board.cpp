#include <bits/stdc++.h>
#include <Board.h>

Board::validSpot(Coordinate coordinate){
    int x = coordinate.first;
    int y = coordinate.second;

    if(x > 27 || x < 0 || y > 27 || y < 0) return false;
    
    if(x <= 13 && y <= 13 && x+y >= 13) return true;
    if(x >= 14 && y >= 14 && x+y <= 41) return true;
    if(x >= 14 && y <= 13 && x-y <= 14) return true;
    if(x <= 13 && y >= 14 && y-x <= 14) return true;

    return false;
}

int Board::onEdge(Coordinate coordinate){
    int x = coordinate.first;
    int y = coordinate.second;
    
    if(x > 27 || x < 0 || y > 27 || y < 0) return 0;

    if(y-x == 14) return 1;
    if(x+y == 41) return 2;

    return 0;
;

Board::containsUnit(Coordinate coordinate){
    return false; // TODO
}


int main(){
    Board b = Board();

    // std::cout << b.validSpot(13, 13) << std::endl;
    // std::cout << b.validSpot(0, 13) << std::endl;
    // std::cout << b.validSpot(0, 13) << std::endl;
    // std::cout << b.validSpot(0, 14) << std::endl;
    // std::cout << b.validSpot(14, 0) << std::endl;
    // std::cout << b.validSpot(13, 27) << std::endl;
    // std::cout << b.validSpot(27, 13) << std::endl;
    // std::cout << b.validSpot(14, 27) << std::endl;
    // std::cout << b.validSpot(27, 14) << std::endl;
    // std::cout << b.validSpot(0, 15) << std::endl;
    // std::cout << b.validSpot(0, 0) << std::endl;
    // std::cout << b.validSpot(3, 4) << std::endl;
}
