#include <bits/stdc++.h>
#include <Board.h>
#include <entity.h>
#include <Player.h>
#include <mobile.h>
using Coordinate = std::pair<int,int>;
using vvi = vector<vector<int>>;

namespace PathFinding{
    Coordinate adjList[4] = {Coordinate(0,1), Coordinate(1,0), Coordinate(0,-1), Coordinate(-1,0)};

    vector<Coordinate> destinations(Board* board, Mobile* mobile){
        Coordinate start = mobile->getLocation();
        BoardEdge targetEdge = mobile->targetEdge;
        
        assert(targetEdge != BoardEdge::NONE);
        assert(board->validSpot(start));

        int visited[28][28] = {-1};
        
        std::queue<Coordinate> q;

        q.push(start);
        visited[start.first][start.second] = true;

        //destination criteria:
        //1. target edge
        //2. max depth, closest to target edge
        //idealness:
        //target edge: 100000000000
        //else: 100*depth-horizontalDistance
        int idealness = 0;
        std::vector<Coordinate> tiedDestinations = std::vector<Coordinate>();
        tiedDestinations.push_back(start);

        while(q.size() > 0){
            Coordinate c = q.front();
            q.pop();

            for(Coordinate direction : adjList){
                Coordinate adj = Coordinate(c.first+direction.first, c.second+direction.second);

                if(board->validSpot(adj) && !board->containsStructure(adj) && !visited[adj.first][adj.second]){
                    q.push(adj);
                    visited[adj.first][adj.second] = true;

                    int current_idealness = calculateIdealness(adj, targetEdge);

                    if(current_idealness > idealness){
                        idealness = current_idealness;
                        tiedDestinations.clear();
                        tiedDestinations.push_back(adj);
                    } else if(current_idealness == idealness){
                        tiedDestinations.push_back(adj);
                    }
                }
            }
        }

        return tiedDestinations;
    }

    int calculateIdealness(Coordinate coordinate, BoardEdge targetEdge){
        int idealness = 0;
        if(Board::onEdge(coordinate) == targetEdge){
            idealness = 100000000000;
        } else {
            int horizontalDistance = 0;
            if(targetEdge == BoardEdge::TOPLEFT || targetEdge == BoardEdge::BOTTOMLEFT){
                horizontalDistance = coordinate.first;
            } else {
                horizontalDistance = 27-coordinate.first;
            }

            int depth = 0;
            if(targetEdge == BoardEdge::TOPLEFT || targetEdge == BoardEdge::TOPRIGHT){
                depth = coordinate.second;
            } else {
                depth = 27-coordinate.second;
            }
            idealness = 100*depth-horizontalDistance;
        }
        
        return idealness;
    }


    vvi calculateDistances(Board* board, vector<Coordinate> targets){
        vvi distances = vvi(28, vector<int>(28, -1));
        std::queue<Coordinate> q;

        for(Coordinate target : targets){
            q.push(target);
            distances[target.first][target.second] = 0;
        }

        while(q.size() > 0){
            Coordinate c = q.front();
            q.pop();

            for(Coordinate direction : adjList){
                Coordinate adj = Coordinate(c.first+direction.first, c.second+direction.second);

                if(board->validSpot(adj) && !board->containsStructure(adj) && distances[adj.first][adj.second] == -1){
                    q.push(adj);
                    distances[adj.first][adj.second] = distances[c.first][c.second]+1;
                }
            }
        }

        return distances;
    }

    Coordinate calculateNextMove(Board* board, Mobile* mobile){
        Coordinate start = mobile->getLocation();
        BoardEdge targetEdge = mobile->targetEdge;
        
        vector<Coordinate> targets = destinations(board, mobile);
        vvi distances = calculateDistances(board, targets);

        // choosing move
        // 1. shortest distance
        // 2. opposite of previous movement
        // 3. direction of target edge

        //idealness:
        //distance * -4 + 2*opposite + edgeDirection

        if(distances[start.first][start.second] == 0){
            return start; //represents self destruct
        }

        int idealness = -10000;
        Coordinate bestMove = Coordinate(0,0);

        for(Coordinate direction : adjList){
            Coordinate adj = Coordinate(start.first+direction.first, start.second+direction.second);

            if(board->validSpot(adj) && !board->containsStructure(adj)){
                int currentDistance = distances[adj.first][adj.second];
                if(currentDistance == -1){
                    continue;
                }else{
                    int currentIdealness = currentDistance*-4;
                    bool vertical = (adj.first == 0);
                    if(vertical == mobile->previousDirectionWasHorizontal){
                        currentIdealness += 2;
                    }
                    if(targetEdge == BoardEdge::TOPLEFT && (adj.first == -1 || adj.second == 1)){
                        currentIdealness += 1;
                    }
                    if(targetEdge == BoardEdge::TOPRIGHT && (adj.first == 1 || adj.second == 1)){
                        currentIdealness += 1;
                    }
                    if(targetEdge == BoardEdge::BOTTOMLEFT && (adj.first == -1 || adj.second == -1)){
                        currentIdealness += 1;
                    }
                    if(targetEdge == BoardEdge::BOTTOMRIGHT && (adj.first == 1 || adj.second == -1)){
                        currentIdealness += 1;
                    }

                    if(currentIdealness > idealness){
                        idealness = currentIdealness;
                        bestMove = adj;
                    }
                }
            }
        }

        return bestMove;
    }
}