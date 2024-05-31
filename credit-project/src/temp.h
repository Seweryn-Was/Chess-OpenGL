#pragma once
#include "app.h"




bool canBePlacedthere(int x, int y);

bool isMate();

void bringFront(ChessPiece* piece);
void bringBack(ChessPiece* piece); 

bool ChoseNewChessPiece_PopUp(const char* question, AppData* data);

void ChangePieceType(ChessPiece* piece, unsigned int chessPieceIndex);

void updateChessPieceVBO(unsigned int VBO, unsigned int pos, unsigned int size, ChessPiece* data);

bool checkCursorHoverChessPieces(AppData* data, vec2* hoveredField, ChessPiece*& clickedPiece, int* index, int chessPieceColor);

void translateChessPiece(ChessPiece* clickedPiece, vec2 offset, vec2* totalOffset);

void setChessPiecePosOnBoard(ChessPiece* clickedPiece, vec2 posOnBoard, float scaleUI);

void loadFromFile(const char* loadFile, AppData* data);

void ChoseSlotToLoadGame_PopUp(const char* question, AppData* data);

void saveToFIle(const char* slotFile, AppData data);

void ChoseSlotToSaveGame_PopUp(const char* question, AppData data);
