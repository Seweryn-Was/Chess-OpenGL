#ifndef APP_LOGIC
#define APP_LOGIC
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "chess_renderer.h"

const vec2 positionsWhite[] = WHITE_START_POSITIONS;
const vec2 positionsBlack[] = BLACK_START_POSITIONS;
const int texturesIndexes[] = CHESS_PIECES_ARRAY_TEXTURES;

#define LAYOUT_LOCATION_POS 0 
#define LAYOUT_LOCATION_COLOR 1 
#define LAYOUT_LOCATION_UV 2 
#define BOARD_FIELDS_NUM 64

#define NUM_OF_TRIS_IN_SQUARE 2

#ifndef ONE_COLOR_SIZE 
    #define ONE_COLOR_SIZE 16
#endif // !ONE_COLOR_SIZE 

/*
UI:
    CHESS LOGO
    NEW GAME
        -SELECT SLOT {1, 2, 3}
    LOAD GAME
        -SELECT SLOT {1, 2, 3}
    TIME 1
    TIME 2
*/

//{ KING, QUEEN, BISHOP, BISHOP, KNIGHT, KNIGHT, ROOK,   ROOK, PAWN,   PAWN,   PAWN,   PAWN,  PAWN,   PAWN,   PAWN, PAWN }


struct Buffers {
    unsigned int VAO, VBO, EBO;
};

struct AppData {
    float scaleUI; 

    vec2 cursorScreenCoords; 
    vec2 boardScreenCoords; 

    bool mouseBtnLeftPress; 
    bool mouseBtnLeftPressClicked;

    bool MouseLeftOnPress; 
    bool MouseLeftOnRelease; 

    bool isWhiteMove;
    bool isMate; 
    bool canMakeMove;

    ChessPiece* activePiece; 
    ChessPiece* lastClickedPiece; 



    unsigned int chessPiecesAtlasTexture;
    unsigned int ProjectionUBO; 
    ShaderProgram *standard;
    ShaderProgram *boardShader;

    //vec2 boardPos; 

    Buffers *boardBuffers;
    Buffers *whitePiecesBuffers; 
    Buffers *blackPiecesBuffers;

    ChessPiece (*WhitePieces)[ONE_COLOR_SIZE];
    uivec3 (*whitePiecesIndicies)[ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];
    
    ChessPiece (*blackPieces)[ONE_COLOR_SIZE];
    uivec3 (*blackPiecesIndicies)[ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];

    chessBoardSquare (*boardLocalVert)[BOARD_FIELDS_NUM];
    uivec3 (*boardTriangleIndicies)[BOARD_FIELDS_NUM * NUM_OF_TRIS_IN_SQUARE];
};

struct App {
	GLFWwindow* window;
    int SCR_WIDTH;
    int SCR_HEIGHT;
    float scaleUI; 
    AppData data; 
};

bool initApp(App* app); 
void SetAppData(App* app); 
bool runApp(App* app);
bool terminateApp(App* app);

#endif