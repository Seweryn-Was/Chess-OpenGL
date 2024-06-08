#pragma once
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