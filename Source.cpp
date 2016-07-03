#include <iostream>

#include <vector>
#include <array>

struct Variants
{
    bool canBe[9] = { false };
    bool solved = false;
    int count = 0;
};

bool inLine(const int(&map)[9][9], int curVal, int line)
{
    for (int i = 0; i < 9; i++)
    {
        if (curVal == map[line][i])
        {
            return true;
        }
    }
    return false;
}

bool inColumn(const int(&map)[9][9], int curVal, int column)
{
    for (int i = 0; i < 9; i++)
    {
        if (curVal == map[i][column])
        {
            return true;
        }
    }
    return false;
}

bool inSquare(const int(&map)[9][9], int curVal, int i, int j)
{
    int i_start = i - (i % 3);
    int j_start = j - (j % 3);
    for (int k = i_start; k < i_start + 3; k++)
    {
        for (int l = j_start; l < j_start + 3; l++)
        {
            if (curVal == map[k][l])
            {
                return true;
            }
        }
    }
    return false;
}

bool buildVariants( const int(&map)[9][9], Variants (&variants)[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            variants[i][j].count = 0;
            memset(variants[i][j].canBe, false, sizeof(bool) * 9);
            if (map[i][j] != 0)
            {
                variants[i][j].solved = true;
                continue;
            }
            for (int curVal = 1; curVal <= 9; curVal++)
            {
                if (!inLine(map, curVal, i) && !inColumn(map, curVal, j) && !inSquare(map, curVal, i, j))
                {
                    variants[i][j].canBe[curVal - 1] = true;
                    variants[i][j].count++;
                }
            }
            if (variants[i][j].count == 0 && variants[i][j].solved == false)
            {
                return false;
            }
        }
    }
    return true;
}

// проверка и изменение поля с условием, что есть хоть один однозначный вариант
bool checkTrivial( int(&map)[9][9], Variants(&variants)[9][9])
{
    bool leastOne = false;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (variants[i][j].count == 1)
            {
                variants[i][j].count = 0;
                for (int k = 1; k <= 9; k++)
                {
                    if (variants[i][j].canBe[k - 1] == true)
                    {
                        map[i][j] = k;
                        if (!buildVariants(map, variants))
                        {
                            return false;
                        }
                        leastOne = true;
                        break;
                    }
                }
            }
        }
    }
    return leastOne;
}

bool checkLines(int(&map)[9][9], Variants(&variants)[9][9])
{
    bool leastOne = false;
    for (int i = 0; i < 9; i++)
    {
        for (int elem = 1; elem <=9; elem++)
        {
            int elemCount = 0;
            int lastJpos = 0;
            for (int j = 0; j < 9; j++)
            {
                if (!(variants[i][j].solved) && variants[i][j].canBe[elem-1])
                {
                    elemCount++;
                    lastJpos = j;
                }
            }
            if (elemCount == 1)
            {
                map[i][lastJpos] = elem;
                if (!buildVariants(map, variants))
                {
                    return false;
                }
                leastOne = true;
            }
        }
    }
    return leastOne;
}

bool checkColumns(int(&map)[9][9], Variants(&variants)[9][9])
{
    bool leastOne = false;
    for (int j = 0; j < 9; j++)
    {
        for (int elem = 1; elem <= 9; elem++)
        {
            int elemCount = 0;
            int lastIpos = 0;
            for (int i = 0; i < 9; i++)
            {
                if (!(variants[i][j].solved) && variants[i][j].canBe[elem - 1])
                {
                    elemCount++;
                    lastIpos = i;
                }
            }
            if (elemCount == 1)
            {
                map[lastIpos][j] = elem;
                if (!buildVariants(map, variants))
                {
                    return false;
                }
                leastOne = true;
            }
        }
    }
    return leastOne;
}

bool solved(const int(&map)[9][9])
{
    for (auto &line : map)
    {
        for (auto &elem : line)
        {
            if (elem == 0)
            {
                return false;
            }
        }
    }
    return true;
}

//class Try
//{
//public:
//    setNewTry();
//    setNextTry();
//private:
//
//};
//
//typedef std::vector < std::array<int, 81> > TryVec;
//
//void setNewTry(const int(&map)[9][9], TryVec &vec)
//{
//    vec.emplace_back();
//    int *data = vec.back().data();
//    memcpy(data, map, sizeof(int) * 81);
//
//}

bool solve(int (&map)[9][9])
{
    Variants variants[9][9];
    //TryVec triesVector;
    do 
    {
        if (!buildVariants(map, variants))
        {
            //restoreLastTry(map);
            buildVariants(map, variants);
        }
        while (checkTrivial(map, variants) || checkLines(map, variants) || checkColumns(map, variants));
        if (solved(map))
        {
            return true;
        }
        //setNewTry(map, triesVector);
    } while (true);
    return false;
}

double f(double f);

int main (int ac, char **av)
{
    std::cout << powf(0.1, 0.1);
    int sudoku[9][9] = {
        { 0, 0, 0, 2, 0, 0, 5, 0, 0 },
        { 0, 6, 0, 0, 0, 3, 0, 1, 0 },
        { 0, 0, 9, 0, 0, 0, 0, 0, 7 },
        { 3, 0, 0, 7, 0, 0, 0, 0, 0 },
        { 0, 0, 5, 0, 0, 9, 0, 0, 6 },
        { 0, 4, 0, 0, 1, 0, 2, 0, 0 },
        { 7, 0, 0, 4, 0, 0, 8, 0, 0 },
        { 0, 2, 0, 0, 5, 0, 0, 9, 0 },
        { 0, 0, 1, 0, 0, 6, 0, 0, 0 }
    };
    solve(sudoku);
	return 0;
}