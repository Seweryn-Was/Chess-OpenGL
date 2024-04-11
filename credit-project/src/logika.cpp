#include<iostream>
#include "logika.h"

/*
1, 7  = pawn
2, 8  = rook
3, 9  = knight
4, 10 = bishop
5, 11 = queen
6, 12 = king
*/



Allowed(*chessboard)[8];
int allowed[8][8];
int without_kings[8][8];
int white_pieces_attacks[8][8]{};
int black_pieces_attacks[8][8]{};
int blocked_pieces[8][8];

//int main()
//{
//    chessboard = new_chessgame();
//    //everywhere_zeros(allowed);
//    int y{}, x{};
//    without(chessboard, without_kings);
//    //chessboard[3][6] = 9;
//    //chessboard[0][6] = 8;
//    //piece(6, 4, allowed);
//    //show_chessboard(chessboard);
//    //show_chessboard(white_pieces);
//    block_pieces(4, 3);
//    //show_chessboard(blocked_pieces);
//    show_chessboard(without_kings);
//    return 0;
//}

void all_possible_attacks(int chessboard[][8])
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (chessboard[i][j] < 7 && chessboard[i][j] != 0)
                piece(i, j, white_pieces_attacks);
            if (chessboard[i][j] >= 7)
            {
                piece(i, j, black_pieces_attacks);
            }
        }
}

void without(Allowed chessboard[][8], int without_kings[][8])
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (chessboard[i][j].key != 6 && chessboard[i][j].key != 12)
                without_kings[i][j] = chessboard[i][j].key;
            else without_kings[i][j] = 0;
        }
}

void show_chessboard(Allowed chessboard[][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            std::cout << chessboard[i][j].key << ' ';
        std::cout << '\n';
    }
}

void show_chessboard(int chessboard[][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            std::cout << chessboard[i][j] << ' ';
        std::cout << '\n';
    }
}

Allowed(*(new_chessgame)())[8]
    {
        static Allowed fresh_chessboard[8][8] = {
            {2, 3, 4, 5, 6 ,4, 3, 2},
            {1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 8, 0, 0, 0, 0},
            {0, 0, 0, 2, 0, 0, 0, 0},
            {0, 8, 2, 6, 2, 8, 0, 0},
            {0, 0, 0, 2, 4, 0, 0, 0},
            {7, 7, 7, 8, 7, 0, 7, 7},
            {8, 9, 10, 12, 11 ,10, 10, 8}
        };
        return fresh_chessboard;
    }

    void block_pieces(int y, int x)
    {
        int change{ 6 };
        if (chessboard[y][x].key >= 7)
            change = 0;
        int i = 0;
        int how_many = 0;
        int ypom = 0, xpom = 0;
        for (int j = -1; j <= 1; ++j)
            for (int k = -1; k <= 1; ++k)
            {
                int first{}, second{};
                if (k == 0 && j == 0)
                    continue;
                if (j * k == 0)
                {
                    first = 2;
                    second = 5;
                }
                else
                {
                    first = 4;
                    second = 5;
                }
                while (y + i * j < 8 && x + i * k < 8 && y + i * j >= 0 && x + i * k >= 0)
                {
                    int new_y = y + i * j;
                    int new_x = x + i * k;
                    if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                    {
                        ypom = new_y;
                        xpom = new_x;
                        ++how_many;
                    }
                    if (how_many == 2)
                        break;
                    if (chessboard[new_y][new_x].key == first + change || chessboard[new_y][new_x].key == second + change)
                        if (how_many == 1)
                        {
                            if (chessboard[ypom][xpom].key == first + 6 - change || chessboard[ypom][xpom].key == second + 6 - change)
                            {
                                i = 1;
                                everywhere_zeros(chessboard[ypom][xpom].allowed);
                                while (ypom + i <= 7 && xpom + i <= 7)
                                {
                                    new_y = ypom + i * j;
                                    new_x = xpom + i * k;
                                    if (chessboard[new_y][new_x].key == first + change || chessboard[new_y][new_x].key == second + change)
                                    {
                                        chessboard[ypom][xpom].allowed[new_y][new_x] = 1;
                                        break;
                                    }
                                    chessboard[ypom][xpom].allowed[new_y][new_x] = 1;
                                    ++i;
                                }
                                break;
                            }
                            blocked_pieces[ypom][xpom] = 1;
                            break;
                        }
                    if (chessboard[new_y][new_x].key >= 7)
                        break;
                    i++;
                }
                i = 1;
                ypom = 0;
                xpom = 0;
                how_many = 0;
            }
    }

    void everywhere_zeros(int allowed[][8])
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                allowed[i][j] = 0;
    }

    void piece(int y, int x, int allowed[][8])
    {
        switch (chessboard[y][x].key)
        {
        case 1:
            pawn(y, x, allowed);
            break;
        case 7:
            pawn(y, x, allowed);
            break;
        case 2:
            rook(y, x, allowed);
            break;
        case 8:
            rook(y, x, allowed);
            break;
        case 3:
            knight(y, x, allowed);
            break;
        case 9:
            knight(y, x, allowed);
            break;
        case 4:
            bishop(y, x, allowed);
            break;
        case 10:
            bishop(y, x, allowed);
            break;
        case 5:
            queen(y, x, allowed);
            break;
        case 11:
            queen(y, x, allowed);
            break;
        case 6:
            king(y, x, allowed);
            break;
        case 12:
            king(y, x, allowed);
            break;
        default:
            break;
        }
    }

    void pawn(int y, int x, int allowed[][8])
    {
        if (y == 1 && chessboard[y][x].key == 1)
        {
            for (int i = 1; i <= 2; i++)
                if (chessboard[y + i][x].key == 0)
                    allowed[y + i][x] = 1;
                else break;
            if (x - 1 >= 0)
                if (chessboard[y + 1][x - 1].key >= 7)
                    allowed[y + 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y + 1][x + 1].key >= 7)
                    allowed[y + 1][x + 1] = 1;
            return;
        }
        if (y == 6 && chessboard[y][x].key == 7)
        {
            for (int i = 1; i <= 2; i++)
                if (chessboard[y - i][x].key == 0)
                    allowed[y - i][x] = 1;
                else break;
            if (x - 1 >= 0)
                if (chessboard[y - 1][x - 1].key <= 6 && chessboard[y - 1][x - 1].key != 0)
                    allowed[y - 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y - 1][x - 1].key <= 6 && chessboard[y - 1][x - 1].key != 0)
                    allowed[y - 1][x - 1] = 1;
            return;
        }
        if (chessboard[y][x].key == 7)
        {
            if (chessboard[y - 1][x].key == 0)
                allowed[y - 1][x] = 1;
            if (x - 1 >= 0)
                if (chessboard[y - 1][x - 1].key <= 6 && chessboard[y - 1][x - 1].key != 0)
                    allowed[y - 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y - 1][x - 1].key <= 6 && chessboard[y - 1][x - 1].key != 0)
                    allowed[y - 1][x - 1] = 1;
            return;
        }
        if (chessboard[y][x].key == 1)
        {
            if (chessboard[y + 1][x].key == 0)
                allowed[y + 1][x] = 1;
            if (x - 1 >= 0)
                if (chessboard[y + 1][x - 1].key >= 7)
                    allowed[y + 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y + 1][x + 1].key >= 7)
                    allowed[y + 1][x + 1] = 1;
            return;
        }
    }

    void bishop(int y, int x, int allowed[][8])
    {
        int i = 1;
        if (chessboard[y][x].key < 7)
        {
            while (y + i < 8 && x + i < 8)
            {
                int new_y = y + i;
                int new_x = x + i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                    break;
                i++;
            }
            i = 1;
            while (y - i >= 0 && x + i < 8)
            {
                int new_y = y - i;
                int new_x = x + i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                    break;
                i++;
            }
            i = 1;
            while (y + i < 8 && x - i >= 0)
            {
                int new_y = y + i;
                int new_x = x - i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                    break;
                i++;
            }
            i = 1;
            while (y - i >= 0 && x - i >= 0)
            {
                int new_y = y - i;
                int new_x = x - i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                    break;
                i++;
            }
        }
        if (chessboard[y][x].key >= 7)
        {
            i = 1;
            while (y + i < 8 && x + i < 8)
            {
                int new_y = y + i;
                int new_x = x + i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                    break;
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y - i >= 0 && x + i < 8)
            {
                int new_y = y - i;
                int new_x = x + i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                    break;
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y + i < 8 && x - i >= 0)
            {
                int new_y = y + i;
                int new_x = x - i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                    break;
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y - i >= 0 && x - i >= 0)
            {
                int new_y = y - i;
                int new_x = x - i;
                if (chessboard[new_y][new_x].key == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x].key >= 7)
                    break;
                if (chessboard[new_y][new_x].key < 7 && chessboard[new_y][new_x].key != 0)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                i++;
            }
        }
    }

    void rook(int y, int x, int allowed[][8])
    {
        if (chessboard[y][x].key < 7)
        {
            int i = 1;
            while (x - i >= 0)
            {
                int new_x = x - i;
                if (chessboard[y][new_x].key == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x].key < 7 && chessboard[y][new_x].key != 0)
                    break;
                if (chessboard[y][new_x].key >= 7)
                {
                    allowed[y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (x + i < 8)
            {
                int new_x = x + i;
                if (chessboard[y][new_x].key == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x].key < 7 && chessboard[y][new_x].key != 0)
                    break;
                if (chessboard[y][new_x].key >= 7)
                {
                    allowed[y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y - i >= 0)
            {
                int new_y = y - i;
                if (chessboard[new_y][x].key == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x].key < 7 && chessboard[new_y][x].key != 0)
                    break;
                if (chessboard[new_y][x].key >= 7)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y + i < 8)
            {
                int new_y = y + i;
                if (chessboard[new_y][x].key == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x].key < 7 && chessboard[new_y][x].key != 0)
                    break;
                if (chessboard[new_y][x].key >= 7)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
        }
        if (chessboard[y][x].key >= 7)
        {
            int i = 1;
            while (x - i >= 0)
            {
                int new_x = x - i;
                if (chessboard[y][new_x].key == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x].key >= 7)
                    break;
                if (chessboard[y][new_x].key < 7 && chessboard[y][new_x].key != 0)
                {
                    allowed[y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (x + i < 8)
            {
                int new_x = x + i;
                if (chessboard[y][new_x].key == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x].key >= 7)
                    break;
                if (chessboard[y][new_x].key < 7 && chessboard[y][new_x].key != 0)
                {
                    allowed[y][new_x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y - i >= 0)
            {
                int new_y = y - i;
                if (chessboard[new_y][x].key == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x].key >= 7)
                    break;
                if (chessboard[new_y][x].key < 7 && chessboard[new_y][x].key != 0)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            while (y + i < 8)
            {
                int new_y = y + i;
                if (chessboard[new_y][x].key == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x].key >= 7)
                    break;
                if (chessboard[new_y][x].key < 7 && chessboard[new_y][x].key != 0)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
        }
    }

    void queen(int y, int x, int allowed[][8])
    {
        bishop(y, x, allowed);
        rook(y, x, allowed);
    }

    void king(int y, int x, int allowed[][8])
    {
        if (chessboard[y][x].key == 6)
        {
            for (int i = -1; i < 2; ++i)
                for (int j = -1; j < 2; ++j)
                {
                    if (y + i < 0 || y + i >= 8 || x + j < 0 || x + j >= 8)
                        continue;
                    if (chessboard[y + i][x + j].key == 6)
                        continue;
                    if (chessboard[y + i][x + j].key == 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                    if (chessboard[y + i][x + j].key >= 7)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                }
        }
        if (chessboard[y][x].key == 12)
        {
            for (int i = -1; i < 2; ++i)
                for (int j = -1; j < 2; ++j)
                {
                    if (y + i < 0 || y + i >= 8 || x + j < 0 || x + j >= 8)
                        continue;
                    if (chessboard[y + i][x + j].key == 12)
                        continue;
                    if (chessboard[y + i][x + j].key < 7 && chessboard[y + i][x + j].key != 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                    if (chessboard[y + i][x + j].key == 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                }
        }
    }

    void knight(int y, int x, int allowed[][8])
    {
        if (chessboard[y][x].key == 3)
        {
            for (int i = -2; i <= 2; i += 4)
            {
                int new_x = x + i;
                int new_y = y + i;
                for (int j = -1; j <= 1; j += 2)
                {
                    int new_x2 = x + j;
                    int new_y2 = y + j;
                    if (new_x >= 0 && new_x < 8 && new_y2 >= 0 && new_y2 < 8)
                    {
                        if (chessboard[new_y2][new_x].key >= 7)
                            allowed[new_y2][new_x] = 1;
                        if (chessboard[new_y2][new_x].key == 0)
                            allowed[new_y2][new_x] = 1;
                    }
                    if (new_y >= 0 && new_y < 8 && new_x2 >= 0 && new_x2 < 8)
                    {
                        if (chessboard[new_y][new_x2].key >= 7)
                            allowed[new_y][new_x2] = 1;
                        if (chessboard[new_y][new_x2].key == 0)
                            allowed[new_y][new_x2] = 1;
                    }
                }
            }
        }

        if (chessboard[y][x].key == 9)
        {
            for (int i = -2; i <= 2; i += 4)
            {
                int new_x = x + i;
                int new_y = y + i;
                for (int j = -1; j <= 1; j += 2)
                {
                    int new_x2 = x + j;
                    int new_y2 = y + j;
                    if (new_x >= 0 && new_x < 8 && new_y2 >= 0 && new_y2 < 8)
                    {
                        if (chessboard[new_y2][new_x].key < 7 && chessboard[new_y2][new_x].key != 0)
                            allowed[new_y2][new_x] = 1;
                        if (chessboard[new_y2][new_x].key == 0)
                            allowed[new_y2][new_x] = 1;
                    }
                    if (new_y >= 0 && new_y < 8 && new_x2 >= 0 && new_x2 < 8)
                    {
                        if (chessboard[new_y][new_x2].key < 7 && chessboard[new_y][new_x2].key != 0)
                            allowed[new_y][new_x2] = 1;
                        if (chessboard[new_y][new_x2].key == 0)
                            allowed[new_y][new_x2] = 1;
                    }
                }
            }
        }
    }