#include"gameplay.h"
piece_t listPieces2[NOMBRE_PIECES] = {
    {
        1,
        0x02,
        {1,1,0,0,0,1,1,0},
        ROUGE,
    },
    {
        2,
        0x04,
        {1,1,0,0,1,1,0,0},
        JAUNE,
    },
    {
        3,
        0x06,
        {1,1,1,0,0,1,0,0},
        VIOLET,
    },
    {
        4,
        0x08,
        {1,1,1,0,1,0,0,0},
        ORANGE,
    },
    {
        5,
        0x0a,
        {0,1,1,0,1,1,0,0},
        VERT,
    },
    {
        6,
        0x0c,
        {1,1,1,1,0,0,0,0},
        BlEU_CIEL,
    },
    {
        7,
        0x0e,
        {1,1,1,0,0,0,1,0},
        BLEU,
    },
};
int getPos(u16 cpt, u16* x, u16* y, int* lignes, int* cols,objet_t *piece_obj_ref){
    if(piece_obj_ref->rotation & (DEG_0 | DEG_180)){
        *y = cpt / NOMBRE_COLS_PIECE;
        *x = cpt - *y * NOMBRE_COLS_PIECE;
        *lignes = NOMBRE_COLS_PIECE;
        *cols = NOMBRE_LIGNES_PIECE;
    }else if(piece_obj_ref->rotation & (DEG_90 | DEG_270)){
        *y = cpt / NOMBRE_LIGNES_PIECE;
        *x = cpt - *y * NOMBRE_LIGNES_PIECE;
        *lignes = NOMBRE_LIGNES_PIECE;
        *cols = NOMBRE_COLS_PIECE;
    }
}
void showPiece(bool visible){
    int show = visible ? OBJ_SHOW : OBJ_HIDE;
    u8 idSprite;
    WaitForVBlank();
    for (idSprite = 0; idSprite < NOMBRE_BLOCS_PIECE; ++idSprite)
    {
        oamSetVisible(idSprite * 4, show);
    }
}
void resetPiece(objet_t * piece_obj_ref,objet_t *plateau_obj_ref,objet_t *next_piece_obj_ref){
    piece_obj_ref->x = plateau_obj_ref->x + (LARGEUR_PLATEAU - NOMBRE_COLS_PIECE) / 2;
    piece_obj_ref->y = plateau_obj_ref->y;
    piece_obj_ref->rotation = DEG_0;
    piece_obj_ref->obj.piece = next_piece_obj_ref->obj.piece;
    
    int nb = fpsCounter & 0x07;
    nb = nb == 7 ? 0 : nb;
    next_piece_obj_ref->obj.piece = listPieces2[nb];

    time_start = fpsCounter;
    
    showPiece(true);
    WaitForVBlank();
    u8 idSprite = NOMBRE_BLOCS_PIECE;
    u8 i;
    for (i = 0; i < TAILLE_PIECE; ++i)
    {
        if(next_piece_obj_ref->obj.piece.schema[i] == 1 && idSprite < NOMBRE_BLOCS_PIECE * 2){
            u16 y = i / NOMBRE_COLS_PIECE;
            u16 x = i - y * NOMBRE_COLS_PIECE;

            oamSet(idSprite * 4, (next_piece_obj_ref->x + x) * 8, (next_piece_obj_ref->y + y) * 8, 3, 0, 0, next_piece_obj_ref->obj.piece.sprite_offset, 0);
            ++idSprite;
        }
    }
}
COLLISION getCollision(objet_t * piece_obj_ref,objet_t * plateau_obj_ref){
    COLLISION col = NONE;
    
    u16 y = piece_obj_ref->y - plateau_obj_ref->y;
    u16 x = piece_obj_ref->x - plateau_obj_ref->x;

    u16 piece_cpt;
    u16 plateau_cpt;
    for (piece_cpt = 0; piece_cpt < TAILLE_PIECE; ++piece_cpt)
    {
        if(piece_obj_ref->obj.piece.schema[piece_cpt] == 1){
            u16 piece_y;
            u16 piece_x;
            u16 lignes;
            u16 cols;
            getPos(piece_cpt, &piece_x, &piece_y, &lignes, &cols,piece_obj_ref);

            u16 plateau_cpt = (y + piece_y) * LARGEUR_PLATEAU * 2 + (x + piece_x) * 2;
            
            if(x + piece_x < 0 || x + piece_x > LARGEUR_PLATEAU - 1 || y + piece_y < 0 || y + piece_y > HAUTEUR_PLATEAU - 1 || plateau_obj_ref->obj.plateau[plateau_cpt] != NONE){
                col |= INTERNE;
            }

            if(x + piece_x <= 0 || plateau_obj_ref->obj.plateau[plateau_cpt - 1] != NONE){
                col |= GAUCHE;
            }
            if(x + piece_x >= LARGEUR_PLATEAU - 1 || plateau_obj_ref->obj.plateau[plateau_cpt + 1] != NONE){
                col |= DROITE;
            }
            if(y + piece_y  <= 0 || plateau_obj_ref->obj.plateau[plateau_cpt - LARGEUR_PLATEAU * 2] != NONE){
                col |= HAUT;
            }
            if(y + piece_y >= HAUTEUR_PLATEAU -1 || plateau_obj_ref->obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2] != NONE){
                col |= BAS;
            }
        }
    }
    return col;
}
void rotatePiece(objet_t * piece_obj_ref){
    u8 newSchema[8];

    u16 i;
    u16 y;
    u16 x;
    u16 lignes;
    u16 cols;

    for (i = 0; i < TAILLE_PIECE; ++i)
    {
        newSchema[i] = 0;
    }

    for (i = 0; i < TAILLE_PIECE; ++i)
    {
        getPos(i, &x, &y, &lignes, &cols,piece_obj_ref);
        u16 new_x = cols - 1 - y;
        u16 new_y = x;
        newSchema[new_y * cols + new_x] = piece_obj_ref->obj.piece.schema[i];
    }

    bool empty_row = true;
    while (empty_row)
    {
        for (i = 0; i < TAILLE_PIECE; i += cols)
        {
            if(newSchema[i] == 1){
                empty_row = false;
            }
        }
        if(empty_row == true){
            for (i = 0; i < TAILLE_PIECE; ++i)
            {
                y = i / cols;
                x = i - y * cols;
                if(x > 0){
                    newSchema[i - 1] = newSchema[i];
                }
            }
            for (i = cols - 1; i < TAILLE_PIECE; i += cols)
            {
                newSchema[i] = 0;
            }
        }
    }

    memcpy(piece_obj_ref->obj.piece.schema, newSchema, TAILLE_PIECE);

    if(piece_obj_ref->rotation == DEG_270){
        piece_obj_ref->rotation = DEG_0;
    }else{
        piece_obj_ref->rotation = piece_obj_ref->rotation << 1;
    }
}
int removeFullRows(objet_t * plateau_obj_ref){
    int i;
    u16 y;
    u16 x;
    bool full_row = true;
    int new_y;
    int isrow =0;
    
    while (full_row == true)
    {
        for (y = 0; y < HAUTEUR_PLATEAU; ++y)
        {
            full_row = true;
            for (x = 0; x < LARGEUR_PLATEAU; ++x)
            {
                if(plateau_obj_ref->obj.plateau[y * LARGEUR_PLATEAU * 2 + x*2] == 0){
                    full_row = false;
                    break;
                }
            }
            if(full_row == true){
                time_end = fpsCounter;
                int bonus;
               if(time_end - time_start <= 100){
                    bonus = (~(time_end - time_start)) & 0xff;
                }
                scoreAdd(&score, 10 + bonus);
                consoleDrawText(2, 22, "%d", (u16)score.scolo);
                if((score.scolo & 0x14) != 0 && speed <= 40){
                    ++speed_boost;
                }
                for (new_y = y ; new_y > -1; --new_y)
                {
                    u16 dest_address = BACKGROUND1_MAP_ADDRESS + (plateau_obj_ref->y + new_y) * LARGEUR_BG1 + plateau_obj_ref->x;
                    u16 plateau_cpt = new_y * LARGEUR_PLATEAU * 2;
                    WaitForVBlank();
                    if(y == new_y){
                        for (i = plateau_cpt; i < plateau_cpt + LARGEUR_PLATEAU * 2; ++i)
                        {
                            plateau_obj_ref->obj.plateau[i] = 0;
                        }
                        dmaCopyVram(&plateau_obj_ref->obj.plateau[plateau_cpt], dest_address, LARGEUR_PLATEAU * 2);
                    }else{
                        memcpy(&plateau_obj_ref->obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], &plateau_obj_ref->obj.plateau[plateau_cpt], LARGEUR_PLATEAU * 2);
                        dmaCopyVram(&plateau_obj_ref->obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], dest_address + LARGEUR_BG1, LARGEUR_PLATEAU * 2);
                    }
                }
                isrow = 1;
                
                
            }
            
        }
        
    }
   return isrow; 
}
bool movePiece(unsigned short padValue,objet_t *piece_obj_ref,objet_t *plateau_obj_ref,objet_t * next_piece_obj_ref){
    int boost = 0;
    if((padValue & KEY_DOWN) != 0){
        boost = 20;
    }else{
        boost = 0;
    }

    if((padValue & KEY_A) != 0){
        if(lastRotation == piece_obj_ref->rotation){
                rotatePiece(piece_obj_ref);
        }
    }else{
        lastRotation = piece_obj_ref->rotation;
    }

    pieceMove2 += INITIAL_SPEED;
    COLLISION col = getCollision(piece_obj_ref,plateau_obj_ref);

    if((col & (INTERNE | HAUT | BAS)) == (HAUT | BAS | INTERNE) && piece_obj_ref->y == plateau_obj_ref->y){
        gameOver = true;
        return true;
    }
    
    if((padValue & KEY_UP) != 0){
        speed = 50;
        while(movePiece(padValue & (~KEY_UP),piece_obj_ref,plateau_obj_ref,next_piece_obj_ref) != true){
            char messtxt[20] = "";
            sprintf(messtxt,"%d\r\n", piece_obj_ref->y);
            consoleNocashMessage(messtxt);
        }
        return true;
    }
    
    if((col & INTERNE) != 0){
        if(lastRotation != piece_obj_ref->rotation){
            u8 i;
            for (i = 0; i < 3; ++i)
            {
                rotatePiece(piece_obj_ref);
            }
            lastRotation = piece_obj_ref->rotation;
        }
    }

    if(padValue & (KEY_RIGHT | KEY_LEFT | KEY_A) == 0){
        pieceMove2 = 0;
    }

    if(pieceMove2 >= 10){
        if((col & DROITE) == 0 && (padValue & KEY_RIGHT) != 0){
            ++piece_obj_ref->x;
        }else if((col & GAUCHE) == 0 && (padValue & KEY_LEFT) != 0){
            --piece_obj_ref->x;
        } 
        pieceMove2 = 0;
    }

    pieceMove += speed + boost;

    if((col & BAS) == 0 && pieceMove >= 50){
        ++piece_obj_ref->y;
        pieceMove = 0;
   }else if(col & BAS){
        u16 piece_cpt;
        u16 offset = piece_obj_ref->y * LARGEUR_BG1 + piece_obj_ref->x;
        for (piece_cpt = 0; piece_cpt < TAILLE_PIECE; ++piece_cpt)
        {
            if(piece_obj_ref->obj.piece.schema[piece_cpt] == 1){
                u16 y;
                u16 x;
                u16 lignes;
                u16 cols;
                getPos(piece_cpt, &x, &y, &lignes, &cols,piece_obj_ref);

                u16 plateau_cpt = (piece_obj_ref->y - plateau_obj_ref->y + y) * LARGEUR_PLATEAU * 2 + (piece_obj_ref->x - plateau_obj_ref->x + x) * 2;
                u16 dest_address = BACKGROUND1_MAP_ADDRESS + offset + y * LARGEUR_BG1 + x;
                char msg[20] = "";
                consoleNocashMessage(msg);
                memcpy(&plateau_obj_ref->obj.plateau[plateau_cpt], (&mapsprite + piece_obj_ref->obj.piece.background_offset), 2);
                WaitForVBlank();
                dmaCopyVram(&mapsprite + piece_obj_ref->obj.piece.background_offset, dest_address, 2);
            }
        }
        showPiece(false);
        if (removeFullRows(plateau_obj_ref)){
            spcPlaySound(1);
        }else
        {
           spcPlaySound(0);
        }
        
        resetPiece(piece_obj_ref,plateau_obj_ref,next_piece_obj_ref);
        
        
         WaitForVBlank();
        return true;
    }
    return false;
}

void setPieceInMemory(objet_t * piece_obj_ref,u8 idSprite){  
    u8 i;
    for (i = 0; i < TAILLE_PIECE; ++i)
                {
                    if(piece_obj_ref->obj.piece.schema[i] == 1 && idSprite < NOMBRE_BLOCS_PIECE){
                        u16 y;
                        u16 x;
                        u16 lignes;
                        u16 cols;
                        getPos(i, &x, &y, &lignes, &cols,piece_obj_ref);

                        oamSet(idSprite * 4, (piece_obj_ref->x + x) * 8, (piece_obj_ref->y + y) * 8, 3, 0, 0, piece_obj_ref->obj.piece.sprite_offset, 0);
                        ++idSprite;
                    }
                }
}