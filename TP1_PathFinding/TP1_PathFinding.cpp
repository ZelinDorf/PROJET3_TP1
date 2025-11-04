#include <iostream>
#include <vector>
#include <conio.h>

#define WIDTH 10
#define HEIGHT 10

// Codes ANSI pour les couleurs

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"


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
};
 

void InitGrid(std::vector<std::vector<const char*>>& grid, GridCoord& cursor, GridCoord& player)
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        std::vector<const char*> temp;

        for (size_t j = 0; j < WIDTH; j++)
        {
            GridCoord tempCoord{ i,j };

            if (tempCoord == cursor)
                temp.push_back("\033[31m|+|\033[0m");

            else if (tempCoord == player)
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

void UpdateGrid(std::vector<std::vector<const char*>>& grid, std::vector<GridCoord>& pathCoords, GridCoord& cursor, GridCoord& player)
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
        }
    }

    PrintGrid(grid);
}

void GoToCursor(std::vector<std::vector<const char*>>& grid, std::vector<GridCoord>& pathCoords, GridCoord& cursor, GridCoord& player);

void Move(std::vector<std::vector<const char*>>& grid, std::vector<GridCoord>& pathCoords, GridCoord& cursor, GridCoord& player)
{
    int input = _getch();
    GridData data{ player, cursor };

    if (input == 'z')
    {
        cursor.row--;

        if (cursor.row <= 0)
            cursor.row = 0;

        if (cursor == player)
            cursor.row++;
    }

    if (input == 's')
    {
        cursor.row++;

        if (cursor.row >= HEIGHT)
            cursor.row = HEIGHT - 1;

        if (cursor == player)
            cursor.row--;
    }

    if (input == 'q')
    {
        cursor.col--;

        if (cursor.col <= 0)
            cursor.col = 0;

        if (cursor == player)
            cursor.col++;
    }

    if (input == 'd')
    {
        cursor.col++;

        if (cursor.col >= WIDTH)
            cursor.col = WIDTH - 1;

        if (cursor == player)
            cursor.col--;
    }

    if (input == 'a')
        GoToCursor(grid, pathCoords, data.mCursor, data.mPlayer);

    if (input == 'e')
        player = cursor;
}

void GoToCursor(std::vector<std::vector<const char*>>& grid, std::vector<GridCoord>& pathCoords, GridCoord& cursor, GridCoord& player)
{
    int colDist = abs(player.col - cursor.col);
    int rowDist = abs(player.row - cursor.row);

    pathCoords.resize(0);  

    for (size_t i = 1; i <= colDist; i++)
    {
        GridCoord temp;

        if (player.col > cursor.col)
            temp = { player.row, (int)(player.col - i) };

        if (player.col < cursor.col)
            temp = { player.row, (int)(player.col + i) };

        pathCoords.push_back(temp);
    }

    for (size_t j = 1; j < rowDist; j++)
    {
        GridCoord temp;

        if (player.row > cursor.row)
            temp = { (int)(player.row - j), pathCoords.back().col };

        if (player.row < cursor.row)
            temp = { (int)(player.row + j), pathCoords.back().col };

        pathCoords.push_back(temp);
    }

    UpdateGrid(grid, pathCoords, cursor, player);
}

int main()
{
    GridData data;

    data.mPlayer = { HEIGHT - 1, WIDTH - 1 };
    data.mCursor = { 0, 0 };

    std::vector<std::vector<const char*>> vGrid;

    std::vector<GridCoord> pathCoords;

    bool open = true;

    InitGrid(vGrid, data.mCursor, data.mPlayer);
    UpdateGrid(vGrid, pathCoords, data.mCursor, data.mPlayer);

    while (open)
    {
        UpdateGrid(vGrid, pathCoords, data.mCursor, data.mPlayer);

        Move(vGrid, pathCoords, data.mCursor, data.mPlayer);
    }
}
