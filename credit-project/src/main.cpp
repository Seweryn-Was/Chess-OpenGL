#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "chess_renderer.h"
#include "app.h"
#include "logika.h"

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

void updateChessPieceVBO(unsigned int VBO, unsigned int pos, unsigned int size, ChessPiece* data) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, pos * sizeof(ChessPiece), sizeof(ChessPiece), data);

}

bool checkCursorHoverChessPieces(AppData* data, vec2* hoveredField, ChessPiece*& clickedPiece, int* index, int chessPieceColor) {

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

void translateChessPiece(ChessPiece* clickedPiece, vec2 offset, vec2* totalOffset) {
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

void setChessPiecePosOnBoard(ChessPiece* clickedPiece, vec2 posOnBoard, float scaleUI) {
    clickedPiece->vertecies[0].x = (-0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[0].y = (-0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[1].x = (0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[1].y = (-0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[2].x = (0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[2].y = (0.5f + posOnBoard.y) * scaleUI;
    clickedPiece->vertecies[3].x = (-0.5f + posOnBoard.x) * scaleUI;
    clickedPiece->vertecies[3].y = (0.5f + posOnBoard.y) * scaleUI;
}

void loadFromFile(const char* loadFile, AppData* data) {
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, loadFile, "r");
    if (err != 0) {
        perror("Nie mo¿na otworzyæ pliku do odczytu ");
        return;
    }
    vec2 loc = {};
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fscanf(file, "%f %f", &loc.x, &loc.y); 

        printf("%f",loc.x);
        printf(" %f \n", loc.y);
  
        (*data->WhitePieces)[i] = createChessPiece({ loc.x - 4 + 0.5f, loc.y - 4 + 0.5f }, texturesIndexes[i], WHITE_CHESS_INDEX, data->scaleUI);
    }

    
    glBindBuffer(GL_ARRAY_BUFFER, data->whitePiecesBuffers->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, data->WhitePieces);
   
    printf("-----\n"); 
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        fscanf(file, "%f %f", &loc.x, &loc.y);

        (*data->blackPieces)[i] = createChessPiece({ loc.x - 4 + 0.5f, loc.y - 4 + 0.5f }, texturesIndexes[i], BLACK_CHESS_INDEX, data->scaleUI);
    }
    glBindBuffer(GL_ARRAY_BUFFER, data->blackPiecesBuffers->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, data->blackPieces);
    // Zamkniêcie pliku
    fclose(file);
}

void ChoseSlotToLoadGame_PopUp(const char* question, AppData* data) {
    
    static bool shouldChose = false; 
    static bool confirmAction = false; 
    static int chosenSlot = 0; 

    if (ImGui::Button("LOAD")) {
        shouldChose = true;
    }

    if (shouldChose) {
        ImGui::OpenPopup("Choose Slot"); // Open the dialog popup
    }

    if (ImGui::BeginPopupModal("Choose Slot", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(question);
        if (ImGui::Button("Slot 1")) {
            // Do something for Option 1
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;
            //loadFromFile("slot_1.txt", &data); 
            chosenSlot = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Slot 2")) {
            // Do something for Option 2
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;

            //loadFromFile("slot_2.txt", &data);
            chosenSlot = 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("Slot 3")) {
            // Do something for Option 3
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;
            //loadFromFile("slot_3.txt", &data);
            chosenSlot = 3; 
        }
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            chosenSlot = 0; 
        }
        ImGui::EndPopup();
    }

    if (confirmAction) {
        ImGui::OpenPopup("load Dialog"); // Open the dialog popup
    }

    if (ImGui::BeginPopupModal("load Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure, you want to load game?");
        if (ImGui::Button("YES")) {
            confirmAction = false;
            //LOAD THAT GAME
            switch (chosenSlot) {
            case 1 : 
                loadFromFile("slot_1.txt", data);
                break;
            case 2:
                loadFromFile("slot_2.txt", data);
                break; 
            case 3:
                loadFromFile("slot_3.txt", data);
                break; 

            }
            printf("LOAD GAME %d\n", chosenSlot);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("NO")) {
            //DO NOTHING
            confirmAction = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void saveToFIle(const char* slotFile, AppData data) {
    FILE* file = fopen(slotFile, "w");
    if (file == NULL) {
        perror("Nie mo¿na otworzyæ pliku");
        return;
    }

    // Zapisanie elementów tablicy do pliku
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fprintf(file, "%f", ((*data.WhitePieces)[i].vertecies->x / data.scaleUI) + 4.0f);
        fprintf(file, " %f \n", ((*data.WhitePieces)[i].vertecies->y / data.scaleUI) + 4.0f);
    }
    fprintf(file, "\n"); 
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fprintf(file, "%f", ((*data.blackPieces)[i].vertecies->x / data.scaleUI) + 4.0f);
        fprintf(file, " %f \n", ((*data.blackPieces)[i].vertecies->y / data.scaleUI) + 4.0f);
    }

    // Zamkniêcie pliku
    fclose(file);
}

void ChoseSlotToSaveGame_PopUp(const char* question, AppData data) {

    static bool shouldChose = false;
    static bool confirmAction = false;
    static int chosenSlot = 0;

    if (ImGui::Button("SAVE")) {
        shouldChose = true;
    }

    if (shouldChose) {
        ImGui::OpenPopup("Choose Slot save"); // Open the dialog popup
    }

    if (ImGui::BeginPopupModal("Choose Slot save", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(question);
        if (ImGui::Button("Slot 1")) {
            // Do something for Option 1
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;
            saveToFIle("slot_1.txt", data); 
            chosenSlot = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Slot 2")) {
            // Do something for Option 2
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;
            saveToFIle("slot_2.txt", data);
            chosenSlot = 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("Slot 3")) {
            // Do something for Option 3
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            confirmAction = true;
            saveToFIle("slot_3.txt", data);
            chosenSlot = 3;
        }
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            chosenSlot = 0;
        }
        ImGui::EndPopup();
    }

    if (confirmAction) {
        ImGui::OpenPopup("save Dialog"); // Open the dialog popup
    }

    if (ImGui::BeginPopupModal("save Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure, you want to save game to slot %d? Game saved to that slot will be lost.", chosenSlot);
        if (ImGui::Button("YES")) {
            confirmAction = false;
            //LOAD THAT GAME
            printf("SAVE GAME to slot %d\n", chosenSlot);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("NO")) {
            //DO NOTHING
            confirmAction = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

int main(void)
{
    App app;

    if (!initApp(&app)) {
        return -1;
    }

    mainLogika();



    app.data.mouseBtnLeftPressClicked = false;
    const int sizeOfTriangles = ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE;

    app.data.WhitePieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    app.data.whitePiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;

    app.data.blackPieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    app.data.blackPiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;
   
    for (int i = 0; i < sizeOfTriangles; i+=NUM_OF_TRIS_IN_SQUARE) {
        int offset = i/2 * STANDARD_SQUARE_VERTICIES_SIZE;
        (*app.data.whitePiecesIndicies)[i].x = 0 + offset;
        (*app.data.whitePiecesIndicies)[i].y = 1 + offset;
        (*app.data.whitePiecesIndicies)[i].z = 2 + offset;
        (*app.data.whitePiecesIndicies)[i+1].x = 2 + offset;
        (*app.data.whitePiecesIndicies)[i+1].y = 3 + offset;
        (*app.data.whitePiecesIndicies)[i+1].z = 0 + offset;

        (*app.data.blackPiecesIndicies)[i].x = 0 + offset;
        (*app.data.blackPiecesIndicies)[i].y = 1 + offset;
        (*app.data.blackPiecesIndicies)[i].z = 2 + offset;
        (*app.data.blackPiecesIndicies)[i + 1].x = 2 + offset;
        (*app.data.blackPiecesIndicies)[i + 1].y = 3 + offset;
        (*app.data.blackPiecesIndicies)[i + 1].z = 0 + offset;
    }
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        (*app.data.WhitePieces)[i] = createChessPiece({ positionsWhite[i].x - 4 + 0.5f, positionsWhite[i].y - 4 + 0.5f }, texturesIndexes[i], WHITE_CHESS_INDEX, app.scaleUI);
        (*app.data.blackPieces)[i] = createChessPiece({ positionsBlack[i].x - 4 + 0.5f, positionsBlack[i].y - 4 + 0.5f }, texturesIndexes[i], BLACK_CHESS_INDEX, app.scaleUI);
    }

    Buffers whitePiecesBuffers; 
    app.data.whitePiecesBuffers = &whitePiecesBuffers; 
    glGenVertexArrays(1, &whitePiecesBuffers.VAO);
    glBindVertexArray(whitePiecesBuffers.VAO);

    glGenBuffers(1, &app.data.whitePiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app.data.whitePiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app.data.WhitePieces), app.data.WhitePieces, GL_STATIC_DRAW);

    glGenBuffers(1, &app.data.whitePiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app.data.whitePiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app.data.whitePiecesIndicies), app.data.whitePiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    Buffers blackPiecesBuffers;
    app.data.blackPiecesBuffers = &blackPiecesBuffers;
    glGenVertexArrays(1, &blackPiecesBuffers.VAO);
    glBindVertexArray(blackPiecesBuffers.VAO);

    glGenBuffers(1, &app.data.blackPiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app.data.blackPiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app.data.blackPieces), app.data.blackPieces, GL_STATIC_DRAW);

    glGenBuffers(1, &app.data.blackPiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app.data.blackPiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app.data.blackPiecesIndicies), app.data.blackPiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    createChessAtlasTexture(&app.data.chessPiecesAtlasTexture, "./res/textures/chess.png");

    Buffers boardBuffers;
    app.data.boardBuffers = &boardBuffers;
    chessBoardSquare boardLocalVert[64];
    app.data.boardLocalVert = &boardLocalVert; 
    uivec3 boardTriangleIndicies[64 * 2];
    app.data.boardTriangleIndicies = &boardTriangleIndicies; 

    CreateBoardBuffer(&boardLocalVert[0], &boardTriangleIndicies[0], app.scaleUI);

    glGenVertexArrays(1, &app.data.boardBuffers->VAO);
    glBindVertexArray(app.data.boardBuffers->VAO);

    glGenBuffers(1, &app.data.boardBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app.data.boardBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app.data.boardLocalVert), app.data.boardLocalVert, GL_STATIC_DRAW);

    glGenBuffers(1, &app.data.boardBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app.data.boardBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app.data.boardTriangleIndicies), app.data.boardTriangleIndicies, GL_STATIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_COLOR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_COLOR);

    int size = sizeof(glm::mat4);
    glGenBuffers(1, &app.data.ProjectionUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, app.data.ProjectionUBO);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, app.data.ProjectionUBO); //accesbiel at binding 0 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    

    /* Loop until the user closes the window */
    app.data.isWhiteMove = true; 
    ChessPiece* clickedPiece = nullptr;
    vec2 clickCursorPosLast{};
    vec2 totalOffset{};
    int index = 0;
    bool EscPress = false , EscPressClicked = false;
    bool isMenuOpen = false; 
    bool selectSlot = false; 
    bool confirmAction = false; 
    bool shouldClose = false; 
    while (!glfwWindowShouldClose(app.window) && !shouldClose) {

        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //printf("Running"); 

        glfwGetFramebufferSize(app.window, &app.SCR_WIDTH, &app.SCR_HEIGHT);
        glViewport(0, 0, app.SCR_WIDTH, app.SCR_HEIGHT);

        ImGui::Begin("Move"); 
        if (app.data.isWhiteMove)
            ImGui::Text("White Move");
        else
            ImGui::Text("Black Move"); 
        ImGui::End(); 

        if (isMenuOpen) {
            ImGui::Begin("MENU", NULL, 0);
            if (ImGui::Button("RESTART")) {
                confirmAction = true; 
            }
            if (confirmAction) {
                ImGui::OpenPopup("My Dialog"); // Open the dialog popup
            }

            if (ImGui::BeginPopupModal("My Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Are you sure, you want to restart game?\nYour game will be lost.");
                if (ImGui::Button("YES")) {
                    confirmAction = false;
                    app.data.isWhiteMove = true; 
                    //RESTART GAME
                    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
                        (*app.data.WhitePieces)[i] = createChessPiece({ positionsWhite[i].x - 4 + 0.5f, positionsWhite[i].y - 4 + 0.5f }, texturesIndexes[i], WHITE_CHESS_INDEX, app.scaleUI);
                        (*app.data.blackPieces)[i] = createChessPiece({ positionsBlack[i].x - 4 + 0.5f, positionsBlack[i].y - 4 + 0.5f }, texturesIndexes[i], BLACK_CHESS_INDEX, app.scaleUI);
                    }

                    glBindBuffer(GL_ARRAY_BUFFER, app.data.whitePiecesBuffers->VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, app.data.WhitePieces);
                    glBindBuffer(GL_ARRAY_BUFFER, app.data.blackPiecesBuffers->VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, app.data.blackPieces);
                    printf("RESTART\n"); 
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine(); 
                if (ImGui::Button("NO")) {
                    //DO NOTHING
                    confirmAction = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            ChoseSlotToLoadGame_PopUp("from what slot load chess game", &app.data);
            
            ChoseSlotToSaveGame_PopUp("to what slot save game?", app.data);

            if(ImGui::Button("Resume")){
                for (int i = 0; i < ONE_COLOR_SIZE; i++) {
                  
                    printf("piece: %f [] ", ((*app.data.WhitePieces)[i].vertecies->x/app.scaleUI) + 4.0f);
                    printf("%f \n", ((*app.data.WhitePieces)[i].vertecies->y / app.scaleUI) + 4.0f );
                }
                
                isMenuOpen = false;
            }
            if (ImGui::Button("Quit Game")) {
                shouldClose = true; 
            }
            ImGui::End();
        }

        if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            // Escape key was pressed
            // Add your handling code here
            //glfwSetWindowShouldClose(app.window, GLFW_TRUE); // Close the window
            //printf("press wsc\n"); 
        }
        EscPress = (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

        if (EscPress && !EscPressClicked)
        { //Press
            printf("OPEN MENU\n");
            EscPressClicked = true; 
            isMenuOpen = !isMenuOpen; 
        }
        else {
            if (EscPressClicked == true && EscPress == false) {
                //Release  
            }
            if (EscPress == false) EscPressClicked = false;
        }

        float ascpectRatio = (float)app.SCR_WIDTH / (float)app.SCR_HEIGHT, t = 1.0f;
        glm::mat4 projection = glm::ortho(-ascpectRatio, ascpectRatio, -t, t);
        glBindBuffer(GL_UNIFORM_BUFFER, app.data.ProjectionUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projection), &projection);

        double xpos, ypos;
        glfwGetCursorPos(app.window, &xpos, &ypos);

        vec2 ndcCursor = { (xpos / app.SCR_WIDTH) * 2.0f - 1.0f,1.0f - (ypos / app.SCR_HEIGHT) * 2.0f };
        glm::vec4 cursorScreenCoords = glm::inverse(projection) * glm::vec4(ndcCursor.x, ndcCursor.y, 0.0f, 1.0f);
        app.data.cursorScreenCoords = { cursorScreenCoords.x, cursorScreenCoords.y };

        glm::vec4 boardscreencoords = glm::inverse(projection) * glm::vec4(-.0f, 0.0f, 0.0f, 1.0f);
        //boardscreencoords.x += 8 / 2 * app.scaleUI + 0.05;
        app.data.boardScreenCoords = { boardscreencoords.x, boardscreencoords.y };

        vec2 hoveredField;
        unsigned int currentVBO = app.data.isWhiteMove ? app.data.whitePiecesBuffers->VBO : app.data.blackPiecesBuffers->VBO; 
        unsigned int currentColorIndex = app.data.isWhiteMove ? WHITE_CHESS_INDEX : BLACK_CHESS_INDEX; 

        app.data.mouseBtnLeftPress = (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

        if(!isMenuOpen)
        if (app.data.mouseBtnLeftPress && !app.data.mouseBtnLeftPressClicked)
        {
            //printf("ON PRESS\n");
            app.data.mouseBtnLeftPressClicked = true;
            if (checkCursorHoverChessPieces(&app.data, &hoveredField, clickedPiece, &index, currentColorIndex)) { totalOffset = { 0.0f, 0.0f }; }
        }
        else {
            if (app.data.mouseBtnLeftPressClicked == true && app.data.mouseBtnLeftPress == false) {
                //printf("REALEASE\n"); 
                checkCursorHover(app.data.boardLocalVert[0], 64, app.data.boardScreenCoords, app.data.cursorScreenCoords, &hoveredField); 
                if(clickedPiece!=nullptr)
                if (canBePlacedthere(hoveredField.x, hoveredField.y)) {
                    bool shouldOpenPopUpToselectChessPiece = false; 
                    if (app.data.isWhiteMove && hoveredField.y == 7) {
                        printf("bialy doszedlem na osatnie pole chcl sie zmeinic\n");
                        shouldOpenPopUpToselectChessPiece = true; 
                        //Pop up
                    }
                    if (!app.data.isWhiteMove && hoveredField.y == 0) {
                        printf("czarny doszedlem na osatnie pole chcl sie zmeinic\n");
                        shouldOpenPopUpToselectChessPiece = true;
                        //Pop up
                    }
                    app.data.isWhiteMove = !app.data.isWhiteMove; 
                    vec2 posOnBoard = { hoveredField.x - 4 + 0.5f, hoveredField.y - 4 + 0.5f };
                    setChessPiecePosOnBoard(clickedPiece, posOnBoard, app.scaleUI);
                    updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece);

                    if (checkCursorHoverChessPieces(&app.data, &hoveredField, clickedPiece, &index, app.data.isWhiteMove ? WHITE_CHESS_INDEX : BLACK_CHESS_INDEX)) {
                        setChessPiecePosOnBoard(clickedPiece, { -5, 0}, app.scaleUI);
                        updateChessPieceVBO(app.data.isWhiteMove ? app.data.whitePiecesBuffers->VBO : app.data.blackPiecesBuffers->VBO, index, sizeof(ChessPiece), clickedPiece);
                    }
                } else {
                    translateChessPiece(clickedPiece, { -totalOffset.x, -totalOffset.y }, &totalOffset);
                    updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece);
                }
            }
            else {
                //printf("NO RELEASE"); 
            }
            if (app.data.mouseBtnLeftPress == false) app.data.mouseBtnLeftPressClicked = false;
        }
        if (!(app.data.mouseBtnLeftPress && clickedPiece != nullptr)) clickedPiece = nullptr;
        if (clickedPiece != nullptr) {
            //MOVE LOGIC 
            vec2 offset = { app.data.cursorScreenCoords.x - clickCursorPosLast.x, app.data.cursorScreenCoords.y - clickCursorPosLast.y };
            translateChessPiece(clickedPiece, offset, &totalOffset); 
            updateChessPieceVBO(currentVBO, index, sizeof(ChessPiece), clickedPiece); 
        }
        clickCursorPosLast = app.data.cursorScreenCoords; 
  
        //rysowanie planszy <--->
        UseShaderProgram(app.data.boardShader);
        GLint uniformLocation = glGetUniformLocation(app.data.boardShader->programId, "transform");
        glm::mat4 transform = glm::mat4(1.0);
       
        transform = glm::mat4(1.0);
        transform = glm::translate(transform, glm::vec3(app.data.boardScreenCoords.x, app.data.boardScreenCoords.y, -1.0f));
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(app.data.boardBuffers->VAO);
        glDrawElements(GL_TRIANGLES, (sizeof(*app.data.boardTriangleIndicies) / sizeof(int)), GL_UNSIGNED_INT, 0);


        //rysowanie pionek <--->
        UseShaderProgram(app.data.standard);
        uniformLocation = glGetUniformLocation(app.data.standard->programId, "transform");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(app.data.whitePiecesBuffers->VAO);
        glDrawElements(GL_TRIANGLES, sizeof(*app.data.whitePiecesIndicies)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(app.data.blackPiecesBuffers->VAO);
        glDrawElements(GL_TRIANGLES, sizeof(*app.data.blackPiecesIndicies)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(app.window);
    }


    DeleteShaderProgram(app.data.standard);
    DeleteShaderProgram(app.data.boardShader);
    glDeleteVertexArrays(1, &app.data.whitePiecesBuffers->VAO);
    glDeleteBuffers(1, &app.data.whitePiecesBuffers->VBO);
    glDeleteBuffers(1, &app.data.whitePiecesBuffers->EBO);

    glDeleteVertexArrays(1, &app.data.boardBuffers->VAO);
    glDeleteBuffers(1, &app.data.boardBuffers->VBO);
    glDeleteBuffers(1, &app.data.boardBuffers->EBO);

    glDeleteBuffers(1, &app.data.ProjectionUBO);

    delete[] app.data.WhitePieces;
    delete[] app.data.whitePiecesIndicies;
    delete[] app.data.blackPiecesIndicies; 
    delete[] app.data.blackPieces; 
    
    terminateApp(&app); 

    return 0;
}
