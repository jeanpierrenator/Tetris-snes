#include <snes.h>
#include <stdlib.h>

#include "soundbank.h"
#include"gameplay.h"

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
extern char __SOUNDBANK__2;

extern char m0, m0_end, p0, p0_end, t0, t0_end;


unsigned int fpsCounter;

objet_t piece_obj;
objet_t plateau_obj;

objet_t next_piece_obj;

objet_t piece_obj2;
objet_t plateau_obj2;

objet_t next_piece_obj2;


unsigned int pieceMove = 0;
unsigned int pieceMove2 = 0;
ROTATION lastRotation = DEG_0;
int speed = INITIAL_SPEED;
int speed_boost = 0;

scoMemory score;
bool gameOver = false;
bool pauseGame = false;
unsigned int time_start, time_end;

piece_t listPieces[NOMBRE_PIECES] = {
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

void menu()
{
    spcLoad(0);
    spcPlay(0); 
    spcSetModuleVolume(100);
    spcProcess();
    short scrX1=0, scrY1=60;
	short scrX2=0, scrY2=0; 
	short scrY3=0;
	short sens =0, sens1 =0;
	short timer = 0;
	unsigned short pad0 =0;
    bgInitTileSet(2, &menubg, &menubgPalette, 0, (&menubg_end - &menubg), 16*2, BG_16COLORS, 0x3000);
	bgInitTileSet(1, &menubg3, &menubg3Palette, 1, (&menubg3_end - &menubg3), 16*2, BG_16COLORS, 0x2000);
	bgInitTileSet(0, &menubg2, &menubg2Palette, 2, (&menubg2_end - &menubg2), 16*2, BG_16COLORS, 0x1000);
		
	
	bgInitMapSet(2, &menubgMap, (&menubgMap_end - &menubgMap),SC_32x32, 0x0000);
	bgInitMapSet(1, &menubg3Map, (&menubg3Map_end - &menubg3Map),SC_32x32, 0x0400);
	bgInitMapSet(0, &menubg2Map, (&menubg2Map_end - &menubg2Map),SC_32x32, 0x0800);

    	setMode(BG_MODE1,0);  
	setScreenOn();

    while (pad0 != KEY_START)
	{
		WaitForVBlank();
		pad0 = padsCurrent(0);
		scrX1++;  scrX2--;
		if(sens == 0 && scrY2 <0 && timer == 0){
			scrY2++;
		}
		if(sens == 1 && scrY2 >-20  && timer == 0){
			scrY2--;
		}
		if(scrY2 >= 0){
			sens = 1;
		}
		if(scrY2 <= -20){
			sens = 0;
		}


		if(sens1 == 0 && scrY1 <60 && timer == 0){
			scrY1++;
		}
		if(sens1 == 1 && scrY1 >40  && timer == 0){
			scrY1--;
		}
		if(scrY1 >= 60){
			sens1 = 1;
		}
		if(scrY1 <= 40){
			sens1 = 0;
		}

		timer++;
		if(timer > 3){
			timer = 0;
		}
		bgSetScroll(0, scrX1,scrY1);
		//bgSetScroll(1, scrY,0);
		bgSetScroll(2, scrX2,scrY2);
    }
        while (scrY1 <72 || scrY2 > -20 || scrY3 <50)
	{
		scrX1++;  scrX2--;
		if(scrY1 <72){
		scrY1++;
		}
		if(scrY2  > -20){
		scrY2--;
		}
		if(scrY3 < 50){
			scrY3++;
		}
		WaitForVBlank();
		bgSetScroll(0, scrX1,scrY1);
		bgSetScroll(1, 0,scrY3);
		bgSetScroll(2, scrX2,scrY2);
	}
    //consoleDrawText(0,0,"player1");
	bgInitTileSet(1, &menubg4, &menubg4Palette, 1, (&menubg4_end - &menubg4), 16*2, BG_16COLORS, 0x2000);
	bgInitMapSet(1, &menubg4Map, (&menubg4Map_end - &menubg4Map),SC_32x32, 0x0400);
	setScreenOn();
	// Wait for nothing :P
	while(pad0 != KEY_A) {
        pad0 = padsCurrent(0);
		WaitForVBlank();
		scrX1++;  scrX2--;
        fpsCounter++;
		bgSetScroll(0, scrX1,scrY1);
		bgSetScroll(1, 0,0);
		bgSetScroll(2, scrX2,scrY2);
		
		}
	

}

void resetVram(){
    dmaClearVram();

  
   
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

void initPiece(objet_t*  next_piece_obj_ref)
{
    next_piece_obj_ref->x = 3;
    next_piece_obj_ref->y = 10;
    next_piece_obj_ref->rotation = DEG_0;
    int nb = fpsCounter & 0x07;
    nb = nb == 7 ? 0 : nb;
    next_piece_obj_ref->obj.piece = listPieces[nb];
}

void initPlateau(objet_t* plateau_obj_ref)
{
    
    plateau_obj_ref->x = PLATEAU_X;
    plateau_obj_ref->y = PLATEAU_Y;
    plateau_obj_ref->rotation = DEG_0;

    u16 i;
    for (i = 0; i < TAILLE_PLATEAU * 2; ++i)
    {
        plateau_obj_ref->obj.plateau[i] = 0;
    }
    for (i = 0 ; i < HAUTEUR_PLATEAU; ++i)
    {
        u16 dest_address = BACKGROUND1_MAP_ADDRESS + (plateau_obj_ref->y + i) * LARGEUR_BG1 + plateau_obj_ref->x;
        WaitForVBlank();
        dmaCopyVram(&plateau_obj_ref->obj.plateau[i * LARGEUR_PLATEAU * 2], dest_address, LARGEUR_PLATEAU * 2);
    }
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

    

    initPlateau(&plateau_obj);
    initPiece(&next_piece_obj);
    resetPiece(&piece_obj,&plateau_obj,&next_piece_obj);
    spcPlay(1);
	        // Update music / sfx stream and wait vbl
	spcProcess();
}

bool lastGamePauseState = false;

int main(void){
    unsigned short pad0;
    unsigned short pad1;
    u8 i, idSprite;
    spcBoot();
    consoleInit();

    	// Set give soundbank
	spcSetBank(&__SOUNDBANK__2);
	spcSetBank(&__SOUNDBANK__1);
	spcSetBank(&__SOUNDBANK__0);

    // allocate around 10K of sound ram (39 256-byte blocks)
	spcAllocateSoundRegion(39);

	// Load music
	
    setMode(BG_MODE1,0);  bgSetDisable(1);  bgSetDisable(2);
    spcSetModuleVolume(50);
    
	// Update music / sfx stream and wait vbl
	spcProcess();
    spcSetSoundEntry(15, 1, 6, &placebrrend-&placebrr, &placebrr, &placesound);
	spcSetSoundEntry(15, 15,  6, &linebrrend-&linebrr, &linebrr, &linesound);
	bootScreen();
    menu();
    resetVram();
    resetGame();
    spcLoad(1);
    spcPlay(0); 
    spcSetModuleVolume(100);
    while (1)
    {
        pad0 = padsCurrent(0);
        pad1 = padsCurrent(1);
        
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
                movePiece(pad0,&piece_obj,&plateau_obj,&next_piece_obj);

                idSprite = 0;
                setPieceInMemory(&piece_obj,0);
                
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