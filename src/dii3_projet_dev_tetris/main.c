#include <snes.h>
#include <stdlib.h>

#include "enums.h"
#include "soundbank.h"

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

#define INITIAL_SPEED 2

extern char snesfont;

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

brrsamples linesound;
brrsamples placesound;

extern char __SOUNDBANK__0;
extern char __SOUNDBANK__1;

extern char m0, m0_end, p0, p0_end, t0, t0_end;

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

piece_t pieces[NOMBRE_PIECES] = {
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

unsigned int fpsCounter;

objet_t piece_obj;
objet_t plateau_obj;

objet_t next_piece_obj;

unsigned int pieceMove = 0;
unsigned int pieceMove2 = 0;
ROTATION lastRotation = DEG_0;
int speed = INITIAL_SPEED;
int speed_boost = 0;

scoMemory score;
bool gameOver = false;
bool pauseGame = false;
unsigned int time_start, time_end;

int getPos(u16 cpt, u16* x, u16* y, int* lignes, int* cols){
    if(piece_obj.rotation & (DEG_0 | DEG_180)){
        *y = cpt / NOMBRE_COLS_PIECE;
        *x = cpt - *y * NOMBRE_COLS_PIECE;
        *lignes = NOMBRE_COLS_PIECE;
        *cols = NOMBRE_LIGNES_PIECE;
    }else if(piece_obj.rotation & (DEG_90 | DEG_270)){
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

void resetPiece(){
    piece_obj.x = plateau_obj.x + (LARGEUR_PLATEAU - NOMBRE_COLS_PIECE) / 2;
    piece_obj.y = plateau_obj.y;
    piece_obj.rotation = DEG_0;
    piece_obj.obj.piece = next_piece_obj.obj.piece;
    
    int nb = rand() & 0x07;
    nb = nb == 7 ? 0 : nb;
    next_piece_obj.obj.piece = pieces[nb];

    time_start = fpsCounter;
    
    showPiece(true);
    WaitForVBlank();
    u8 idSprite = NOMBRE_BLOCS_PIECE;
    u8 i;
    for (i = 0; i < TAILLE_PIECE; ++i)
    {
        if(next_piece_obj.obj.piece.schema[i] == 1 && idSprite < NOMBRE_BLOCS_PIECE * 2){
            u16 y = i / NOMBRE_COLS_PIECE;
            u16 x = i - y * NOMBRE_COLS_PIECE;

            oamSet(idSprite * 4, (next_piece_obj.x + x) * 8, (next_piece_obj.y + y) * 8, 3, 0, 0, next_piece_obj.obj.piece.sprite_offset, 0);
            ++idSprite;
        }
    }
}

void bootScreen(){
    setMode(BG_MODE1,0);
    bgInitTileSet(0, &patterns, &palette, 0, (&patterns_end - &patterns), (&palette_end - &palette), BG_16COLORS, 0x4000);
	bgInitMapSet(0, &map, (&map_end - &map),SC_32x32, 0x0000);

    u8 timer = 0;
    setScreenOn();

	while ( timer<160)
	{
		timer++;
		WaitForVBlank();
	}
	bgInitTileSet(0, &patternslogo, &palettelogo, 0, (&patternslogo_end - &patternslogo), (&palettelogo_end - &palettelogo), BG_16COLORS, 0x4000);
    bgInitMapSet(0, &maplogo, (&maplogo_end - &maplogo),SC_32x32, 0x0000);
	setScreenOn();
	timer = 0;
	while ( timer<160)
	{
		timer++;
		WaitForVBlank();
	}
}

void resetVram(){
    dmaClearVram();

  
    bootScreen();
    consoleInitText(2, 0, &snesfont);
    // Init Sprites gfx and palette with default size of 16x16
	oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end-&gfxpsrite), &palsprite, (&palsprite_end-&palsprite),0, SPRITE_ADDRESS, OBJ_SIZE8);

    bgInitTileSet(0, &gfxpsrite, &palsprite, 1, (&gfxpsrite_end - &gfxpsrite), (&palsprite_end - &palsprite), BG_16COLORS, BACKGROUND1_ADDRESS);
    bgInitTileSet(1, &t0, &p0, 2, (&t0_end - &t0), (&p0_end - &p0), BG_16COLORS, BACKGROUND2_ADDRESS);

    bgSetMapPtr(0, BACKGROUND1_MAP_ADDRESS, SC_32x32);
	bgInitMapSet(1, &m0, (&m0_end - &m0),SC_32x32, BACKGROUND2_MAP_ADDRESS);

    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);
    
    u8 idSprite;
    for (idSprite = 0; idSprite < NOMBRE_BLOCS_PIECE * 2; ++idSprite)
    {
        oamSet(idSprite * 4, 0, 0, 0, 0, 0, 0, 0);
        oamSetEx(idSprite * 4, OBJ_SMALL, OBJ_SHOW);
        oamSetVisible(idSprite * 4,OBJ_SHOW);
    }
    setScreenOn();

    WaitForVBlank();
}

void resetGame(){
    scoreClear(&score);
    gameOver = false;
    speed_boost = 0;
    consoleDrawText(1, 20, "score :");
    consoleDrawText(2, 22, "0        ");

    consoleDrawText(1, 2, "         ");
    consoleDrawText(2, 4, "       ");
    consoleDrawText(1, 5, "          ");

    plateau_obj.x = PLATEAU_X;
    plateau_obj.y = PLATEAU_Y;
    plateau_obj.rotation = DEG_0;

    u16 i;
    for (i = 0; i < TAILLE_PLATEAU * 2; ++i)
    {
        plateau_obj.obj.plateau[i] = 0;
    }
    for (i = 0 ; i < HAUTEUR_PLATEAU; ++i)
    {
        u16 dest_address = BACKGROUND1_MAP_ADDRESS + (plateau_obj.y + i) * LARGEUR_BG1 + plateau_obj.x;
        WaitForVBlank();
        dmaCopyVram(&plateau_obj.obj.plateau[i * LARGEUR_PLATEAU * 2], dest_address, LARGEUR_PLATEAU * 2);
    }
    

    next_piece_obj.x = 3;
    next_piece_obj.y = 10;
    next_piece_obj.rotation = DEG_0;
    int nb = rand() & 0x07;
    nb = nb == 7 ? 0 : nb;
    next_piece_obj.obj.piece = pieces[nb];

    resetPiece();
    spcPlay(1);
	        // Update music / sfx stream and wait vbl
	spcProcess();
}

COLLISION getCollision(){
    COLLISION col = NONE;
    
    u16 y = piece_obj.y - plateau_obj.y;
    u16 x = piece_obj.x - plateau_obj.x;

    u16 piece_cpt;
    u16 plateau_cpt;
    for (piece_cpt = 0; piece_cpt < TAILLE_PIECE; ++piece_cpt)
    {
        if(piece_obj.obj.piece.schema[piece_cpt] == 1){
            u16 piece_y;
            u16 piece_x;
            u16 lignes;
            u16 cols;
            getPos(piece_cpt, &piece_x, &piece_y, &lignes, &cols);

            u16 plateau_cpt = (y + piece_y) * LARGEUR_PLATEAU * 2 + (x + piece_x) * 2;
            
            if(x + piece_x < 0 || x + piece_x > LARGEUR_PLATEAU - 1 || y + piece_y < 0 || y + piece_y > HAUTEUR_PLATEAU - 1 || plateau_obj.obj.plateau[plateau_cpt] != NONE){
                col |= INTERNE;
            }

            if(x + piece_x <= 0 || plateau_obj.obj.plateau[plateau_cpt - 1] != NONE){
                col |= GAUCHE;
            }
            if(x + piece_x >= LARGEUR_PLATEAU - 1 || plateau_obj.obj.plateau[plateau_cpt + 1] != NONE){
                col |= DROITE;
            }
            if(y + piece_y  <= 0 || plateau_obj.obj.plateau[plateau_cpt - LARGEUR_PLATEAU * 2] != NONE){
                col |= HAUT;
            }
            if(y + piece_y >= HAUTEUR_PLATEAU -1 || plateau_obj.obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2] != NONE){
                col |= BAS;
            }
        }
    }
    return col;
}

void rotatePiece(){
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
        getPos(i, &x, &y, &lignes, &cols);
        u16 new_x = cols - 1 - y;
        u16 new_y = x;
        newSchema[new_y * cols + new_x] = piece_obj.obj.piece.schema[i];
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

    memcpy(piece_obj.obj.piece.schema, newSchema, TAILLE_PIECE);

    if(piece_obj.rotation == DEG_270){
        piece_obj.rotation = DEG_0;
    }else{
        piece_obj.rotation = piece_obj.rotation << 1;
    }
}

int removeFullRows(){
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
                if(plateau_obj.obj.plateau[y * LARGEUR_PLATEAU * 2 + x*2] == 0){
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
                    u16 dest_address = BACKGROUND1_MAP_ADDRESS + (plateau_obj.y + new_y) * LARGEUR_BG1 + plateau_obj.x;
                    u16 plateau_cpt = new_y * LARGEUR_PLATEAU * 2;
                    WaitForVBlank();
                    if(y == new_y){
                        for (i = plateau_cpt; i < plateau_cpt + LARGEUR_PLATEAU * 2; ++i)
                        {
                            plateau_obj.obj.plateau[i] = 0;
                        }
                        dmaCopyVram(&plateau_obj.obj.plateau[plateau_cpt], dest_address, LARGEUR_PLATEAU * 2);
                    }else{
                        memcpy(&plateau_obj.obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], &plateau_obj.obj.plateau[plateau_cpt], LARGEUR_PLATEAU * 2);
                        dmaCopyVram(&plateau_obj.obj.plateau[plateau_cpt + LARGEUR_PLATEAU * 2], dest_address + LARGEUR_BG1, LARGEUR_PLATEAU * 2);
                    }
                }
                isrow = 1;
                
                
            }
            
        }
        
    }
   return isrow; 
}

bool movePiece(unsigned short padValue){
    int boost = 0;
    if((padValue & KEY_DOWN) != 0){
        boost = 20;
    }else{
        boost = 0;
    }

    if((padValue & KEY_A) != 0){
        if(lastRotation == piece_obj.rotation){
                rotatePiece();
        }
    }else{
        lastRotation = piece_obj.rotation;
    }

    pieceMove2 += INITIAL_SPEED;
    COLLISION col = getCollision();

    if((col & (INTERNE | HAUT | BAS)) == (HAUT | BAS | INTERNE) && piece_obj.y == plateau_obj.y){
        gameOver = true;
        return true;
    }
    
    if((padValue & KEY_UP) != 0){
        speed = 50;
        while(movePiece(padValue & (~KEY_UP)) != true){
            char messtxt[20] = "";
            sprintf(messtxt,"%d\r\n", piece_obj.y);
            consoleNocashMessage(messtxt);
        }
        return true;
    }
    
    if((col & INTERNE) != 0){
        if(lastRotation != piece_obj.rotation){
            u8 i;
            for (i = 0; i < 3; ++i)
            {
                rotatePiece();
            }
            lastRotation = piece_obj.rotation;
        }
    }

    if(padValue & (KEY_RIGHT | KEY_LEFT | KEY_A) == 0){
        pieceMove2 = 0;
    }

    if(pieceMove2 >= 10){
        if((col & DROITE) == 0 && (padValue & KEY_RIGHT) != 0){
            ++piece_obj.x;
        }else if((col & GAUCHE) == 0 && (padValue & KEY_LEFT) != 0){
            --piece_obj.x;
        } 
        pieceMove2 = 0;
    }

    pieceMove += speed + boost;

    if((col & BAS) == 0 && pieceMove >= 50){
        ++piece_obj.y;
        pieceMove = 0;
    }else if(col & BAS){
        u16 piece_cpt;
        u16 offset = piece_obj.y * LARGEUR_BG1 + piece_obj.x;
        for (piece_cpt = 0; piece_cpt < TAILLE_PIECE; ++piece_cpt)
        {
            if(piece_obj.obj.piece.schema[piece_cpt] == 1){
                u16 y;
                u16 x;
                u16 lignes;
                u16 cols;
                getPos(piece_cpt, &x, &y, &lignes, &cols);

                u16 plateau_cpt = (piece_obj.y - plateau_obj.y + y) * LARGEUR_PLATEAU * 2 + (piece_obj.x - plateau_obj.x + x) * 2;
                u16 dest_address = BACKGROUND1_MAP_ADDRESS + offset + y * LARGEUR_BG1 + x;
                char msg[20] = "";
                consoleNocashMessage(msg);
                memcpy(&plateau_obj.obj.plateau[plateau_cpt], (&mapsprite + piece_obj.obj.piece.background_offset), 2);
                WaitForVBlank();
                dmaCopyVram(&mapsprite + piece_obj.obj.piece.background_offset, dest_address, 2);
            }
        }
        showPiece(false);
        if (removeFullRows()){
            spcPlaySound(1);
        }else
        {
           spcPlaySound(0);
        }
        
        resetPiece();
        
        
         WaitForVBlank();
        return true;
    }
    return false;
}

bool lastGamePauseState = false;

int main(void){
    unsigned short pad0;
    u8 i, idSprite;
    spcBoot();
    consoleInit();

    	// Set give soundbank
	// spcSetBank(&__SOUNDBANK__);
	spcSetBank(&__SOUNDBANK__1);
	spcSetBank(&__SOUNDBANK__0);

    // allocate around 10K of sound ram (39 256-byte blocks)
	spcAllocateSoundRegion(39);

	// Load music
	spcLoad(0);
    setMode(BG_MODE1,0);  bgSetDisable(1);  bgSetDisable(2);
    spcSetModuleVolume(50);
    spcPlay(0);
	// Update music / sfx stream and wait vbl
	spcProcess();
    spcSetSoundEntry(15, 1, 6, &placebrrend-&placebrr, &placebrr, &placesound);
	spcSetSoundEntry(15, 15,  6, &linebrrend-&linebrr, &linebrr, &linesound);
	
    resetVram();
    resetGame();

    while (1)
    {
        pad0 = padsCurrent(0);
        
		spcProcess();

        if(gameOver == true){
             spcStop();
	        // Update music / sfx stream and wait vbl
	        spcProcess();
            showPiece(false);
            consoleDrawText(1, 2, "Game Over");
            consoleDrawText(2, 4, "Press A");
            consoleDrawText(1, 5, "to restart");
            if(pad0 & KEY_A){
                // resetVram();
                resetGame();
            }
        }else{
            if(pad0 & KEY_START){
                if(lastGamePauseState == pauseGame){
                    if(pauseGame == false){
                        pauseGame = true;
                        consoleDrawText(10, 6, "Game paused");
                        consoleDrawText(5, 7, "Press start to resume");
                    }else{
                        pauseGame = false;
                        consoleDrawText(10, 6, "           ");
                        consoleDrawText(5, 7, "                     ");
                    }
                }
            }else{
                lastGamePauseState = pauseGame;
            }

            if(pauseGame == false){
                speed = INITIAL_SPEED + speed_boost;
                movePiece(pad0);

                idSprite = 0;
                for (i = 0; i < TAILLE_PIECE; ++i)
                {
                    if(piece_obj.obj.piece.schema[i] == 1 && idSprite < NOMBRE_BLOCS_PIECE){
                        u16 y;
                        u16 x;
                        u16 lignes;
                        u16 cols;
                        getPos(i, &x, &y, &lignes, &cols);

                        oamSet(idSprite * 4, (piece_obj.x + x) * 8, (piece_obj.y + y) * 8, 3, 0, 0, piece_obj.obj.piece.sprite_offset, 0);
                        ++idSprite;
                    }
                }
            }
        }
        

        // char messtxt[20] = "";
		// sprintf(messtxt,"FPS=%d\r\n", snes_vblank_count);
		// consoleNocashMessage(messtxt);
        ++fpsCounter;
        WaitForVBlank();
    }

    return 0;
}