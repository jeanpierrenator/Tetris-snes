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
void showPiece(bool visible, player_t * player){
    int show = visible ? OBJ_SHOW : OBJ_HIDE;
    u8 idSprite =player->idSpritePiece;
    WaitForVBlank();
    for (; idSprite < player->idSpritePiece+4; ++idSprite)
    {
        oamSetVisible(idSprite * 4, show);
    }
}
void resetPiece(player_t * player){
    player->piece->x = player->plateau->x + (LARGEUR_PLATEAU - NOMBRE_COLS_PIECE) / 2;
    player->piece->y = player->plateau->y;
    player->piece->rotation = DEG_0;
    player->piece->obj.piece = player->next_piece->obj.piece;
    
    int nb = fpsCounter & 0x07;
    nb = nb == 7 ? 0 : nb;
    player->next_piece->obj.piece = listPieces2[nb];
    time_start = fpsCounter;
    
    showPiece(true,player);
    WaitForVBlank();
    u8 idSprite = player->idSpriteNextPiece;
    u8 i;
    for (i = 0; i < TAILLE_PIECE; ++i)
    {
        if(player->next_piece->obj.piece.schema[i] == 1 && idSprite < player->idSpriteNextPiece+4){
            u16 y = i / NOMBRE_COLS_PIECE;
            u16 x = i - y * NOMBRE_COLS_PIECE;
            oamSet(idSprite * 4, (player->next_piece->x + x) * 8, (player->next_piece->y + y) * 8, 3, 0, 0, player->next_piece->obj.piece.sprite_offset, 0);
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
        if(piece_obj_ref->obj.piece.schema[piece_cpt] == 1 ){ 
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
            if(x + piece_x >= LARGEUR_PLATEAU - 1 || plateau_obj_ref->obj.plateau[plateau_cpt +2] != NONE){
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
int removeFullRows(player_t * player){
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
                if(player->plateau->obj.plateau[y * LARGEUR_PLATEAU * 2 + x*2] == 0){
                    full_row = false;
                    break;
                }
            }
            if(full_row == true){
                
                for (new_y = y ; new_y > -1; --new_y)
                {
                    u16 dest_address = BACKGROUND1_MAP_ADDRESS + (player->plateau->y + new_y) * LARGEUR_BG1 + player->plateau->x;
                    u16 plateau_cpt = new_y * LARGEUR_PLATEAU * 2;
                    WaitForVBlank();
                    if(y == new_y){
                        for (i = plateau_cpt; i < plateau_cpt + LARGEUR_PLATEAU * 2; ++i)
                        {
                            player->plateau->obj.plateau[i] = 0;
                        }
                        dmaCopyVram(&player->plateau->obj.plateau[plateau_cpt], dest_address, LARGEUR_PLATEAU * 2);
                    }else{
                        memcpy(&player->plateau->obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], &player->plateau->obj.plateau[plateau_cpt], LARGEUR_PLATEAU * 2);
                        dmaCopyVram(&player->plateau->obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], dest_address + LARGEUR_BG1, LARGEUR_PLATEAU * 2);
                    }
                }
                isrow = 1;
                isRowDeleted++;
                
                
            }
            
        }
        
    }
   return isrow; 
}
bool movePiece(unsigned short padValue,player_t *player){
    int boost = 0;
    unsigned int speed =0;
    if((padValue & KEY_DOWN) != 0){
        boost = 20;
    }else{
        boost = 0;
    }

    if((padValue & KEY_A) != 0){
        if(player->lastPieceRotation == player->piece->rotation){
                rotatePiece(player->piece);
        }
    }else{
        player->lastPieceRotation = player->piece->rotation;
    }

    player->pieceMove2 += INITIAL_SPEED;
    COLLISION col = getCollision(player->piece,player->plateau);

    if((col & (INTERNE | HAUT | BAS)) == (HAUT | BAS | INTERNE) && player->piece->y == player->plateau->y){
        gameOver = true;
        looser = player->id;   
        return true;
    }
    
    if((padValue & KEY_UP) != 0 ){
        player->pieceMove = 50;
        while (!(col & BAS))
        {
            player->piece->y++;
            col = getCollision(player->piece,player->plateau);
        }
    }
    
    if((col & INTERNE) != 0){
        if(player->lastPieceRotation != player->piece->rotation){
            u8 i;
            for (i = 0; i < 3; ++i)
            {
                rotatePiece(player->piece);
            }
            player->lastPieceRotation = player->piece->rotation;
        }
    }

    if(padValue & (KEY_RIGHT | KEY_LEFT | KEY_A) == 0){
        player->pieceMove2 = 0;
    }

   
      if(player->pieceMove2 >= 7){
        if((col & GAUCHE) == 0 && (padValue & KEY_LEFT) != 0){
            --player->piece->x;
        }else if(!(col & DROITE) && (padValue & KEY_RIGHT)){
            player->piece->x++;
        } 
        player->pieceMove2 = 0;
    }
   
    speed = player->speed;
    player->pieceMove += speed + boost;
     col = getCollision(player->piece,player->plateau);
    if((col & BAS) == 0 && player->pieceMove >= 50 ){
        ++player->piece->y;
        player->pieceMove = 0;
   }else if((col & BAS) && (padValue & KEY_RIGHT) != 0 && !(col & DROITE) ){
       player->piece->x++;
       col = getCollision(player->piece,player->plateau);
   }
   else if((col & BAS) && (padValue & KEY_LEFT) != 0 && !(col & GAUCHE) ){
       player->piece->x--;
       col = getCollision(player->piece,player->plateau);
   }
    if((col & BAS)){
        u16 piece_cpt;
        u16 offset = player->piece->y * LARGEUR_BG1 + player->piece->x;
        for (piece_cpt = 0; piece_cpt < TAILLE_PIECE; ++piece_cpt)
        {
            if(player->piece->obj.piece.schema[piece_cpt] == 1){
                u16 y;
                u16 x;
                u16 lignes;
                u16 cols;
                getPos(piece_cpt, &x, &y, &lignes, &cols,player->piece);

                u16 plateau_cpt = (player->piece->y - player->plateau->y + y) * LARGEUR_PLATEAU * 2 + (player->piece->x - player->plateau->x + x) * 2;
                u16 dest_address = BACKGROUND1_MAP_ADDRESS + offset + y * LARGEUR_BG1 + x;
                char msg[20] = "";
                consoleNocashMessage(msg);
                memcpy(&player->plateau->obj.plateau[plateau_cpt], (&mapsprite + player->piece->obj.piece.background_offset), 2);
                WaitForVBlank();
                dmaCopyVram(&mapsprite + player->piece->obj.piece.background_offset, dest_address, 2);
            }
        }
        showPiece(false ,player);
        if (removeFullRows(player)){
            spcPlaySound(1);
        }else
        {
           spcPlaySound(0);
        }
        
        resetPiece(player);
        
        
         WaitForVBlank();
        return true;
    }
   
     
    return false;
}

void setPieceInMemory(player_t * player){  
    u8 i;
    u8 idSprite = player->idSpritePiece;
    for (i = 0; i < TAILLE_PIECE; ++i)
                {
                    if(player->piece->obj.piece.schema[i] == 1 && idSprite < player->idSpritePiece+4){
                        u16 y;
                        u16 x;
                        u16 lignes;
                        u16 cols;
                        getPos(i, &x, &y, &lignes, &cols,player->piece);

                        oamSet(idSprite * 4, (player->piece->x + x) * 8, (player->piece->y + y) * 8, 3, 0, 0, player->piece->obj.piece.sprite_offset, 0);
                        ++idSprite;
                    }
                }
}