
#include "Game.h"
#include "Shapes.h"

tU8 shapeNext;			// Next shape
tS8 shapeX, shapeY;		// Shape X/Y coords

tU8 shapeCurrent[cShapeSize][cShapeSize];	// Holds the current shape's data

const tU8 _RawShapeData[cShapeNum*cShapeSize] = {

	1+(0*cShapeSize), 1+(1*cShapeSize), 1+(2*cShapeSize), 1+(3*cShapeSize),	// Tower
	1+(1*cShapeSize), 1+(2*cShapeSize), 2+(1*cShapeSize), 2+(2*cShapeSize),	// Box
	1+(1*cShapeSize), 0+(2*cShapeSize), 1+(2*cShapeSize), 2+(2*cShapeSize),	// Pyramid
	0+(1*cShapeSize), 1+(1*cShapeSize), 1+(2*cShapeSize), 2+(2*cShapeSize),	// Left Corner
	2+(1*cShapeSize), 1+(1*cShapeSize), 1+(2*cShapeSize), 0+(2*cShapeSize),	// Right Corner
	1+(1*cShapeSize), 2+(1*cShapeSize), 2+(2*cShapeSize), 2+(3*cShapeSize),	// Left Knight
	2+(1*cShapeSize), 1+(1*cShapeSize), 1+(2*cShapeSize), 1+(3*cShapeSize)	// Right Knight

};


void shapeNew() {

	memset(shapeCurrent, 255, cShapeSize*cShapeSize);

	tU8 Pos;
	for (Pos = 0; Pos < cShapeSize; ++Pos)
		shapeCurrent[shapeData(shapeNext, Pos)%cShapeSize][(tU8)shapeData(shapeNext, Pos)/cShapeSize] = shapeNext;

	shapeX = ((cMapWidth/2)-(cShapeSize/2));
	shapeY = -(tS8)(shapeData(shapeNext, 0)/cShapeSize);

	shapeNext = rand()%cShapeNum;
	gameDrawHUD();									// Update HUD (Score, Next Shape)

}

// Returns true if current Shape collides with tiles at X/YPos
bool shapeCheck(const tS8 XPos, const tS8 YPos) {

	tU8 _XP, _YP;

	for (_XP = 0; _XP < cShapeSize; ++_XP)			// Go through the Shape...
	for (_YP = 0; _YP < cShapeSize; ++_YP)

		if (shapeCurrent[_XP][_YP] != cTileNone) {			// There is a Tile here...

			if ((XPos+_XP < 0 || XPos+_XP > cMapWidth-1		// The Shape is out of bounds!
			|| YPos+_YP < 0 || YPos+_YP > cMapHeight-1)
			|| MapData[XPos+_XP][YPos+_YP] != cTileNone)	// Or a Tile is already here...
				return TRUE;

		}

	return FALSE;

}



void shapeRotate() {

	tU8 _XP, _YP, TShape[cShapeSize][cShapeSize];

	for (_XP = 0; _XP < cShapeSize; ++_XP)
	for (_YP = 0; _YP < cShapeSize; ++_YP)
		TShape[cShapeSize-1-_YP][_XP] = shapeCurrent[_XP][_YP];

	for (_XP = 0; _XP < cShapeSize; ++_XP)			// Go through the Shape...
	for (_YP = 0; _YP < cShapeSize; ++_YP)

		if (TShape[_XP][_YP] != cTileNone) {					// There is a Tile here...

			if ((shapeX+_XP < 0 || shapeX+_XP > cMapWidth-1		// The Shape is out of bounds!
			|| shapeY+_YP < 0 || shapeY+_YP > cMapHeight-1)
			|| MapData[shapeX+_XP][shapeY+_YP] != cTileNone)	// Or a Tile is already here...
				return;

		}

	for (_XP = 0; _XP < cShapeSize; ++_XP)
	for (_YP = 0; _YP < cShapeSize; ++_YP)
		shapeCurrent[_XP][_YP] = TShape[_XP][_YP];


	gameUpdateMap();	// Update the Tilemap!

}


void shapePlace() {

	tU8 _XP, _YP;
	tU8 Rows = 0;

	for (_XP = 0; _XP < cShapeSize; ++_XP)		// Add the Tiles to the Map!
	for (_YP = 0; _YP < cShapeSize; ++_YP)
	if (shapeCurrent[_XP][_YP] != cTileNone)
		MapData[shapeX+_XP][shapeY+_YP] = shapeCurrent[_XP][_YP];

	for (_YP = shapeY; _YP < cMapHeight; ++_YP) {	// Check for completed rows!

		for (_XP = 0; _XP < cMapWidth; ++_XP)
			if (MapData[_XP][_YP] == cTileNone)
				break;

		if (_XP == cMapWidth) {		// Reached the end of the previous loop...
			gameRemoveRow(_YP);		// There WAS a full row... Destroy it!
			gameScore += (++Rows);	// The more rows, the higher the score!
		}

	}

	shapeNew();			// Generate a new shape for shapeNext, and set ShapeType to old shapeNext!
	gameUpdateMap();	// Update the TileMap!

}


void shapeMove(const tS8 aXAdd, const tS8 aYAdd) {

	if (!shapeCheck(shapeX+aXAdd, shapeY+aYAdd)) {

		shapeX += aXAdd;
		shapeY += aYAdd;

		gameUpdateMap();	// Update the TileMap!
		return;				// Nothing more to do here...

	}	// Anything after this assumes a collision where the Tiles are heading!


	if (aYAdd) {				// Going down!

		if (shapeY < 1) {		// The Shape exited the Map!
			gameEnd();
		} else {				// Good to add the Tile!
			shapePlace();
		}

	}

}

