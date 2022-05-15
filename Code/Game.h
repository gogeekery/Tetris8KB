
#include "Defines.h"

extern tU8 MapData[cMapWidth][cMapHeight];		// Holds the Map's Data (Tiles)
extern tU16 gameScore;


void gameUpdateMap();					// Draws all the tiles onto the map buffer.
void gameDrawHUD();						// Draws the next shape, as well as your score.

void gameNew();							// Wipes the map and starts anew.
void gameRemoveRow(const tU8 aRow);		// Removes the bottom row of tiles.

void gameInit();						// Called once at the start of the program. Mostly to allocate resources.
void gameLoop();						// Run every tick of the game. Executes functions and handles logic.

void gameNew();							// Starts a new game!

