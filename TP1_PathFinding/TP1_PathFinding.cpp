#include <iostream>
#include <vector>
#include <conio.h>
#include "stdlib.h"


#define WIDTH 10
#define HEIGHT 10

// Codes ANSI pour les couleurs

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"


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
 

std::vector<std::vector<const char*>> grid;

std::vector<GridCoord> pathCoords;



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

        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            if (tempCoord == cursor)
                temp.push_back("\033[31m|+|\033[0m");
            else if (IsObstacle(tempCoord, obstacles))
                temp.push_back("\033[34m|#|\033[0m");
            else if (tempCoord == player)
                temp.push_back("\033[35m|O|\033[0m");
            else
                temp.push_back("| |");
        }

        grid.push_back(temp);
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

void UpdateGrid(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    system("cls");

    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            if (tempCoord == cursor)
                grid[i][j] = "\033[31m|+|\033[0m"; 
            else if (tempCoord == player)
                grid[i][j] = "\033[35m|O|\033[0m";
            else
                grid[i][j] = "| |";

            for (GridCoord coords : pathCoords)
            {
                if (tempCoord == coords)
                    grid[i][j] = "\033[32m|P|\033[0m";
            }
            
            if (IsObstacle(tempCoord, obstacles))
                grid[i][j] = "\033[34m|#|\033[0m";
        }
    }

    PrintGrid();
}

void PathFinding(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles);

void Move(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    int input = _getch();
    GridData data{ player, cursor , obstacles};

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

    if (input == 'a')
        PathFinding(data.mCursor, data.mPlayer, data.obstacles);

    if (input == 'e')
        player = cursor;
}

void ObstacleAvoiding(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    for (GridCoord co : obstacles)
    {
        
    }
}

void PathFinding(GridCoord& cursor, GridCoord& player, std::vector<GridCoord>& obstacles)
{
    int colDist = abs(player.col - cursor.col);
    int rowDist = abs(player.row - cursor.row);

    pathCoords.resize(0);  

    for (size_t i = 1; i <= colDist; i++)
    {
        GridCoord temp{ 0, 0 };

        if (player.col > cursor.col)
            temp = { player.row, (int)(player.col - i) };

        if (player.col < cursor.col)
            temp = { player.row, (int)(player.col + i) };

        if (IsObstacle(temp, obstacles))
            ObstacleAvoiding(cursor, player, obstacles);
        else
            pathCoords.push_back(temp);

    }

    for (size_t j = 1; j < rowDist; j++)
    {
        GridCoord temp{ 0, 0 };

        if (pathCoords.size() > 0)
        {
            if (player.row > cursor.row)
                temp = { (int)(player.row - j), pathCoords.back().col };

            if (player.row < cursor.row)
                temp = { (int)(player.row + j), pathCoords.back().col };
        }
        else
        {
            if (player.row > cursor.row)
                temp = { (int)(player.row - j), player.col };

            if (player.row < cursor.row)
                temp = { (int)(player.row + j), player.col };
        }


        if (IsObstacle(temp, obstacles))
            ObstacleAvoiding(cursor, player, obstacles);
        else
            pathCoords.push_back(temp);
    }

    UpdateGrid(cursor, player, obstacles);
}

int main()
{
    srand(time(NULL));


    GridData data;

    data.mPlayer = { HEIGHT - 1, WIDTH - 1 };
    data.mCursor = { 0, 0 };

    for (size_t i = 0; i < WIDTH; i++)
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
