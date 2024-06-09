#pragma once
#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"


// P P  P P P P P  P 
// R Kn B K Q B Kn R  
//#define CHESS_SET_UP PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX, ROOK_TEXTURE_INDEX, KNIGHT_TEXTURE_INDEX, BISHOP_TEXTURE_INDEX, KING_TEXTURE_INDEX, QUEEN_TEXTURE_INDEX, BISHOP_TEXTURE_INDEX,KNIGHT_TEXTURE_INDEX, ROOK_TEXTURE_INDEX
//    0     1     2       3       4       5       6     7     8      9   10     11    12    13    14    15
// {KING, QUEEN, BISHOP, BISHOP, KNIGHT, KNIGHT, ROOK, ROOK, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN}
// pozycja ka�dego z nich zapisana w tablicy 
// gdy sprawdzam czy ruszam si� dan� figur� to przeczhodz� przez wszystskie te figury w odpowiedniej tablicy w kt�rej znajduje si�  i sprawdzam czy kursor jest nad figur� je�li tak to
// odpowiednio update pozycji figury zale�nie od myszki 
// podczas puszczenia figury myszk� sprawdzenie nad kt�rym polem jest kursor 
// sprawdzenie czy dan� figur� mo�na zrobi� tam ruch jesli tak wukonujesz ruch 
// i odpowiedno updatejuszesz nowa pozycje figury bazuj�c na pozycji zwr�conej przez checkCursorHover
// je�eli ma zostac zbita figura przez obecny ruch 
//          0     1     2       3       4       5       6     7     8      9   10     11    12    13    14    15
// WHITE {KING, QUEEN, BISHOP, BISHOP, KNIGHT, KNIGHT, ROOK, ROOK, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN}
// BLACK {KING, QUEEN, BISHOP, BISHOP, KNIGHT, KNIGHT, ROOK, ROOK, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN}
// 
//fieldY = (~fieldY) & 7; //zamiana na koordynaty tadeuszowe
/*
* z racji tego �e �rodek planszy jest ustawiony jako przeci�cie si� przek�tnych to jest to {4,4} daltego by wyra�a� wsp�rz�dne planszy jako 1-8 i A-H
* trzeba te wpisane wsp�rzene znormalizowa� do wsp�rzenych wy�weitalnych wzgl�dem planszy
*/

#define BOARD_SIDE_SIZE 8

#define COLOR_1 {1.0f, 0.807f, 0.619f}  //255,206,158
#define COLOR_2  {0.819f, 0.545f, 0.278f} //209,139,71


#define CHESS_POSITION_ARRAY_KING_INDEX 0 
#define CHESS_POSITION_ARRAY_QUEEN_INDEX 1
#define CHESS_POSITION_ARRAY_BISHOP_INDEX 2
#define CHESS_POSITION_ARRAY_KNIGHT_INDEX 4
#define CHESS_POSITION_ARRAY_ROOK_INDEX 6
#define CHESS_POSITION_ARRAY_PAWN_INDEX 8

#define CHESS_TEXTURE_UV_WIDTH 1.0f/6.0f
#define CHESS_TEXTURE_UV_HEIGHT 1.0f/2.0f

#define ONE_COLOR_SIZE 16

#define WHITE_START_POSITIONS {{4,0}, {3,0}, {2,0},  {5,0,},{1,0}, { 6,0}, {0,0},{7,0}, { 0,1}, { 1,1}, { 2,1},{ 3,1},{ 4,1}, { 5,1}, { 6,1},{ 7,1}}
#define BLACK_START_POSITIONS {{4,7}, {3,7}, {2,7},  {5,7,},{1,7}, { 6,7}, {0,7},{7,7}, { 0,6}, { 1,6}, { 2,6},{ 3,6},{ 4,6}, { 5,6}, { 6,6},{ 7,6}}

#define CHESS_PIECES_ARRAY_TEXTURES {KING_TEXTURE_INDEX, QUEEN_TEXTURE_INDEX, BISHOP_TEXTURE_INDEX, BISHOP_TEXTURE_INDEX, KNIGHT_TEXTURE_INDEX, KNIGHT_TEXTURE_INDEX, ROOK_TEXTURE_INDEX, ROOK_TEXTURE_INDEX,  PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX,PAWN_TEXTURE_INDEX}

#define STANDARD_SQUARE_INDICIES_SIZE 6
#define STANDARD_SQUARE_VERTICIES_SIZE 4
#define SQUARE_STANDARD_INDICIES {0, 1, 2, 2, 3, 0}

enum chessPieceColor {
    WHITE_CHESS_INDEX = 1,
    BLACK_CHESS_INDEX = 0
};

/*
1, 7  = pawn
2, 8  = rook
3, 9  = knight
4, 10 = bishop
5, 11 = queen
6, 12 = king
*/
enum chessPieceTexIndex {
    KING_TEXTURE_INDEX = 0,
    QUEEN_TEXTURE_INDEX = 1,
    BISHOP_TEXTURE_INDEX = 2,
    KNIGHT_TEXTURE_INDEX = 3,
    ROOK_TEXTURE_INDEX= 4,
    PAWN_TEXTURE_INDEX = 5,
};

struct vec3 {
    float x, y, z;
};

struct vec2 {
    float x, y;
};

struct boardVertex {
    vec3 position;
    vec3 color;
};

typedef struct boardVertex boardVertex;

struct chessBoardSquare {
    boardVertex point[4];
};



struct ivec3 {
    int x;
    int y;
    int z;
};

struct uivec3 {
    unsigned int x;
    unsigned int y;
    unsigned int z;
};


struct vertexChessPiece {
    float x, y, z;
    float u, v;
    unsigned int pieceId; 
};

struct ChessPiece {
    vertexChessPiece vertecies[4];
};

const int ChessPieceIndiciesOffset = 6;
struct ChessPiceIndicies {
    unsigned int i[6];
};

//bool checkCursorHoverChessPieces(AppData *data, vec2 *hoveredField); 
bool createChessAtlasTexture(unsigned int *textureId, const char *path);
bool CheckIfCursorHoversChessPiece(ChessPiece piece, vec2 boardPos, vec2 cursorPos);
chessBoardSquare createBoardSquare(vec3 position, vec3 color, float scale);
void CreateBoardBuffer(chessBoardSquare *board, uivec3* boardIndicies, float scale);
bool checkCursorHover(chessBoardSquare boardLocalVert[], int size, vec2 boardPos, vec2 cursorPos, vec2* fieldIndex);
ChessPiece createChessPiece(vec2 positionOnBoard, unsigned int chessPieceIndex, unsigned int chessPieceColorIndex, float scale); 