
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <array>

#include <cstdio>

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
        cacheVector.emplace_back();
        int *data = cacheVector.back().data();
        memcpy(data, map, sizeof(int) * 81);
        int best_i = 0;
        int best_j = 0;
        int best_count = 9;
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
        guessVec.emplace_back();
        guessVec.back().i = best_i;
        guessVec.back().j = best_j;
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
        do
        {
            const int *data = cacheVector.back().data();
            memcpy(map, data, sizeof(int) * 81);
            buildVariants(map, variants);
            ++guessVec.back().number;
            if (guessVec.back().number < variants[guessVec.back().i][guessVec.back().j].count)
            {
                break;
            }
            else
            {
                cacheVector.pop_back();
                guessVec.pop_back();
            }

        } while (true);
        int num = guessVec.back().number;
        for (int i = 0; i < 9; i++)
        {
            if (variants[guessVec.back().i][guessVec.back().j].canBe[i] && (num-- == 0))
            {
                map[guessVec.back().i][guessVec.back().j] = i + 1;
                break;
            }
        }
        buildVariants(map, variants);
    }
private:

    struct Guess
    {
        int i;
        int j;
        int number;
    };

    typedef std::vector < std::array<int, 81> > CacheVec;
    typedef std::vector < Guess > GuessVec;
    CacheVec cacheVector;
    GuessVec guessVec;
    
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
    FILE *f = fopen("testtest.txt", "rb");
    int sudoku[9][9];
    int *data = (int*)sudoku;
    char buf[81];
    while (!feof(f))
    {
        fread(buf, 1, 81, f);
        for (int i = 0; i < 81; i++)
        {
            data[i] = buf[i] - '0';
        }
        solve(sudoku);
    }
    fclose(f);
	return 0;
}
