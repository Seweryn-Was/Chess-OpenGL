#pragma once

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
void without(Allowed chessboard[][8], int without_kings[][8]);
void piece(int y, int x, int allowed[][8]);
void pawn(int y, int x, int allowed[][8]);
void bishop(int y, int x, int allowed[][8]);
void rook(int y, int x, int allowed[][8]);
void queen(int y, int x, int allowed[][8]);
void king(int y, int x, int allowed[][8]);
void knight(int y, int x, int allowed[][8]);
void everywhere_zeros(int allowed[][8]);
void all_possible_attacks(int chessboard[][8]);
void block_pieces(int y, int x);