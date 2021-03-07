#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <snes.h>
#include <stdlib.h>
#include "constants.h"
#include "enums.h"
typedef struct pos_s
{
    int x;
    int y;
} pos_t;




typedef struct piece_s
{
    u8 sprite_offset;
    u8 background_offset;
    u8 schema[TAILLE_PIECE];
    COULEUR couleur;
} piece_t;

typedef union obj_s
{
    piece_t piece;
    u8 plateau[TAILLE_PLATEAU*2];
} obj_t;

typedef struct objet_s
{
    int x;
    int y;
    ROTATION rotation;
    obj_t obj;
} objet_t;

typedef struct player_s
{
    u8 id;
    objet_t *  plateau;
    objet_t * piece;
    objet_t * next_piece;
    u32 score;
    u8 idSpritePiece;
    u8 idSpriteNextPiece ;
    ROTATION lastPieceRotation;
}player_t;



#endif