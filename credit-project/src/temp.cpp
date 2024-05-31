#include "temp.h"

bool canBePlacedthere(int x, int y) {
    if (x % 2 == 0) {
        return true;
    }
    return false;
}

bool isMate() {
    static int x = 0;
    x += 1; 
    if (x >= 7)
        return true;
    else
        return false; 
}


void bringFront(ChessPiece* piece) {
    piece->vertecies[0].z = 0.5f;
    piece->vertecies[1].z = 0.5f;
    piece->vertecies[2].z = 0.5f;
    piece->vertecies[3].z = 0.5f;
}

void bringBack(ChessPiece* piece) {
    piece->vertecies[0].z = 0.1f;
    piece->vertecies[1].z = 0.1f;
    piece->vertecies[2].z = 0.1f;
    piece->vertecies[3].z = 0.1f;
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
    int move;
    int id; 
    fscanf(file, "%d", &move);
    data->isWhiteMove = move;

    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fscanf(file, "%d ", &id); 
        fscanf(file, "%f %f", &loc.x, &loc.y);

        printf("%f", loc.x);
        printf(" %f \n", loc.y);

        (*data->WhitePieces)[i] = createChessPiece({ loc.x - 4 + 0.5f, loc.y - 4 + 0.5f }, id, WHITE_CHESS_INDEX, data->scaleUI);
    }


    glBindBuffer(GL_ARRAY_BUFFER, data->whitePiecesBuffers->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, data->WhitePieces);

    printf("-----\n");
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        fscanf(file, "%d ", &id);
        fscanf(file, "%f %f", &loc.x, &loc.y);

        (*data->blackPieces)[i] = createChessPiece({ loc.x - 4 + 0.5f, loc.y - 4 + 0.5f }, id, BLACK_CHESS_INDEX, data->scaleUI);
    }
    glBindBuffer(GL_ARRAY_BUFFER, data->blackPiecesBuffers->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChessPiece) * ONE_COLOR_SIZE, data->blackPieces);
    // Zamkniêcie pliku
    fclose(file);
}

void ChangePieceType(ChessPiece* piece, unsigned int chessPieceIndex) {
    
    float posx = chessPieceIndex; 
    piece->vertecies[0].u = posx * CHESS_TEXTURE_UV_WIDTH;
    piece->vertecies[1].u = posx * CHESS_TEXTURE_UV_WIDTH + CHESS_TEXTURE_UV_WIDTH;
    piece->vertecies[2].u = posx * CHESS_TEXTURE_UV_WIDTH + CHESS_TEXTURE_UV_WIDTH;
    piece->vertecies[3].u = posx * CHESS_TEXTURE_UV_WIDTH;

    piece->vertecies[0].pieceId = chessPieceIndex; 
    piece->vertecies[1].pieceId = chessPieceIndex;
    piece->vertecies[2].pieceId = chessPieceIndex;
    piece->vertecies[3].pieceId = chessPieceIndex;

}

bool ChoseNewChessPiece_PopUp(const char* question, AppData* data) {

    static bool shouldChose = false;
    static bool confirmAction = false;
    bool result = false; ;


    ImGui::OpenPopup("Choose Slot"); // Open the dialog popup

    if (ImGui::BeginPopupModal("Choose Slot", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(question);
        if (ImGui::Button("Rook")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;

            ChangePieceType(data->lastClickedPiece, ROOK_TEXTURE_INDEX);
            result =  true; 
        }
        ImGui::SameLine();
        if (ImGui::Button("Bishop")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            ChangePieceType(data->lastClickedPiece, BISHOP_TEXTURE_INDEX);
            result = true;

        }
        ImGui::SameLine();
        if (ImGui::Button("Queen")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            ChangePieceType(data->lastClickedPiece, QUEEN_TEXTURE_INDEX);
            result = true;

        }
        ImGui::SameLine();
        if (ImGui::Button("Knight")) {
            ImGui::CloseCurrentPopup();
            shouldChose = false;
            ChangePieceType(data->lastClickedPiece, KNIGHT_TEXTURE_INDEX);

            result = true;

        }
        ImGui::EndPopup();
    }
    return result; 
  
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
            case 1:
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
    fprintf(file, "%d\n", data.isWhiteMove ? 1 : 0);
    // Zapisanie elementów tablicy do pliku
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fprintf(file, "%d ",((*data.WhitePieces)[i].vertecies->pieceId));
        fprintf(file, "%f", ((*data.WhitePieces)[i].vertecies->x / data.scaleUI) + 4.0f);
        fprintf(file, " %f \n", ((*data.WhitePieces)[i].vertecies->y / data.scaleUI) + 4.0f);
    }
    fprintf(file, "\n");
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {

        fprintf(file, "%d ", ((*data.blackPieces)[i].vertecies->pieceId));
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
