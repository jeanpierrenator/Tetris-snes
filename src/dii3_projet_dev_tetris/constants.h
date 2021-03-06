#ifndef CONSTANTS_H
#define CONSTANTS_H
#define SPRITE_ADDRESS 0x4000
#define BACKGROUND1_ADDRESS 0x5000
#define BACKGROUND1_MAP_ADDRESS 0x1000

#define BACKGROUND2_ADDRESS 0x6000
#define BACKGROUND2_MAP_ADDRESS 0x1400

#define LARGEUR_BG1 32
#define HAUTEUR_BG1 32

#define LARGEUR_PLATEAU 10
#define HAUTEUR_PLATEAU 22
#define TAILLE_PLATEAU LARGEUR_PLATEAU * HAUTEUR_PLATEAU

#define NOMBRE_PIECES 7
#define NOMBRE_BLOCS_PIECE 4
#define TAILLE_PIECE 8
#define NOMBRE_COLS_PIECE 4
#define NOMBRE_LIGNES_PIECE 2

#define PLATEAU_X 14
#define PLATEAU_Y 3

#define PLATEAU_X_P1 5
#define PLATEAU_Y_P1 4

#define PLATEAU_X_P2 17
#define PLATEAU_Y_P2 4

#define NEXT_PIECE_X 3
#define NEXT_PIECE_Y 3

#define NEXT_PIECE_X_P1 0
#define NEXT_PIECE_Y_P1 3

#define NEXT_PIECE_X_P2 28
#define NEXT_PIECE_Y_P2 3

#define INITIAL_SPEED 2
#endif