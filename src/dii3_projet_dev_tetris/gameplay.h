#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "structures.h"

extern unsigned int fpsCounter;
extern unsigned int time_start;
extern unsigned int time_end;
extern scoMemory score;
extern ROTATION lastRotation;

extern bool gameOver ;
extern bool pauseGame ;
extern bool isRowDeleted;


extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;
extern char mapsprite, mapsprite_end;
extern char linebrr,linebrrend;
extern char placebrr,placebrrend;

extern char patterns, patterns_end;
extern char palette, palette_end;
extern char map, map_end;

extern char patternslogo, patternslogo_end;
extern char palettelogo, palettelogo_end;
extern char maplogo, maplogo_end;
extern u8 looser;

int getPos(u16 cpt, u16* x, u16* y, int* lignes, int* cols,objet_t *piece_obj_ref);
void showPiece(bool visible, player_t * player);
void resetPiece(player_t * player);
COLLISION getCollision(objet_t * piece_obj_ref,objet_t * plateau_obj_ref);
void rotatePiece(objet_t * piece_obj_ref);
int removeFullRows(player_t * player);
bool movePiece(unsigned short padValue,player_t * player);
void setPieceInMemory(player_t * player);

#endif