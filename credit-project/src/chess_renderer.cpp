#include "chess_renderer.h"
chessBoardSquare createBoardSquare(vec3 position, vec3 color, float scale)
 {
     chessBoardSquare square;
     square.point[0] = { (position.x - 0.5f)*scale, (position.y - 0.5f)*scale, position.z, color };
     square.point[1] = { (position.x + 0.5f)*scale, (position.y - 0.5f)*scale, position.z, color };
     square.point[2] = { (position.x + 0.5f)*scale, (position.y + 0.5f)*scale,   position.z, color };
     square.point[3] = { (position.x - 0.5f)*scale, (position.y + 0.5f)*scale,   position.z, color };
     /*
        3----2
        |  / |
        | /  |
        0----1
     */

     return square;
}
bool CheckIfCursorHoversChessPiece(ChessPiece piece, vec2 boardPos, vec2 cursorPos) {
    vec3 min = { piece.vertecies[0].x + boardPos.x, piece.vertecies[0].y + boardPos.y };
    vec3 max = { piece.vertecies[2].x + boardPos.x, piece.vertecies[2].y + boardPos.y };
    if (cursorPos.x < max.x && cursorPos.y<max.y && cursorPos.x>min.x && cursorPos.y > min.y) {
        return true;
    }
    return false;
}

bool checkCursorHover(chessBoardSquare boardLocalVert[], int size, vec2 boardPos, vec2 cursorPos, vec2* fieldIndex) {
    for (int i = 0; i < size; i++) {
        chessBoardSquare square = boardLocalVert[i];
        vec3 min = { square.point[0].position.x + boardPos.x,square.point[0].position.y + boardPos.y };
        vec3 max = { square.point[2].position.x + boardPos.x,square.point[2].position.y + boardPos.y };

        if (cursorPos.x < max.x && cursorPos.y<max.y && cursorPos.x>min.x && cursorPos.y > min.y) {

            int x = i % 8, y = i / 8;
            //printf("HOVER: %d, %d\n", x, y); 
            fieldIndex->x = x;
            fieldIndex->y = y;
            return true;
        }
    }
    fieldIndex->x = -1;
    fieldIndex->y = -1;
    return false;
}

void CreateBoardBuffer(chessBoardSquare* board, uivec3* boardIndicies, float scale) {
    //board must have 64 number of element 
    //boardIndicies must have 2*64 cause every square has 2 triangles

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int squareIndex = BOARD_SIDE_SIZE * y + x;
            vec3 pos = { x - 3.5f, y - 3.5f, 0.0 };
            vec3 color{};

            int colorIndex = ((squareIndex % 4) + (y % 2)) % 2;
            if (colorIndex == 0) {
                color = COLOR_2;
            }
            else {
                color = COLOR_1;
            }

            board[squareIndex] = createBoardSquare(pos, color,scale);
            unsigned int offset = 4 * squareIndex;
            boardIndicies[2 * squareIndex] = { 0 + offset , 1 + offset, 2 + offset };
            boardIndicies[2 * squareIndex + 1] = { 2 + offset, 3 + offset, 0 + offset };
        }
    }
}

bool createChessAtlasTexture(unsigned int *textureId, const char* path) {
    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);


    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return true; 
    }

    std::cout << "[Stb_image]Failed to load texture\n";
    stbi_image_free(data);
    return false; 
}


ChessPiece createChessPiece(vec2 positionOnBoard, unsigned int chessPieceIndex, unsigned int chessPieceColorIndex, float scale) {
    ChessPiece toReturn = {};
    float xlen = CHESS_TEXTURE_UV_WIDTH, ylen = CHESS_TEXTURE_UV_HEIGHT;
    float posx = chessPieceIndex, posy = chessPieceColorIndex;
    vec2 relativePos = { positionOnBoard.x * scale, positionOnBoard.y * scale };
    toReturn.vertecies[0] = { -0.5f * scale + relativePos.x, -0.5f * scale + relativePos.y, 0.0, posx * xlen       , posy * ylen, chessPieceIndex };
    toReturn.vertecies[1] = { 0.5f * scale + relativePos.x, -0.5f * scale + relativePos.y, 0.0, posx * xlen + xlen, posy * ylen , chessPieceIndex };
    toReturn.vertecies[2] = { 0.5f * scale + relativePos.x,  0.5f * scale + relativePos.y, 0.0, posx * xlen + xlen, posy * ylen + ylen, chessPieceIndex };
    toReturn.vertecies[3] = { -0.5f * scale + relativePos.x,  0.5f * scale + relativePos.y, 0.0, posx * xlen,        posy * ylen + ylen  , chessPieceIndex };
    return toReturn;
}

//bool checkCursorHoverChessPieces(AppData* data, vec2* hoveredField) {
//    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
//        ChessPiece current = (*data->WhitePieces)[i]; //(*appdata.WhitePieces)[i]; 
//        //current.vertecies[0].x; )
//
//        vec3 min = { current.vertecies[0].x + data->boardScreenCoords.x, current.vertecies[0].y + data->boardScreenCoords.y };
//        vec3 max = { current.vertecies[2].x + data->boardScreenCoords.x, current.vertecies[2].y + data->boardScreenCoords.y };
//        vec2 cursorPos = data->cursorScreenCoords;
//        if (cursorPos.x < max.x && cursorPos.y<max.y && cursorPos.x>min.x && cursorPos.y > min.y) {
//            printf("HOVER PRESS");
//            checkCursorHover(data->boardLocalVert[0], 64, data->boardScreenCoords, data->cursorScreenCoords, hoveredField);
//            return true;
//        }
//    }
//    return false;
//}