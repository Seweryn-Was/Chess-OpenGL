#include<iostream>
#include<algorithm>
/*
1, 7  = pawn
2, 8  = rook
3, 9  = knight
4, 10 = bishop
5, 11 = queen
6, 12 = king
*/

struct Allowed
{
    int key;
    int allowed[8][8];

    Allowed(int key)
        : key(key)
    {
        this->key = key;
    }
};

Allowed(*chessboard)[8];
int chessboard_int[8][8];
int allowed[8][8];
int without_kings[8][8];
int white_pieces_attacks[8][8]{};
int black_pieces_attacks[8][8]{};
int holy_land_black[8][8], holy_land_white[8][8], black_king[2], white_king[2];
bool lost_black{ false }, lost_white{ false }, black_downfall{ false }, white_downfall{ false };

void show_chessboard(Allowed chessboard[][8]);
void show_chessboard(int chessboard[][8]);
Allowed(*(new_chessgame)())[8];
void without(Allowed chessboard[][8], int without_kings[][8], int kolor);
void piece(int y, int x, int allowed[][8], int chessboard[][8]);
void pawn(int y, int x, int allowed[][8], int chessboard[][8]);
void bishop(int y, int x, int allowed[][8], int chessboard[][8]);
void rook(int y, int x, int allowed[][8], int chessboard[][8]);
void queen(int y, int x, int allowed[][8], int chessboard[][8]);
void king(int y, int x, int allowed[][8], int chessboard[][8]);
void knight(int y, int x, int allowed[][8], int chessboard[][8]);
void everywhere_zeros(int allowed[][8]);
void all_possible_attacks(int chessboard[][8], int kolor);
void block_pieces(int y, int x);
void pawn_attack(int y, int x, int allowed[][8]);
void int_chessgame(Allowed chessboard[][8]);
void szachen_machen(int y, int x, int color, bool& lost, bool& is_downfall, int holy_land[][8]);
void all_possible_moves(Allowed chessboard[][8]);
bool is_move_allowed(int y_start, int x_start, int y_end, int x_end);

bool is_move_allowed(int y_start, int x_start, int y_end, int x_end)
{
    Allowed temp{ 0 };
    everywhere_zeros(holy_land_black);
    everywhere_zeros(holy_land_white);
    black_downfall = false;
    white_downfall = false;
    all_possible_moves(chessboard);
    if (chessboard[y_start][x_start].key >= 7)
    {
        block_pieces(black_king[0], black_king[1]);
        if (chessboard[y_start][x_start].key == 12)
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1)
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(white_king[0], white_king[1], 1, lost_white, white_downfall, holy_land_white);
                return true;
            }
            else
            {
                return false;
            }
        }
        if (black_downfall)
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1 && holy_land_black[y_end][x_start])
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(white_king[0], white_king[1], 1, lost_white, white_downfall, holy_land_white);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1)
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(white_king[0], white_king[1], 1, lost_white, white_downfall, holy_land_white);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    if (chessboard[y_start][x_start].key < 7 && chessboard[y_start][x_start].key != 0)
    {
        block_pieces(white_king[0], white_king[1]);
        if (chessboard[y_start][x_start].key == 6)
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1)
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(white_king[0], white_king[1], 1, lost_white, white_downfall, holy_land_white);
                return true;
            }
            else
            {
                return false;
            }
        }
        if (white_downfall)
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1 && holy_land_white[y_end][x_start])
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(black_king[0], black_king[1], 1, lost_black, black_downfall, holy_land_black);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (chessboard[y_start][x_start].allowed[y_end][x_start] == 1)
            {
                temp = chessboard[y_start][x_start];
                chessboard[y_start][x_start] = chessboard[y_end][x_end];
                chessboard[y_end][x_end] = temp;
                szachen_machen(black_king[0], black_king[1], 1, lost_black, black_downfall, holy_land_black);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

void all_possible_attacks(int chessboard[][8], int kolor)
{
    if (kolor == 2)
        everywhere_zeros(white_pieces_attacks);
    if (kolor == 1)
        everywhere_zeros(black_pieces_attacks);
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (chessboard[i][j] == 1 && kolor == 2)
            {
                white_pieces_attacks[i][j] = 1;
                pawn_attack(i, j, white_pieces_attacks);
                continue;
            }
            if (chessboard[i][j] == 7 && kolor == 1)
            {
                black_pieces_attacks[i][j] = 1;
                pawn_attack(i, j, black_pieces_attacks);
                continue;
            }
            if (chessboard[i][j] < 7 && chessboard[i][j] != 0 && kolor == 2)
            {
                white_pieces_attacks[i][j] = 1;
                piece(i, j, white_pieces_attacks, chessboard);
            }
            if (chessboard[i][j] >= 7 && kolor == 1)
            {
                black_pieces_attacks[i][j] = 1;
                piece(i, j, black_pieces_attacks, chessboard);
            }
        }
}

void int_chessgame(Allowed chessboard[][8])
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            chessboard_int[i][j] = chessboard[i][j].key;
        }
    }
}

void all_possible_moves(Allowed chessboard[][8])
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (chessboard[i][j].key < 7 && chessboard[i][j].key != 0)
            {
                everywhere_zeros(allowed);
                piece(i, j, chessboard[i][j].allowed, chessboard_int);
            }
            if (chessboard[i][j].key >= 7)
            {
                everywhere_zeros(allowed);
                piece(i, j, black_pieces_attacks, chessboard_int);
            }
        }
}

void without(Allowed chessboard[][8], int without_kings[][8], int kolor)
{

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if ((kolor == 2 || chessboard[i][j].key != 6) && (chessboard[i][j].key != 12 || kolor == 1))
                without_kings[i][j] = chessboard[i][j].key;
            else without_kings[i][j] = 0;
        }
}

void szachen_machen(int y, int x, int color, bool& lost, bool& is_downfall, int holy_land[][8])
{
    everywhere_zeros(holy_land);
    without(chessboard, without_kings, color);
    all_possible_attacks(without_kings, color);

    int_chessgame(chessboard);

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            piece(i, j, chessboard[i][j].allowed, chessboard_int);
        }
    }

    block_pieces(y, x);

    lost = true;

    int temp[8][8];
    int relativity{};
    int temp1;

    everywhere_zeros(holy_land);

    if (color == 1 && black_pieces_attacks[y][x] == 1)
    {
        is_downfall = true;
    }
    else if (color == 2 && white_pieces_attacks[y][x] == 1)
    {
        relativity = 6;
        is_downfall = true;
    }

    if (is_downfall)
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if ((chessboard[i][j].key < 7 || chessboard[i][j].key == 0) && white_pieces_attacks[i][j] == 1 && color == 2)
                {
                    std::copy(&white_pieces_attacks[0][0], &white_pieces_attacks[0][0] + 8 * 8, &temp[0][0]);
                    temp1 = chessboard[i][j].key;
                    chessboard[i][j].key = 7;
                    without(chessboard, without_kings, color);
                    all_possible_attacks(without_kings, color);
                    if (white_pieces_attacks[y][x] == 0)
                    {
                        holy_land[i][j] == 1;
                    }
                    std::copy(&temp[0][0], &temp[0][0] + 8 * 8, &white_pieces_attacks[0][0]);
                    chessboard[i][j].key = temp1;
                    without(chessboard, without_kings, color);
                }
                if ((chessboard[i][j].key >= 7 || chessboard[i][j].key == 0) && black_pieces_attacks[i][j] == 1 && color == 1)
                {
                    std::copy(&black_pieces_attacks[0][0], &black_pieces_attacks[0][0] + 8 * 8, &temp[0][0]);
                    temp1 = chessboard[i][j].key;
                    chessboard[i][j].key = 1;
                    without(chessboard, without_kings, color);
                    all_possible_attacks(without_kings, color);
                    if (black_pieces_attacks[y][x] == 0)
                    {
                        holy_land[i][j] = 1;
                    }
                    std::copy(&temp[0][0], &temp[0][0] + 8 * 8, &black_pieces_attacks[0][0]);
                    chessboard[i][j].key = temp1;
                    without(chessboard, without_kings, color);
                }
            }
        }
        without(chessboard, without_kings, color);
        all_possible_attacks(without_kings, color);
    }
    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            if (y + i < 0 || y + i >= 8 || x + j < 0 || x + j >= 8)
                continue;
            if (chessboard[y + i][x + j].key == 6 || chessboard[y + i][x + j].key == 12)
                continue;
            if ((chessboard[y + i][x + j].key >= 7 || chessboard[y + i][x + j].key == 0) && color == 1)
            {
                std::copy(&black_pieces_attacks[0][0], &black_pieces_attacks[0][0] + 8 * 8, &temp[0][0]);
                temp1 = chessboard[y + i][x + j].key;
                chessboard[y + i][x + j].key = 1;
                without(chessboard, without_kings, color);
                all_possible_attacks(without_kings, color);
                std::cout << '\n';
                show_chessboard(without_kings);
                if (black_pieces_attacks[y + i][x + j] == 0)
                {
                    lost = false;
                    chessboard[y][x].allowed[y + i][x + j] = 1;
                }
                std::copy(&temp[0][0], &temp[0][0] + 8 * 8, &black_pieces_attacks[0][0]);
                chessboard[y + i][x + j].key = temp1;
                without(chessboard, without_kings, color);
            }
            if ((chessboard[y + i][x + j].key < 7 || chessboard[y + i][x + j].key == 0) && color == 2)
            {
                std::copy(&white_pieces_attacks[0][0], &white_pieces_attacks[0][0] + 8 * 8, &temp[0][0]);
                temp1 = chessboard[y + i][x + j].key;
                chessboard[y + i][x + j].key = 7;
                without(chessboard, without_kings, color);
                all_possible_attacks(without_kings, color);
                std::cout << '\n';
                show_chessboard(without_kings);
                if (white_pieces_attacks[y + i][x + j] == 0)
                {
                    lost = false;
                    chessboard[y][x].allowed[y + i][x + j] = 1;
                }
                std::copy(&temp[0][0], &temp[0][0] + 8 * 8, &white_pieces_attacks[0][0]);
                chessboard[y + i][x + j].key = temp1;
                without(chessboard, without_kings, color);
            }
        }
    }

    if (lost)
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (holy_land[i][j] == 1)
                {
                    for (int a = 0; a < 8; ++a)
                    {
                        for (int b = 0; b < 8; ++b)
                        {
                            if (chessboard[a][b].key != 0 && chessboard[a][b].allowed[i][j] == 1 && color == 1 && chessboard[a][b].key < 7)
                            {
                                lost = false;
                            }
                            if (chessboard[a][b].allowed[i][j] == 1 && color == 2 && chessboard[a][b].key >= 7)
                            {
                                lost = false;
                            }
                            if (!lost) {
                                break;
                            }
                        }
                        if (!lost) {
                            break;
                        }
                    }
                }
                if (!lost) {
                    break;
                }
            }
            if (!lost) {
                break;
            }
        }
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
            {1, 1, 1, 0, 5, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 6, 0, 0, 0, 0},
            {0, 0, 0, 5, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {7, 7, 7, 0, 11, 0, 7, 7},
            {8, 9, 10, 11, 11 ,10, 8, 8}
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
                                while (ypom + i * j <= 7 && xpom + i * k <= 7)
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
                            everywhere_zeros(chessboard[ypom][xpom].allowed);
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

    void piece(int y, int x, int allowed[][8], int chessboard[][8])
    {
        switch (chessboard[y][x])
        {
        case 1:
            pawn(y, x, allowed, chessboard);
            break;
        case 7:
            pawn(y, x, allowed, chessboard);
            break;
        case 2:
            rook(y, x, allowed, chessboard);
            break;
        case 8:
            rook(y, x, allowed, chessboard);
            break;
        case 3:
            knight(y, x, allowed, chessboard);
            break;
        case 9:
            knight(y, x, allowed, chessboard);
            break;
        case 4:
            bishop(y, x, allowed, chessboard);
            break;
        case 10:
            bishop(y, x, allowed, chessboard);
            break;
        case 5:
            queen(y, x, allowed, chessboard);
            break;
        case 11:
            queen(y, x, allowed, chessboard);
            break;
        case 6:
            white_king[0] = y;
            white_king[1] = x;
            szachen_machen(y, x, 1, lost_white, white_downfall, holy_land_white);
            break;
        case 12:
            black_king[0] = y;
            black_king[1] = x;
            szachen_machen(y, x, 2, lost_black, black_downfall, holy_land_black);
            break;
        default:
            break;
        }
    }

    void pawn_attack(int y, int x, int allowed[][8])
    {
        if (chessboard[y][x].key == 1)
        {
            if (x + 1 < 8)
                allowed[y + 1][x + 1] = 1;
            if (x - 1 > -1)
                allowed[y + 1][x - 1] = 1;
        }
        if (chessboard[y][x].key == 7)
        {
            if (x - 1 > -1)
                allowed[y - 1][x - 1] = 1;
            if (x - 1 > -1)
                allowed[y - 1][x + 1] = 1;
        }
    }

    void pawn(int y, int x, int allowed[][8], int chessboard[][8])
    {
        if (y == 1 && chessboard[y][x] == 1)
        {
            for (int i = 1; i <= 2; i++)
                if (chessboard[y + i][x] == 0)
                    allowed[y + i][x] = 1;
                else break;
            if (x - 1 >= 0)
                if (chessboard[y + 1][x - 1] >= 7)
                    allowed[y + 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y + 1][x + 1] >= 7)
                    allowed[y + 1][x + 1] = 1;
            return;
        }
        if (y == 6 && chessboard[y][x] == 7)
        {
            for (int i = 1; i <= 2; i++)
                if (chessboard[y - i][x] == 0)
                    allowed[y - i][x] = 1;
                else break;
            if (x - 1 >= 0)
                if (chessboard[y - 1][x - 1] <= 6)
                    allowed[y - 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y - 1][x - 1] <= 6)
                    allowed[y - 1][x - 1] = 1;
            return;
        }
        if (chessboard[y][x] == 7)
        {
            if (chessboard[y - 1][x] == 0)
                allowed[y - 1][x] = 1;
            if (x - 1 >= 0)
                if (chessboard[y - 1][x - 1] <= 6)
                    allowed[y - 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y - 1][x - 1] <= 6)
                    allowed[y - 1][x - 1] = 1;
            return;
        }
        if (chessboard[y][x] == 1)
        {
            if (chessboard[y + 1][x] == 0)
                allowed[y + 1][x] = 1;
            if (x - 1 >= 0)
                if (chessboard[y + 1][x - 1] >= 7)
                    allowed[y + 1][x - 1] = 1;
            if (x + 1 <= 7)
                if (chessboard[y + 1][x + 1] >= 7)
                    allowed[y + 1][x + 1] = 1;
            return;
        }
    }

    void bishop(int y, int x, int allowed[][8], int chessboard[][8])
    {
        int i = 1;
        if (chessboard[y][x] < 7)
        {
            while (y + i < 8 && x + i < 8)
            {
                int new_y = y + i;
                int new_x = x + i;
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
                    break;
                i++;
            }
            i = 1;
            while (y - i >= 0 && x + i < 8)
            {
                int new_y = y - i;
                int new_x = x + i;
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
                    break;
                i++;
            }
            i = 1;
            while (y + i < 8 && x - i >= 0)
            {
                int new_y = y + i;
                int new_x = x - i;
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
                    break;
                i++;
            }
            i = 1;
            while (y - i >= 0 && x - i >= 0)
            {
                int new_y = y - i;
                int new_x = x - i;
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
                    break;
                i++;
            }
        }
        if (chessboard[y][x] >= 7)
        {
            i = 1;
            while (y + i < 8 && x + i < 8)
            {
                int new_y = y + i;
                int new_x = x + i;
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                    break;
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
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
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                    break;
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
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
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                    break;
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
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
                if (chessboard[new_y][new_x] == 0)
                    allowed[new_y][new_x] = 1;
                if (chessboard[new_y][new_x] >= 7)
                    break;
                if (chessboard[new_y][new_x] < 7 && chessboard[new_y][new_x] != 0)
                {
                    allowed[new_y][new_x] = 1;
                    break;
                }
                i++;
            }
        }
    }

    void rook(int y, int x, int allowed[][8], int chessboard[][8])
    {
        if (chessboard[y][x] <= 7)
        {
            int i = 1;
            while (x - i >= 0)
            {
                int new_x = x - i;
                if (chessboard[y][new_x] == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x] < 7 && chessboard[y][new_x] != 0)
                    break;
                if (chessboard[y][new_x] >= 7)
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
                if (chessboard[y][new_x] == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x] < 7 && chessboard[y][new_x] != 0)
                    break;
                if (chessboard[y][new_x] >= 7)
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
                if (chessboard[new_y][x] == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x] < 7 && chessboard[new_y][x] != 0)
                    break;
                if (chessboard[new_y][x] >= 7)
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
                if (chessboard[new_y][x] == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x] < 7 && chessboard[new_y][x] != 0)
                    break;
                if (chessboard[new_y][x] >= 7)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
        }
        if (chessboard[y][x] > 7)
        {
            int i = 1;
            while (x - i >= 0)
            {
                int new_x = x - i;
                if (chessboard[y][new_x] == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x] >= 7)
                    break;
                if (chessboard[y][new_x] < 7 && chessboard[y][new_x] != 0)
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
                if (chessboard[y][new_x] == 0)
                    allowed[y][new_x] = 1;
                if (chessboard[y][new_x] >= 7)
                    break;
                if (chessboard[y][new_x] < 7 && chessboard[y][new_x] != 0)
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
                if (chessboard[new_y][x] == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x] >= 7)
                    break;
                if (chessboard[new_y][x] < 7 && chessboard[new_y][x] != 0)
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
                if (chessboard[new_y][x] == 0)
                    allowed[new_y][x] = 1;
                if (chessboard[new_y][x] >= 7)
                    break;
                if (chessboard[new_y][x] < 7 && chessboard[new_y][x] != 0)
                {
                    allowed[new_y][x] = 1;
                    break;
                }
                i++;
            }
            i = 1;
        }
    }

    void queen(int y, int x, int allowed[][8], int chessboard[][8])
    {
        bishop(y, x, allowed, chessboard);
        rook(y, x, allowed, chessboard);
    }

    void king(int y, int x, int allowed[][8], int chessboard[][8])
    {
        if (chessboard[y][x] == 6)
        {
            for (int i = -1; i < 2; ++i)
                for (int j = -1; j < 2; ++j)
                {
                    if (y + i < 0 || y + i >= 8 || x + j < 0 || x + j >= 8)
                        continue;
                    if (chessboard[y + i][x + j] == 6)
                        continue;
                    if (chessboard[y + i][x + j] == 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                    if (chessboard[y + i][x + j] >= 7)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                }
        }
        if (chessboard[y][x] == 12)
        {
            for (int i = -1; i < 2; ++i)
                for (int j = -1; j < 2; ++j)
                {
                    if (y + i < 0 || y + i >= 8 || x + j < 0 || x + j >= 8)
                        continue;
                    if (chessboard[y + i][x + j] == 12)
                        continue;
                    if (chessboard[y + i][x + j] < 7 && chessboard[y + i][x + j] != 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                    if (chessboard[y + i][x + j] == 0)
                    {
                        allowed[y + i][x + j] = 1;
                        continue;
                    }
                }
        }
    }

    void knight(int y, int x, int allowed[][8], int chessboard[][8])
    {
        if (chessboard[y][x] == 3)
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
                        if (chessboard[new_y2][new_x] >= 7)
                            allowed[new_y2][new_x] = 1;
                        if (chessboard[new_y2][new_x] == 0)
                            allowed[new_y2][new_x] = 1;
                    }
                    if (new_y >= 0 && new_y < 8 && new_x2 >= 0 && new_x2 < 8)
                    {
                        if (chessboard[new_y][new_x2] >= 7)
                            allowed[new_y][new_x2] = 1;
                        if (chessboard[new_y][new_x2] == 0)
                            allowed[new_y][new_x2] = 1;
                    }
                }
            }
        }

        if (chessboard[y][x] == 9)
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
                        if (chessboard[new_y2][new_x] < 7 && chessboard[new_y2][new_x] != 0)
                            allowed[new_y2][new_x] = 1;
                        if (chessboard[new_y2][new_x] == 0)
                            allowed[new_y2][new_x] = 1;
                    }
                    if (new_y >= 0 && new_y < 8 && new_x2 >= 0 && new_x2 < 8)
                    {
                        if (chessboard[new_y][new_x2] < 7 && chessboard[new_y][new_x2] != 0)
                            allowed[new_y][new_x2] = 1;
                        if (chessboard[new_y][new_x2] == 0)
                            allowed[new_y][new_x2] = 1;
                    }
                }
            }
        }
    }