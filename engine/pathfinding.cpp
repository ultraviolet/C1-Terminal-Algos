#include <bits/stdc++.h>
#include <Board.h>
#include <entity.h>
#include <Player.h>
#include <mobile.h>
using Coordinate = std::pair<int,int>;

Coordinate adjList[4] = {Coordinate(0,1), Coordinate(1,0), Coordinate(0,-1), Coordinate(-1,0)};

Coordinate destination(Board* board, Mobile* mobile){
    Coordinate start = mobile->getLocation();
    int targetEdge = mobile->targetEdge;
    int player = mobile->player->number;
    
    assert(targetEdge == 1 || targetEdge == 2);
    assert(player == 1 || player == 2);
    assert(board->validSpot(start));

    bool visited[28][28];
    std::queue<Coordinate> q;

    q.push(start);
    visited[start.first][start.second] = true;

    Coordinate destinations[28];

    while(q.size() > 0){
        Coordinate c = q.front();
        q.pop();

        for(Coordinate direction : adjList){
            Coordinate adj = Coordinate(c.first+direction.first, c.second+direction.second);

            if(board->validSpot(adj) && !board->containsStructure(adj) && !visited[adj.first][adj.second]){
                q.push(adj);
                visited[adj.first][adj.second] = true;
            }
        }
    }
}
