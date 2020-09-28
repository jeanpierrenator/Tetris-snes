#ifndef ENUMS_H
#define ENUMS_H

typedef enum ACTION{
    NONE = 0,
    HAUT = 1,
    DROITE = 2,
    BAS = 4,
    GAUCHE = 8,
    ROTATION = 16
} ACTION;

typedef enum ROTATION{
    DEG_0 = 1 << 0,
    DEG_90 = 1 << 1,
    DEG_180 = 1 << 2,
    DEG_270 = 1 << 3,
} ROTATION;

typedef enum COLLISION{
    NONE = 0,
    HAUT = 1 << 0,
    BAS = 1 << 1,
    GAUCHE = 1 << 2,
    DROITE = 1 << 3,
    INTERNE = 1 << 4
} COLLISION;

typedef enum COULEUR{
    NONE = 0,
    GRIS = 1,
    BlEU_CIEL = 2,
    JAUNE = 3,
    VIOLET = 4,
    ORANGE = 5,
    BLEU = 6,
    ROUGE = 7,
    VERT = 8
} COULEUR;

#endif