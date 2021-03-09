#include <snes.h>
#include <stdlib.h>

#include "soundbank.h"
#include "gameplay.h"

void mode2player();

void mode1player();
extern char snesfont;

extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;
extern char mapsprite, mapsprite_end;
extern char linebrr, linebrrend;
extern char placebrr, placebrrend;

extern char patterns, patterns_end;
extern char palette, palette_end;
extern char map, map_end;

extern char patternslogo, patternslogo_end;
extern char palettelogo, palettelogo_end;
extern char maplogo, maplogo_end;

extern char menubg, menubg_end;
extern char menubgPalette, menubgPalette_end;
extern char menubgMap, menubgMap_end;

extern char menubg2, menubg2_end;
extern char menubg2Palette, menubg2Palette_end;
extern char menubg2Map, menubg2Map_end;

extern char menubg3, menubg3_end;
extern char menubg3Palette, menubg3Palette_end;
extern char menubg3Map, menubg3Map_end;

extern char menubg4, menubg4_end;
extern char menubg4Palette, menubg4Palette_end;
extern char menubg4Map, menubg4Map_end;

brrsamples linesound;
brrsamples placesound;

extern char __SOUNDBANK__0;
extern char __SOUNDBANK__1;

extern char m0, m0_end, p0, p0_end, t0, t0_end;
extern char m2, m2_end, p2, p2_end, t2, t2_end;

unsigned int fpsCounter;

objet_t piece_obj;
objet_t plateau_obj;

objet_t next_piece_obj;

objet_t piece_obj2;
objet_t plateau_obj2;

objet_t next_piece_obj2;

player_t player1;
player_t player2;
scoMemory score;

u8 twoPlayer = 0;
u8 isRowDeleted = 0;
u8 looser = 0;



bool gameOver = false;
bool pauseGame = false;
unsigned int time_start, time_end;

piece_t listPieces[NOMBRE_PIECES] = {
    {
        1,
        0x02,
        {1, 1, 0, 0, 0, 1, 1, 0},
        ORANGE, 
    },
    {
        2,
        0x04,
        {1, 1, 0, 0, 1, 1, 0, 0},
        JAUNE,
    },
    {
        3,
        0x06,
        {1, 1, 1, 0, 0, 1, 0, 0},
        VIOLET,
    },
    {
        4,
        0x08,
        {1, 1, 1, 0, 1, 0, 0, 0},
        ROUGE,
    },
    {
        5,
        0x0a,
        {0, 1, 1, 0, 1, 1, 0, 0},
        VERT,
    },
    {
        6,
        0x0c,
        {1, 1, 1, 1, 0, 0, 0, 0},
        BlEU_CIEL,
    },
    {
        7,
        0x0e,
        {1, 1, 1, 0, 0, 0, 1, 0},
        BLEU,
    },
};

void bootScreen()
{

    setMode(BG_MODE1, 0);
    bgInitTileSet(0, &patterns, &palette, 0, (&patterns_end - &patterns), (&palette_end - &palette), BG_16COLORS, 0x5000);
    bgInitMapSet(0, &map, (&map_end - &map), SC_32x32, 0x0000);

    u8 timer = 0;
    setScreenOn();

    while (timer < 160)
    {
        timer++;
        WaitForVBlank();
    }
    bgInitTileSet(0, &patternslogo, &palettelogo, 0, (&patternslogo_end - &patternslogo), (&palettelogo_end - &palettelogo), BG_16COLORS, 0x5000);
    bgInitMapSet(0, &maplogo, (&maplogo_end - &maplogo), SC_32x32, 0x0000);
    setScreenOn();
    timer = 0;
    while (timer < 160)
    {
        timer++;
        WaitForVBlank();
    }
    dmaClearVram();
}

void menu()
{
    consoleInit();
    dmaClearVram();
    oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end - &gfxpsrite), &palsprite, (&palsprite_end - &palsprite), 0, SPRITE_ADDRESS, OBJ_SIZE8);
    spcLoad(MOD_GALGOX_BLUEFIRE);
    spcPlay(MOD_GALGOX_BLUEFIRE);
    spcSetModuleVolume(100);
    spcProcess();
    short scrX1 = 0, scrY1 = 60;
    short scrX2 = 0, scrY2 = 0;
    short scrY3 = 0;
    short sens = 0, sens1 = 0;
    short timer = 0;
    unsigned short pad0 = 0;
    bgInitTileSet(2, &menubg, &menubgPalette, 0, (&menubg_end - &menubg), 16 * 2, BG_16COLORS, 0x3000);
    bgInitTileSet(1, &menubg3, &menubg3Palette, 1, (&menubg3_end - &menubg3), 16 * 2, BG_16COLORS, 0x2000);
    bgInitTileSet(0, &menubg2, &menubg2Palette, 2, (&menubg2_end - &menubg2), 16 * 2, BG_16COLORS, 0x1000);

    bgInitMapSet(2, &menubgMap, (&menubgMap_end - &menubgMap), SC_32x32, 0x0000);
    bgInitMapSet(1, &menubg3Map, (&menubg3Map_end - &menubg3Map), SC_32x32, 0x0400);
    bgInitMapSet(0, &menubg2Map, (&menubg2Map_end - &menubg2Map), SC_32x32, 0x0800);

    setMode(BG_MODE1, 0);
    setScreenOn();

    while (pad0 != KEY_START)
    {
        WaitForVBlank();
        pad0 = padsCurrent(0);
        scrX1++;
        scrX2--;
        if (sens == 0 && scrY2 < 0 && timer == 0)
        {
            scrY2++;
        }
        if (sens == 1 && scrY2 > -20 && timer == 0)
        {
            scrY2--;
        }
        if (scrY2 >= 0)
        {
            sens = 1;
        }
        if (scrY2 <= -20)
        {
            sens = 0;
        }

        if (sens1 == 0 && scrY1 < 60 && timer == 0)
        {
            scrY1++;
        }
        if (sens1 == 1 && scrY1 > 40 && timer == 0)
        {
            scrY1--;
        }
        if (scrY1 >= 60)
        {
            sens1 = 1;
        }
        if (scrY1 <= 40)
        {
            sens1 = 0;
        }

        timer++;
        if (timer > 3)
        {
            timer = 0;
        }
        bgSetScroll(0, scrX1, scrY1);
        //bgSetScroll(1, scrY,0);
        bgSetScroll(2, scrX2, scrY2);
    }
    while (scrY1 < 72 || scrY2 > -20 || scrY3 < 50)
    {
        scrX1++;
        scrX2--;
        if (scrY1 < 72)
        {
            scrY1++;
        }
        if (scrY2 > -20)
        {
            scrY2--;
        }
        if (scrY3 < 50)
        {
            scrY3++;
        }
        WaitForVBlank();
        bgSetScroll(0, scrX1, scrY1);
        bgSetScroll(1, 0, scrY3);
        bgSetScroll(2, scrX2, scrY2);
    }
    //consoleDrawText(0,0,"player1");
    bgInitTileSet(1, &menubg4, &menubg4Palette, 1, (&menubg4_end - &menubg4), 16 * 2, BG_16COLORS, 0x2000);
    bgInitMapSet(1, &menubg4Map, (&menubg4Map_end - &menubg4Map), SC_32x32, 0x0400);
    setScreenOn();
    oamSet(1 * 4, 55, 120, 1, 0, 0, 8, 0);
    oamSetEx(1 * 4, OBJ_LARGE, OBJ_SHOW);
    oamSetVisible(1 * 4, OBJ_SHOW);

    oamSet(2 * 4, 170, 120, 0, 1, 0, 8, 0);
    oamSetEx(2 * 4, OBJ_LARGE, OBJ_SHOW);
    oamSetVisible(2 * 4, OBJ_SHOW);
    u8 pointerY = 120;
    u8 pointerX = 55;
    u8 pointerX2 = 170;
    bool changePointer = false;
    unsigned short lastPad = KEY_START;
    pad0 = 0;
    // Wait for nothing :P
    while (pad0 != KEY_START)
    {
        pad0 = padsCurrent(0);
        if (pad0 == KEY_UP && lastPad != KEY_UP)
        {
            if (pointerY == 120)
            {
                pointerY = 140;
                pointerX = 50;
                pointerX2 = 180;
            }
            else
            {
                pointerY = 120;
                pointerX = 55;
                pointerX2 = 170;
            }
            changePointer = true;
        }
        if (pad0 == KEY_DOWN && lastPad != KEY_DOWN)
        {
            if (pointerY == 140)
            {
                pointerY = 120;
                pointerX = 55;
                pointerX2 = 170;
            }
            else
            {
                pointerY = 140;
                pointerX = 50;
                pointerX2 = 180;
            }
            changePointer = true;
        }
        WaitForVBlank();
        scrX1++;
        scrX2--;
        fpsCounter++;
        bgSetScroll(0, scrX1, scrY1);
        bgSetScroll(1, 0, 0);
        bgSetScroll(2, scrX2, scrY2);
        lastPad = pad0;

        if (changePointer)
        {
            oamSet(1 * 4, pointerX, pointerY, 1, 0, 0, 8, 0);
            oamSet(2 * 4, pointerX2, pointerY, 1, 1, 0, 8, 0);
            changePointer = false;
        }
    }

    if (pointerY == 120)
    {
        twoPlayer = 0;
    }
    else
    {
        twoPlayer = 1;
    }
    oamSetVisible(1 * 4, OBJ_HIDE);
    oamSetVisible(2 * 4, OBJ_HIDE);
}

void resetVram()
{
    dmaClearVram();

    consoleInitText(2, 0, &snesfont);
    // Init Sprites gfx and palette with default size of 16x16
    oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end - &gfxpsrite), &palsprite, (&palsprite_end - &palsprite), 0, SPRITE_ADDRESS, OBJ_SIZE8);
    if (!twoPlayer)
    {
        bgInitTileSet(0, &gfxpsrite, &palsprite, 1, (&gfxpsrite_end - &gfxpsrite), (&palsprite_end - &palsprite), BG_16COLORS, BACKGROUND1_ADDRESS);
        bgInitTileSet(1, &t0, &p0, 2, (&t0_end - &t0), (&p0_end - &p0), BG_16COLORS, BACKGROUND2_ADDRESS);

        bgSetMapPtr(0, BACKGROUND1_MAP_ADDRESS, SC_32x32);
        bgInitMapSet(1, &m0, (&m0_end - &m0), SC_32x32, BACKGROUND2_MAP_ADDRESS);
    }
    else
    {
        bgInitTileSet(0, &gfxpsrite, &palsprite, 1, (&gfxpsrite_end - &gfxpsrite), (&palsprite_end - &palsprite), BG_16COLORS, BACKGROUND1_ADDRESS);
        bgInitTileSet(1, &t2, &p2, 2, (&t2_end - &t2), (&p2_end - &p2), BG_16COLORS, BACKGROUND2_ADDRESS);

        bgSetMapPtr(0, BACKGROUND1_MAP_ADDRESS, SC_32x32);
        bgInitMapSet(1, &m2, (&m2_end - &m2), SC_32x32, BACKGROUND2_MAP_ADDRESS);
    }
    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);

    u8 idSprite;
    for (idSprite = 8; idSprite < 12; ++idSprite)
    {
        oamSet(idSprite * 4, 0, 0, 0, 0, 0, 0, 0);
        oamSetEx(idSprite * 4, OBJ_SMALL, OBJ_SHOW);
        oamSetVisible(idSprite * 4, OBJ_SHOW);
    }
    for (idSprite = 16; idSprite < 20; ++idSprite)
    {
        oamSet(idSprite * 4, 0, 0, 0, 0, 0, 0, 0);
        oamSetEx(idSprite * 4, OBJ_SMALL, OBJ_SHOW);
        oamSetVisible(idSprite * 4, OBJ_SHOW);
    }
    setScreenOn();

    WaitForVBlank();
}

void initPiece(objet_t *next_piece_obj_ref, u8 x, u8 y)
{
    next_piece_obj_ref->x = x;
    next_piece_obj_ref->y = y;
    next_piece_obj_ref->rotation = DEG_0;
    int nb = fpsCounter & 0x07;
    nb = nb == 7 ? 0 : nb;
    next_piece_obj_ref->obj.piece = listPieces[nb];
}

void initPlateau(objet_t *plateau_obj_ref, u8 x, u8 y)
{

    plateau_obj_ref->x = x;
    plateau_obj_ref->y = y;
    plateau_obj_ref->rotation = DEG_0;

    u16 i;
    for (i = 0; i < TAILLE_PLATEAU * 2; ++i)
    {
        plateau_obj_ref->obj.plateau[i] = 0;
    }
    for (i = 0; i < HAUTEUR_PLATEAU; ++i)
    {
        u16 dest_address = BACKGROUND1_MAP_ADDRESS + (plateau_obj_ref->y + i) * LARGEUR_BG1 + plateau_obj_ref->x;
        WaitForVBlank();
        dmaCopyVram(&plateau_obj_ref->obj.plateau[i * LARGEUR_PLATEAU * 2], dest_address, LARGEUR_PLATEAU * 2);
    }
}

void resetGame()
{
    scoreClear(&score);
    gameOver = false;

    consoleDrawText(1, 2, "         ");
    consoleDrawText(2, 4, "       ");
    consoleDrawText(1, 5, "          ");
    player1.speed = INITIAL_SPEED;
    player1.score =0;
    player1.lastPieceRotation = DEG_0;
    player2.speed = INITIAL_SPEED;
    player2.lastPieceRotation = DEG_0;
    player2.score =0;
    if (!twoPlayer)
    {
        consoleDrawText(2, 22, "line: 0");
        initPlateau(&plateau_obj, PLATEAU_X, PLATEAU_Y);
        initPiece(&next_piece_obj, NEXT_PIECE_X, NEXT_PIECE_Y);
        resetPiece(&player1);
    }
    else
    {
        consoleDrawText(6, 1, "line : 0");
        consoleDrawText(18, 1, "line : 0");
        initPlateau(&plateau_obj, PLATEAU_X_P1, PLATEAU_Y_P1);
        initPiece(&next_piece_obj, NEXT_PIECE_X_P1, NEXT_PIECE_Y_P1);
        resetPiece(&player1);
        initPlateau(&plateau_obj2, PLATEAU_X_P2, PLATEAU_Y_P2);
        initPiece(&next_piece_obj2, NEXT_PIECE_X_P2, NEXT_PIECE_Y_P2);
        resetPiece(&player2);
    }
     spcPlay(0);

   
}

void gameStart()
{
    resetVram();
    resetGame();
  
    spcSetModuleVolume(100);

    if (!twoPlayer)
    {
        spcLoad(MOD_DIGIMON_RURY_DARK_TOWER);
        spcPlay(0);
        mode1player();

    }
    else
    {
        spcLoad(MOD_MMR_BATTLE);
        spcPlay(0);
        mode2player();
    }
}

void backToMenu()
{
    consoleInit();
    menu();
    gameStart();
}

bool lastGamePauseState = false;
void mode2player()
{
   
    unsigned short pad0;
    unsigned short pad1;
    bool quitting = false;
    while (!quitting)
    {
        pad0 = padsCurrent(0);
        pad1 = padsCurrent(1);

        spcProcess();

        if (gameOver == true)
        {
            spcStop();
            // Update music / sfx stream and wait vbl
            spcProcess();
            showPiece(false, &player1);
            showPiece(false, &player2);
            consoleDrawText(1, 2, "Game Over");
            consoleDrawText(2, 4, "Press A");
            consoleDrawText(1, 5, "to restart");

            consoleDrawText(2, 6, "Press Start");
            consoleDrawText(1, 7, "to back to menu");

            if (looser){
                consoleDrawText(8, 13, "winner player 1");
            }else{
                consoleDrawText(8, 13, "winner player 2");
            }
            if (pad0 & KEY_A)
            {
                // resetVram();
                resetGame();
            }
            if (pad0 & KEY_START)
            {
                quitting = true;
                // menu();
            }
        }
        else
        {
            if (pad0 & KEY_START)
            {
                if (lastGamePauseState == pauseGame)
                {
                    if (pauseGame == false)
                    {
                        pauseGame = true;
                        consoleDrawText(10, 6, "Game paused");
                        consoleDrawText(5, 7, "Press start to resume");

                        //consoleDrawText(5, 10, "Press select to back to menu");
                    }
                    else
                    {
                        pauseGame = false;
                        consoleDrawText(10, 6, "           ");
                        consoleDrawText(5, 7, "                     ");
                    }
                }
            }
            else
            {
                lastGamePauseState = pauseGame;
            }

            if (pauseGame == false)
            {
                movePiece(pad0, &player1);
                if (isRowDeleted)
                {
                    player1.score += isRowDeleted;
                    consoleDrawText(6, 1, "line : %d", (u32)player1.score);
                    player2.speed += isRowDeleted;
                    isRowDeleted = 0;
                }
                movePiece(pad1, &player2);
                 if (isRowDeleted)
                {
                    player2.score += isRowDeleted;
                    consoleDrawText(18, 1, "line: %d", (u32)player2.score);
                    player1.speed +=isRowDeleted;
                    isRowDeleted = 0;
                }
                setPieceInMemory(&player1);

                setPieceInMemory(&player2);
            }
        }

        // char messtxt[20] = "";
        // sprintf(messtxt,"FPS=%d\r\n", snes_vblank_count);
        // consoleNocashMessage(messtxt);
        ++fpsCounter;
        WaitForVBlank();
    }
    resetGame();
    resetVram();
    backToMenu();
}

void mode1player()
{
    spcPlay(MOD_DIGIMON_RURY_DARK_TOWER);
    // Update music / sfx stream and wait vbl
    spcProcess();
    unsigned short pad0;
    bool quitting = false;
    while (!quitting)
    {
        pad0 = padsCurrent(0);

        spcProcess();

        if (gameOver == true)
        {
            spcStop();
            // Update music / sfx stream and wait vbl
            spcProcess();
            showPiece(false, &player1);
            consoleDrawText(1, 2, "Game Over");
            consoleDrawText(2, 4, "Press A");
            consoleDrawText(1, 5, "to restart");
            consoleDrawText(2, 6, "Press Start");
            consoleDrawText(1, 7, "to back to menu");
            if (pad0 & KEY_A)
            {
                // resetVram();
                resetGame();
            }
            if (pad0 & KEY_START)
            {
                quitting = true;
                // menu();
            }
        }
        else
        {
            if (pad0 & KEY_START)
            {
                if (lastGamePauseState == pauseGame)
                {
                    if (pauseGame == false)
                    {
                        pauseGame = true;
                        consoleDrawText(10, 6, "Game paused");
                        consoleDrawText(5, 7, "Press start to resume");

                        //consoleDrawText(5, 10, "Press select to back to menu");
                    }
                    else
                    {
                        pauseGame = false;
                        consoleDrawText(10, 6, "           ");
                        consoleDrawText(5, 7, "                     ");
                    }
                }
            }
            else
            {
                lastGamePauseState = pauseGame;
            }

            if (pauseGame == false)
            {
                movePiece(pad0, &player1);
                if (isRowDeleted)
                {
                    player1.score += isRowDeleted;
                    consoleDrawText(2, 22, "line: %d", (u32)player1.score);
                    player1.speed +=isRowDeleted;
                    isRowDeleted = 0;
                }
                setPieceInMemory(&player1);
            }
        }

        // char messtxt[20] = "";
        // sprintf(messtxt,"FPS=%d\r\n", snes_vblank_count);
        // consoleNocashMessage(messtxt);
        ++fpsCounter;
        WaitForVBlank();
    }
    resetGame();
    resetVram();
    backToMenu();
}

int main(void)
{
    
    player1.id = 0;
    player1.speed = INITIAL_SPEED;
    player1.lastPieceRotation = DEG_0;
    player1.idSpriteNextPiece = 8;
    player1.idSpritePiece = 4;
    player1.next_piece = &next_piece_obj;
    player1.plateau = &plateau_obj;
    player1.piece = &piece_obj;

    player2.id = 1;
    player2.speed = 0;
    player2.lastPieceRotation = DEG_0;
    player2.idSpriteNextPiece = 16;
    player2.idSpritePiece = 12;
    player2.next_piece = &next_piece_obj2;
    player2.plateau = &plateau_obj2;
    player2.piece = &piece_obj2;

    u8 i, idSprite;
    spcBoot();
    consoleInit();

    // Set give soundbank
 
    spcSetBank(&__SOUNDBANK__1);
    spcSetBank(&__SOUNDBANK__0);

    // allocate around 10K of sound ram (39 256-byte blocks)
    spcAllocateSoundRegion(39);

    // Load music

    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);
    spcSetModuleVolume(50);

    // Update music / sfx stream and wait vbl
    spcProcess();
    spcSetSoundEntry(15, 1, 6, &placebrrend - &placebrr, &placebrr, &placesound);
    spcSetSoundEntry(15, 15, 6, &linebrrend - &linebrr, &linebrr, &linesound);
    //bootScreen();
    menu();
    gameStart();

    return 0;
}