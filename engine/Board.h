using Coordinate = std::pair<int,int>;

class Board{
    public:
        int map[28][28];

        bool validSpot(Coordinate coordinate);
        int onEdge(Coordinate coordinate);
        bool containsUnit(Coordinate coordinate);
};