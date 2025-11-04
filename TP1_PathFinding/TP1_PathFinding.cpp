#include <iostream>
#include <vector>
#include <conio.h>

#define WIDTH 15
#define HEIGHT 10

// Codes ANSI pour les couleurs

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"



struct GridCoord
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
};
 

void InitGrid(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player)
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        std::vector<const char*> temp;

        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            if (tempCoord == data.mCursor)
                temp.push_back("\033[31m|+|\033[0m");

            else if (tempCoord == data.mPlayer)
                temp.push_back("\033[35m|O|\033[0m");
            else
                temp.push_back("| |");
        }

        grid.push_back(temp);
    }
}

void PrintGrid(std::vector<std::vector<const char*>>& grid)
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

void UpdateGrid(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player)
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
        }

    }

    PrintGrid(grid);
}

void GoToCursor(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player);

void Move(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player)
{
    int input = _getch();
    GridData data{ player, cursor };

    if (input == 'z')
    {
        cursor.row--;

        if (cursor.row <= 0)
            cursor.row = 0;
    }

    if (input == 's')
    {
        cursor.row++;

        if (cursor.row >= HEIGHT)
            cursor.row = HEIGHT - 1;
    }

    if (input == 'q')
    {
        cursor.col--;

        if (cursor.col <= 0)
            cursor.col = 0;
    }

    if (input == 'd')
    {
        cursor.col++;

        if (cursor.col >= WIDTH)
            cursor.col = WIDTH - 1;
    }

    if (input == 'a')
        GoToCursor(grid, data.mCursor, data.mPlayer);

    
    UpdateGrid(grid, data.mCursor, data.mPlayer);
}

void GoToCursor(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player)
{
    int colDist = abs(player.col - cursor.col);
    int rowDist = abs(player.row - cursor.row);

    for (size_t i = player.col; i < colDist; i++)
    {
        for (size_t i = player.row; i < rowDist; i++)
        {

        }
    }
}

int main()
{
    GridData data;

    data.mPlayer = { HEIGHT - 1, WIDTH - 1 };
    data.mCursor = { 0, 0 };

    std::vector<std::vector<const char*>> vGrid;

    bool open = true;

    InitGrid(vGrid, data.mCursor, data.mPlayer);
    UpdateGrid(vGrid, data.mCursor, data.mPlayer);

    while (open)
    {
        Move(vGrid, data.mCursor, data.mPlayer);
    }
}
