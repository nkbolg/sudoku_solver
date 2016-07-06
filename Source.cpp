#include <iostream>

#include <vector>
#include <array>

struct Variants
{
    // todo: возможно, переписать через хранение только возможных вариантов
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

//TODO: переписать через dirty
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

class Try
{   
public:
    void setNewTry(int(&map)[9][9], Variants(&variants)[9][9])
    {
        saveState(map);
        int best_i = 0;
        int best_j = 0;
        int best_count = variants[0][0].count;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (!variants[i][j].solved && variants[i][j].count < best_count)
                {
                    best_i = i;
                    best_j = j;
                    best_count = variants[i][j].count;
                }
            }
        }
        currentGuess = { best_i, best_j, 0 };
        for (int i = 0; i < 9; i++)
        {
            if (variants[best_i][best_j].canBe[i])
            {
                map[best_i][best_j] = i + 1;
                break;
            }
        }
        buildVariants(map, variants);
    }
    void setNextTry(int(&map)[9][9], Variants(&variants)[9][9])
    {
        loadState(map);
        int num = ++currentGuess.number;
        for (int i = 0; i < 9; i++)
        {
            if (variants[currentGuess.i][currentGuess.j].canBe[i] && (num-- == 0))
            {
                map[currentGuess.i][currentGuess.j] = i + 1;
                break;
            }
        }
        buildVariants(map, variants);
    }
private:

    void saveState(const int(&map)[9][9])
    {
        cacheVector.emplace_back();
        int *data = cacheVector.back().data();
        memcpy(data, map, sizeof(int) * 81);
    }

    void loadState(int(&map)[9][9])
    {
        const int *data = cacheVector.back().data();
        memcpy(map, data, sizeof(int) * 81);
        cacheVector.pop_back();
    }

    typedef std::vector < std::array<int, 81> > CacheVec;
    CacheVec cacheVector;
    struct 
    {
        int i;
        int j;
        int number;
    } currentGuess;
};

bool solve(int (&map)[9][9])
{
    Variants variants[9][9];
    Try m_try;
    do 
    {
        if (!buildVariants(map, variants))
        {
            m_try.setNextTry(map, variants);
        }
        while (checkTrivial(map, variants) || checkLines(map, variants) || checkColumns(map, variants));
        if (solved(map))
        {
            return true;
        }
        m_try.setNewTry(map, variants);
    } while (true);
    return false;
}

int main ()
{
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
