
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "chess_renderer.h"
#include "app.h"

//{ KING, QUEEN, BISHOP, BISHOP, KNIGHT, KNIGHT, ROOK,   ROOK, PAWN,   PAWN,   PAWN,   PAWN,  PAWN,   PAWN,   PAWN, PAWN }

const vec2 positionsWhite[] = WHITE_START_POSITIONS;
const vec2 positionsBlack[] = BLACK_START_POSITIONS;
const int texturesIndexes[] = CHESS_PIECES_ARRAY_TEXTURES;

bool canBePlacedthere(int x, int y) {
    if (x % 2 == 0) {
        return true; 
    }
    return false; 
}

void updateChessPieceVBO(unsigned int VBO, unsigned int pos, unsigned int size,ChessPiece *data) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, pos * sizeof(ChessPiece), sizeof(ChessPiece), data);

}

bool checkCursorHoverChessPieces(AppData* data, vec2 *hoveredField, ChessPiece* &clickedPiece, int *index, int chessPieceColor) {
    
    ChessPiece(*curretnCheckedPieces)[ONE_COLOR_SIZE] = nullptr; ;

    if (chessPieceColor == BLACK_CHESS_INDEX) {
        curretnCheckedPieces = data->blackPieces; 
    }
    if (chessPieceColor == WHITE_CHESS_INDEX) {
        curretnCheckedPieces = data->WhitePieces; 
    }

    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        ChessPiece current = (*curretnCheckedPieces)[i]; //(*appdata.WhitePieces)[i]; 
        //current.vertecies[0].x; )

        vec3 min = { current.vertecies[0].x + data->boardScreenCoords.x, current.vertecies[0].y + data->boardScreenCoords.y };
        vec3 max = { current.vertecies[2].x + data->boardScreenCoords.x, current.vertecies[2].y + data->boardScreenCoords.y };
        vec2 cursorPos = data->cursorScreenCoords;
        if (cursorPos.x < max.x && cursorPos.y<max.y && cursorPos.x>min.x && cursorPos.y > min.y) {
            //printf("HOVER PRESS"); 
            //std::cout << &(*data->WhitePieces)[i] << "\n"; 
            clickedPiece = &(*curretnCheckedPieces)[i];
            *index = i; 
            checkCursorHover(data->boardLocalVert[0], 64, data->boardScreenCoords, data->cursorScreenCoords, hoveredField);
            return true; 
        }
    }
    *index = -1; 
    clickedPiece = nullptr; 
    return false; 
}

void translateChessPiece(ChessPiece* clickedPiece, vec2 offset, vec2 *totalOffset) {
    clickedPiece->vertecies[0].x += offset.x;
    clickedPiece->vertecies[0].y += offset.y;
    clickedPiece->vertecies[1].x += offset.x;
    clickedPiece->vertecies[1].y += offset.y;
    clickedPiece->vertecies[2].x += offset.x;
    clickedPiece->vertecies[2].y += offset.y;
    clickedPiece->vertecies[3].x += offset.x;
    clickedPiece->vertecies[3].y += offset.y;
    
    totalOffset->x += offset.x; 
    totalOffset->y += offset.y; 
}
void setChessPiecePosOnBoard(ChessPiece *clickedPiece, vec2 posOnBoard, float scaleUI){
    clickedPiece->vertecies[0].x = (-0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[0].y = (-0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[1].x = (0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[1].y = (-0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[2].x = (0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[2].y = (0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[3].x = (-0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[3].y = (0.5f + posOnBoard.y) * scaleUI;
}

struct UIrect {
    vec3 vert[4];
    vec3 color[4]; 
};

//void mouseButtonLeft(AppData *appdata) {
//    appdata.mouseBtnLeftPress = (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
//    if (appdata.mouseBtnLeftPress && !appdata.mouseBtnLeftPressClicked)
//    {
//        printf("ON PRESS\n");
//        appdata.mouseBtnLeftPressClicked = true;
//    }
//    else {
//        if (appdata.mouseBtnLeftPressClicked == true && appdata.mouseBtnLeftPress == false) {
//            printf("ON REALEASE\n");
//            //appdata->MouseLeftOnRelease = 
//        }
//        if (appdata.mouseBtnLeftPress == false) appdata.mouseBtnLeftPressClicked = false;
//    }
//}

int main(void)
{
    App app;
    AppData appdata;

    if (!initApp(&app)) {
        return -1;
    }

    UIrect startGameVertUi = {
        -1.5f*app.scaleUI, -0.75f*app.scaleUI, 0.0f, 1.0f, 0.0f, 0.0f,
         1.5f*app.scaleUI, -0.75f*app.scaleUI, 0.0f, 1.0f, 0.0f, 0.0f,
         1.5f*app.scaleUI,  0.75f*app.scaleUI, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.5f*app.scaleUI,  0.75f*app.scaleUI, 0.0f, 1.0f, 0.0f, 0.0f,
    }; 
    unsigned int startGame[] = {
        0, 1, 2,
        2, 3, 0,
    }; 
    Buffers btnStartGame; 

    glGenVertexArrays(1, &btnStartGame.VAO);
    glBindVertexArray(btnStartGame.VAO);

    glGenBuffers(1, &btnStartGame.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, btnStartGame.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(startGameVertUi),&startGameVertUi , GL_STATIC_DRAW);

    glGenBuffers(1, &btnStartGame.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, btnStartGame.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(startGame),&startGame, GL_STATIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_COLOR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_COLOR);
    


    appdata.mouseBtnLeftPressClicked = false;
    const int sizeOfTriangles = ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE;

    appdata.WhitePieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    appdata.whitePiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;

    appdata.blackPieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    appdata.blackPiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;
   
    for (int i = 0; i < sizeOfTriangles; i+=NUM_OF_TRIS_IN_SQUARE) {
        int offset = i/2 * STANDARD_SQUARE_VERTICIES_SIZE;
        (*appdata.whitePiecesIndicies)[i].x = 0 + offset;
        (*appdata.whitePiecesIndicies)[i].y = 1 + offset;
        (*appdata.whitePiecesIndicies)[i].z = 2 + offset;
        (*appdata.whitePiecesIndicies)[i+1].x = 2 + offset;
        (*appdata.whitePiecesIndicies)[i+1].y = 3 + offset;
        (*appdata.whitePiecesIndicies)[i+1].z = 0 + offset;

        (*appdata.blackPiecesIndicies)[i].x = 0 + offset;
        (*appdata.blackPiecesIndicies)[i].y = 1 + offset;
        (*appdata.blackPiecesIndicies)[i].z = 2 + offset;
        (*appdata.blackPiecesIndicies)[i + 1].x = 2 + offset;
        (*appdata.blackPiecesIndicies)[i + 1].y = 3 + offset;
        (*appdata.blackPiecesIndicies)[i + 1].z = 0 + offset;
    }
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        (*appdata.WhitePieces)[i] = createChessPiece({ positionsWhite[i].x - 4 + 0.5f, positionsWhite[i].y - 4 + 0.5f }, texturesIndexes[i], WHITE_CHESS_INDEX, app.scaleUI);
        (*appdata.blackPieces)[i] = createChessPiece({ positionsBlack[i].x - 4 + 0.5f, positionsBlack[i].y - 4 + 0.5f }, texturesIndexes[i], BLACK_CHESS_INDEX, app.scaleUI);
    }

    Buffers whitePiecesBuffers; 
    appdata.whitePiecesBuffers = &whitePiecesBuffers; 
    glGenVertexArrays(1, &whitePiecesBuffers.VAO);
    glBindVertexArray(whitePiecesBuffers.VAO);

    glGenBuffers(1, &appdata.whitePiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, appdata.whitePiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*appdata.WhitePieces), appdata.WhitePieces, GL_STATIC_DRAW);

    glGenBuffers(1, &appdata.whitePiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, appdata.whitePiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*appdata.whitePiecesIndicies), appdata.whitePiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    Buffers blackPiecesBuffers;
    appdata.blackPiecesBuffers = &blackPiecesBuffers;
    glGenVertexArrays(1, &blackPiecesBuffers.VAO);
    glBindVertexArray(blackPiecesBuffers.VAO);

    glGenBuffers(1, &appdata.blackPiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, appdata.blackPiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*appdata.blackPieces), appdata.blackPieces, GL_STATIC_DRAW);

    glGenBuffers(1, &appdata.blackPiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, appdata.blackPiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*appdata.blackPiecesIndicies), appdata.blackPiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    createChessAtlasTexture(&appdata.chessPiecesAtlasTexture, "./res/textures/chess.png");

    Buffers boardBuffers;
    appdata.boardBuffers = &boardBuffers;
    chessBoardSquare boardLocalVert[64];
    appdata.boardLocalVert = &boardLocalVert; 
    uivec3 boardTriangleIndicies[64 * 2];
    appdata.boardTriangleIndicies = &boardTriangleIndicies; 

    CreateBoardBuffer(&boardLocalVert[0], &boardTriangleIndicies[0], app.scaleUI);

    glGenVertexArrays(1, &appdata.boardBuffers->VAO);
    glBindVertexArray(appdata.boardBuffers->VAO);

    glGenBuffers(1, &appdata.boardBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, appdata.boardBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*appdata.boardLocalVert), appdata.boardLocalVert, GL_STATIC_DRAW);

    glGenBuffers(1, &appdata.boardBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, appdata.boardBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*appdata.boardTriangleIndicies), appdata.boardTriangleIndicies, GL_STATIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_COLOR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_COLOR);

    int size = sizeof(glm::mat4);
    glGenBuffers(1, &appdata.ProjectionUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, appdata.ProjectionUBO);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, appdata.ProjectionUBO); //accesbiel at binding 0 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    

    /* Loop until the user closes the window */
    appdata.isWhiteMove = true; 
    ChessPiece* clickedPiece = nullptr;
    vec2 clickCursorPosLast{};
    vec2 totalOffset{};
    int index = 0;
    while (!glfwWindowShouldClose(app.window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetFramebufferSize(app.window, &app.SCR_WIDTH, &app.SCR_HEIGHT);
        glViewport(0, 0, app.SCR_WIDTH, app.SCR_HEIGHT);

        float ascpectRatio = (float)app.SCR_WIDTH / (float)app.SCR_HEIGHT, t = 1.0f;
        glm::mat4 projection = glm::ortho(-ascpectRatio, ascpectRatio, -t, t);
        glBindBuffer(GL_UNIFORM_BUFFER, appdata.ProjectionUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projection), &projection);

        double xpos, ypos;
        glfwGetCursorPos(app.window, &xpos, &ypos);

        vec2 ndcCursor = { (xpos / app.SCR_WIDTH) * 2.0f - 1.0f,1.0f - (ypos / app.SCR_HEIGHT) * 2.0f };
        glm::vec4 cursorScreenCoords = glm::inverse(projection) * glm::vec4(ndcCursor.x, ndcCursor.y, 0.0f, 1.0f);
        appdata.cursorScreenCoords = { cursorScreenCoords.x, cursorScreenCoords.y };

        glm::vec4 boardscreencoords = glm::inverse(projection) * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
        boardscreencoords.x += 1.f;
        appdata.boardScreenCoords = { boardscreencoords.x, boardscreencoords.y };

        glm::vec4 Menucreencoords = glm::inverse(projection) * glm::vec4(1.0f, .75f, 0.0f, 1.0f);
        Menucreencoords.x -= 0.5f;
        vec2 menucoords = { Menucreencoords.x, Menucreencoords.y };

        vec2 hoveredField;
        unsigned int currentVBO = appdata.isWhiteMove ? appdata.whitePiecesBuffers->VBO : appdata.blackPiecesBuffers->VBO; 
        unsigned int currentColorIndex = appdata.isWhiteMove ? WHITE_CHESS_INDEX : BLACK_CHESS_INDEX; 

        appdata.mouseBtnLeftPress = (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        if (appdata.mouseBtnLeftPress && !appdata.mouseBtnLeftPressClicked)
        {
            //printf("ON PRESS\n");
            appdata.mouseBtnLeftPressClicked = true;
            if (checkCursorHoverChessPieces(&appdata, &hoveredField, clickedPiece, &index, currentColorIndex)) { totalOffset = { 0.0f, 0.0f }; }
        }
        else {
            if (appdata.mouseBtnLeftPressClicked == true && appdata.mouseBtnLeftPress == false) {
                //printf("REALEASE\n"); 
                checkCursorHover(appdata.boardLocalVert[0], 64, appdata.boardScreenCoords, appdata.cursorScreenCoords, &hoveredField); 
                if(clickedPiece!=nullptr)
                if (canBePlacedthere(hoveredField.x, hoveredField.y)) {
                    appdata.isWhiteMove = !appdata.isWhiteMove; 
                    vec2 posOnBoard = { hoveredField.x - 4 + 0.5f, hoveredField.y - 4 + 0.5f };
                    setChessPiecePosOnBoard(clickedPiece, posOnBoard, app.scaleUI);
                    updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece);

                    if (checkCursorHoverChessPieces(&appdata, &hoveredField, clickedPiece, &index, appdata.isWhiteMove ? WHITE_CHESS_INDEX : BLACK_CHESS_INDEX)) {
                        setChessPiecePosOnBoard(clickedPiece, { -5, 0}, app.scaleUI);
                        updateChessPieceVBO(appdata.isWhiteMove ? appdata.whitePiecesBuffers->VBO : appdata.blackPiecesBuffers->VBO, index, sizeof(ChessPiece), clickedPiece);
                    }
                } else {
                    translateChessPiece(clickedPiece, { -totalOffset.x, -totalOffset.y }, &totalOffset);
                    updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece);
                }
            }
            else {
                //printf("NO RELEASE"); 
            }
            if (appdata.mouseBtnLeftPress == false) appdata.mouseBtnLeftPressClicked = false;
        }
        if (!(appdata.mouseBtnLeftPress && clickedPiece != nullptr)) clickedPiece = nullptr;
        if (clickedPiece != nullptr) {
            //MOVE LOGIC 
            vec2 offset = { appdata.cursorScreenCoords.x - clickCursorPosLast.x, appdata.cursorScreenCoords.y - clickCursorPosLast.y };
            translateChessPiece(clickedPiece, offset, &totalOffset); 
            updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece); 
        }
        clickCursorPosLast = appdata.cursorScreenCoords; 
  
        //rysowanie planszy <--->
        UseShaderProgram(app.data.boardShader);
        GLint uniformLocation = glGetUniformLocation(app.data.boardShader->programId, "transform");
        glm::mat4 transform = glm::mat4(1.0);
        /*transform = glm::translate(transform, glm::vec3(Menucreencoords.x, Menucreencoords.y, -1.0f));
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(btnStartGame.VAO);
        glDrawElements(GL_TRIANGLES, (sizeof(startGame) / sizeof(unsigned int)), GL_UNSIGNED_INT, 0);*/

        transform = glm::mat4(1.0);
        transform = glm::translate(transform, glm::vec3(appdata.boardScreenCoords.x, appdata.boardScreenCoords.y, -1.0f));
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(appdata.boardBuffers->VAO);
        glDrawElements(GL_TRIANGLES, (sizeof(*appdata.boardTriangleIndicies) / sizeof(int)), GL_UNSIGNED_INT, 0);


        //rysowanie pionek <--->
        UseShaderProgram(app.data.standard);
        uniformLocation = glGetUniformLocation(app.data.standard->programId, "transform");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(appdata.whitePiecesBuffers->VAO);
        glDrawElements(GL_TRIANGLES, sizeof(*appdata.whitePiecesIndicies) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(appdata.blackPiecesBuffers->VAO);
        glDrawElements(GL_TRIANGLES, sizeof(*appdata.blackPiecesIndicies) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }
    DeleteShaderProgram(app.data.standard);
    DeleteShaderProgram(app.data.boardShader);
    glDeleteVertexArrays(1, &appdata.whitePiecesBuffers->VAO);
    glDeleteBuffers(1, &appdata.whitePiecesBuffers->VBO);
    glDeleteBuffers(1, &appdata.whitePiecesBuffers->EBO);

    glDeleteVertexArrays(1, &appdata.boardBuffers->VAO);
    glDeleteBuffers(1, &appdata.boardBuffers->VBO);
    glDeleteBuffers(1, &appdata.boardBuffers->EBO);

    glDeleteBuffers(1, &appdata.ProjectionUBO);

    delete[] appdata.WhitePieces;
    delete[] appdata.whitePiecesIndicies;
    delete[] appdata.blackPiecesIndicies; 
    delete[] appdata.blackPieces; 
    
    terminateApp(&app); 

    return 0;
}
