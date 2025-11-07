#include <iostream>
#include <vector>
#include <conio.h>
#include "stdlib.h"


#define WIDTH 10
#define HEIGHT 10
#define OBSTACLE_NB 15

// Codes ANSI pour les couleurs

#define CURSOR      "\033[31m|+|\033[0m"
#define PLAYER      "\033[35m|O|\033[0m"
#define PATH        "\033[32m|P|\033[0m"
#define OBSTACLE    "\033[34m|#|\033[0m"
#define VISITED     "\033[37m|V|\033[0m"
#define EMPTY       "| |"


struct GridCoord//pas faire 2 update...
{
    int row;
    int col;
public:
    bool operator==(const GridCoord& coord) const
    {
        return row == coord.row && col == coord.col;
    }
};

struct GridData
{
    GridCoord mPlayer;
    GridCoord mCursor;

    std::vector<GridCoord> obstacles;
};

struct Tile
{
    GridCoord position;

    bool walkable = true;
    bool isVisited = false;

    Tile* cameFrom = nullptr;
};


std::vector<std::vector<const char*>> grid;

std::vector<std::vector<Tile>> tiles;

int GenerateRandomNumber(int min, int max)
{
    int range = max - min;

    int value = rand() % range + min;

    return value;
}

bool IsObstacle(GridCoord coord, std::vector<GridCoord>& pObstacle);

void InitGrid(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        std::vector<const char*> temp;

        std::vector<Tile> tileTemp;

        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            Tile tempTile;
            tempTile.position = tempCoord;

            if (tempCoord == cursor)
                temp.push_back(CURSOR);

            else if (IsObstacle(tempCoord, obstacles))
            {
                temp.push_back(OBSTACLE);
                tempTile.walkable = false;
            }

            else if (tempCoord == player)
            {
                temp.push_back(PLAYER);
                tempTile.walkable = false;
            }

            else
                temp.push_back(EMPTY);

            tileTemp.push_back(tempTile);
        }
        grid.push_back(temp);

        tiles.push_back(tileTemp);
    }
}

void ClearMap()
{
    int x = 0;
    int y = 0;

    for (std::vector<const char*> ligne : grid)
    {
        x = 0;
        for (const char* tile : ligne)
        {
            if (tile == PATH || tile == VISITED)
                grid[y][x] = EMPTY;
            x++;
        }
        std::cout << std::endl;

        y++;
    }
}

void ClearTiles()
{

    for (std::vector<Tile>& tile : tiles)//boucle auto
    {
        for (Tile& temp : tile)//boucle auto
        {
            temp.isVisited = false;
            temp.cameFrom = nullptr;
        }
    }
}

bool IsObstacle(GridCoord coord, std::vector<GridCoord>& pObstacle)
{
    for (int k = 0; k < pObstacle.size(); k++)
    {
        if (coord == pObstacle[k])
        {
            return true;
        }
    }
    return false;
}

void PrintGrid()
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            printf(grid[i][j]);
        }
        std::cout << std::endl;
    }
}

void DrawPath(GridCoord& posCurseur);

void UpdateGrid(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    system("cls");

    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            grid[i][j] = EMPTY;

            DrawPath(cursor);

            if (tiles[i][j].isVisited == true)
                grid[i][j] = VISITED;

            if (tiles[i][j].cameFrom != nullptr)
            {
                if (tempCoord == tiles[i][j].cameFrom->position)
                    grid[i][j] = PATH;
            }
            

            if (IsObstacle(tempCoord, obstacles))
                grid[i][j] = OBSTACLE;

            if (tempCoord == player)
                grid[i][j] = PLAYER;

            if (tempCoord == cursor)
                grid[i][j] = CURSOR;
        }
    }

    PrintGrid();
}

void PathFinding(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles, bool move);

void Move(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    char input = _getch();

    if (input == 'z')
    {
        cursor.row--;

        if (cursor.row <= 0)
            cursor.row = 0;

        if (cursor == player || IsObstacle(cursor, obstacles))
            cursor.row++;
    }

    if (input == 's')
    {
        cursor.row++;

        if (cursor.row >= HEIGHT)
            cursor.row = HEIGHT - 1;

        if (cursor == player || IsObstacle(cursor, obstacles))
            cursor.row--;
    }

    if (input == 'q')
    {
        cursor.col--;

        if (cursor.col <= 0)
            cursor.col = 0;

        if (cursor == player || IsObstacle(cursor, obstacles))
            cursor.col++;
    }

    if (input == 'd')
    {
        cursor.col++;

        if (cursor.col >= WIDTH)
            cursor.col = WIDTH - 1;

        if (cursor == player || IsObstacle(cursor, obstacles))
            cursor.col--;
    }

    if (input == '\r')
        PathFinding(cursor, player, obstacles, true);
    else
        PathFinding(cursor, player, obstacles, false);
}

void DrawPath(GridCoord& posCurseur)
{
    Tile tile = tiles[posCurseur.row][posCurseur.col];

    while (true)
    {
        Tile* nextTile = tile.cameFrom;

        if (nextTile == nullptr)
            return;

        if (grid[nextTile->position.row][nextTile->position.col] != PLAYER)
            grid[nextTile->position.row][nextTile->position.col] = PATH;

        tile = *nextTile;
    }
}

bool FindPath(Tile* start, Tile* end)
{
    std::vector<Tile*> queue;
    queue.push_back(start);

    while (queue.empty() == false)
    {
        Tile* temp = queue[0];
        queue.erase(queue.begin());

        if (temp->position == end->position)
            return true; // chemin trouvé

        temp->isVisited = true;

        //ROmain
        std::vector<Tile*> neighbours;

        if (temp->position.col + 1 < HEIGHT)
            neighbours.push_back(&tiles[temp->position.row][temp->position.col + 1]);

        if (temp->position.col - 1 >= 0)
            neighbours.push_back(&tiles[temp->position.row][temp->position.col - 1]);

        if (temp->position.row + 1 < WIDTH)
            neighbours.push_back(&tiles[temp->position.row + 1][temp->position.col]);

        if (temp->position.row - 1 >= 0)
            neighbours.push_back(&tiles[temp->position.row - 1][temp->position.col]);
        //plus ROmain

        for (Tile* tile : neighbours)//boucle auto
        {
            if (tile->isVisited == false && tile->walkable)
            {
                queue.push_back(tile);
                tile->isVisited = true;
                tile->cameFrom = temp;
            }
        }
    }
    return false; //chemin non trouvé
}

void PathFinding(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles, bool move)
{
    ClearMap();
    ClearTiles();

    Tile* playerTile = new Tile();
    playerTile->position = player;

    Tile* cursorTile = new Tile();
    cursorTile->position = cursor;

    if (FindPath(playerTile, cursorTile))
    {
        DrawPath(cursor);

        if (move)//pas appellé
        {
            grid[player.row][player.col] = EMPTY;
            grid[cursor.row][cursor.col] = PLAYER;

            player = cursor;
        }

    }

}

int main()
{
    srand(time(NULL));


    GridData data;

    data.mPlayer = { HEIGHT - 1, WIDTH - 1 };
    data.mCursor = { 0, 0 };


    //obstacle
    for (size_t i = 0; i < OBSTACLE_NB; i++)
    {
        GridCoord obsCoord;

        int row = GenerateRandomNumber(1, WIDTH - 1);
        int col = GenerateRandomNumber(1, HEIGHT - 1);

        obsCoord = { row, col };

        data.obstacles.push_back(obsCoord);
    }


    bool open = true;

    InitGrid(data.mCursor, data.mPlayer, data.obstacles);

    UpdateGrid(data.mCursor, data.mPlayer, data.obstacles);

    while (open)
    {
        UpdateGrid(data.mCursor, data.mPlayer, data.obstacles);

        Move(data.mCursor, data.mPlayer, data.obstacles);
    }
}
