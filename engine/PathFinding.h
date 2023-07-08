#include <bits/stdc++.h>
#include <Board.h>
#include <entity.h>
#include <Player.h>
#include <mobile.h>

namespace PathFinding{
    vector<Coordinate> destinations(Board* board, Mobile* mobile);
    int calculateIdealness(Coordinate coordinate, BoardEdge targetEdge);
    vector<vector<int> > calculateDistances(Board* board, vector<Coordinate> targets);
    Coordinate calculateNextMove(Board* board, Mobile* mobile);
}
