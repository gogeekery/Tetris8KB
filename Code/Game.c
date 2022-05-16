
#include "Game.h"
#include "Shapes.h"

tU8 MapData[cMapWidth][cMapHeight];		// Holds the Map's Data (Tiles)

tU32 gameTime;							// Total Frames passed since start! (20 = 1 Sec)
tU16 gameScore;
tU8 gameKey;


void gameNew() {

	// Ready the Map!
	memset(MapData, 255, cMapWidth*cMapHeight);

	shapeNext = rand()%cShapeNum;
	gameTime = gameScore = 0;
	shapeNew();
	gameUpdateMap();

}

void gameEnd() {

	// TODO: "GAME OVER"
	gameNew();

}



void gameRemoveRow(const tU8 aRow) {

	tU8 _XP, _YP;	// Could use memset!

	for (_YP = aRow; _YP > 0; --_YP)
	for (_XP = 0; _XP < cMapWidth; ++_XP)
		MapData[_XP][_YP] = MapData[_XP][_YP-1];

}


void gameKeyDown(const tU8 aKey) {
	gameKey = aKey;
}




// -----------------------------

void gameInit() {

	srand(timeGetTime());

	hMapDC	= CreateCompatibleDC(hWinDC);
	hTileDC	= CreateCompatibleDC(hWinDC);

	hMapBM	= CreateCompatibleBitmap(hWinDC, cTileSize*cMapWidth, cTileSize*cMapHeight);
	hTileBM	= CreateCompatibleBitmap(hWinDC, cTileSize*cShapeNum, cTileSize);

	SelectObject(hMapDC, hMapBM);
	SelectObject(hTileDC, hTileBM);


	HBRUSH Brushs[cShapeNum];

	Brushs[0] = CreateSolidBrush(RGB(255, 0, 0));
	Brushs[1] = CreateSolidBrush(RGB(0, 0, 255));
	Brushs[2] = CreateSolidBrush(RGB(224, 224, 224));
	Brushs[3] = CreateSolidBrush(RGB(255, 255, 0));
	Brushs[4] = CreateSolidBrush(RGB(0, 255, 0));
	Brushs[5] = CreateSolidBrush(RGB(255, 255, 255));
	Brushs[6] = CreateSolidBrush(RGB(255, 0, 255));


	tU8 Pos, XP;
	for (Pos = 0; Pos < cShapeNum; ++Pos) {

		const RECT lRect = {Pos*cTileSize, 0, (Pos*cTileSize)+cTileSize, cTileSize};
		FillRect(hTileDC, &lRect, Brushs[Pos]);	// Body (Color)

		// Border W/B
		for (XP = 0; XP < cTileSize; ++XP) {

			SetPixel(hTileDC, XP+(Pos*cTileSize), cTileSize-1, RGB(16, 16, 16));		// Bottom
			SetPixel(hTileDC, XP+(Pos*cTileSize),			0, RGB(208, 208, 208));		// Top

			SetPixel(hTileDC, cTileSize+(Pos*cTileSize)-1,	XP, RGB(16, 16, 16));		// Right
			SetPixel(hTileDC, Pos*cTileSize,				XP, RGB(208, 208, 208));	// Left

		}

	}

	for (Pos = 0; Pos < cShapeNum; ++Pos)
		DeleteObject(Brushs[Pos]);

}


void gameLoop() {

	int Speed = 20-(gameScore/2);
	if (Speed <= 4) Speed = 4;

	++gameTime;

	switch (gameKey) {

		case VK_LEFT:
			shapeMove(-1, 0);
		break;

		case VK_RIGHT:
			shapeMove(1, 0);
		break;

		case VK_UP:
			shapeRotate();
		break;

		case VK_DOWN:
			shapeMove(0, 1);
		break;

	}

	gameKey = 0;

	if (!(gameTime % Speed))
		shapeMove(0, 1);

}



// -------------------------------

void gameUpdateMap() {

	tU8 _XP, _YP;
	RECT Rect;

	// Cool background gradient
	for (_YP = 0; _YP < cMapHeight; ++_YP) {

		HBRUSH _Brush = CreateSolidBrush(RGB(192-((96/cMapHeight)*_YP), 192-((96/cMapHeight)*_YP), 192-((96/cMapHeight)*_YP)));
		const RECT lRect = {0, _YP*cTileSize, cMapWidth*cTileSize, (_YP*cTileSize)+cTileSize};

		FillRect(hMapDC, &lRect, _Brush);
		DeleteObject(_Brush);

	}

	// Draw solid (placed) tiles
	for (_XP = 0; _XP < cMapWidth; ++_XP)
	for (_YP = 0; _YP < cMapHeight; ++_YP)
	if (MapData[_XP][_YP] != 255)
		BitBlt(
			hMapDC,
			_XP*cTileSize, _YP*cTileSize, cTileSize, cTileSize,
			hTileDC, MapData[_XP][_YP]*cTileSize, 0, SRCCOPY
		);

	// Draw active (moving) shape
	for (_XP = 0; _XP < cShapeSize; ++_XP)
	for (_YP = 0; _YP < cShapeSize; ++_YP)
	if (shapeCurrent[_XP][_YP] != 255)
		BitBlt(
			hMapDC,
			(_XP+shapeX)*cTileSize, (_YP+shapeY)*cTileSize,
			cTileSize, cTileSize,
			hTileDC, shapeCurrent[_XP][_YP]*cTileSize, 0, SRCAND
		);


	// Border:
	POINT _Points[] = {{0, 0}, {0, (cMapHeight*cTileSize)-1}};
	Polyline(hMapDC, _Points, 2);	/// Left

	_Points[1].x = (cMapWidth*cTileSize)-1;
	_Points[1].y = 0;
	Polyline(hMapDC, _Points, 2);	/// Top

	_Points[0].y = _Points[1].y = (cMapHeight*cTileSize)-1;
	Polyline(hMapDC, _Points, 2);	/// Bottom

	_Points[0].x = (cMapWidth*cTileSize)-1;
	_Points[0].y = 0;
	++_Points[1].y;
	Polyline(hMapDC, _Points, 2);	/// Right
	// -------

	// Draw everything onto the window!
	{	// Don't wipe the whole window, just repaint the game area...
		const RECT lRect = {0, 0, cTileSize*cMapWidth, cTileSize*cMapHeight};
		BitBlt(hWinDC, 2, 1, cTileSize*cMapWidth, cTileSize*cMapHeight, hMapDC, 0, 0, SRCCOPY);
		ValidateRect(gWnd, &lRect);
	}

}

void gameDrawHUD() {			// Updates the Shape preview (next piece), Score, ETC

	tU8 Pos;

	// Background:
	const RECT Rect = {
		(cMapWidth+1)*cTileSize,
		cTileSize,
		(cMapWidth+3+cShapeSize)*cTileSize,
		(cShapeSize+3)*cTileSize
	};

	FillRect(hWinDC, &Rect, GetStockObject(BLACK_BRUSH));
	// ------

	// Shape:
	for (Pos = 0; Pos < cShapeSize; ++Pos)
		BitBlt(
			hWinDC,
			((shapeData(shapeNext, Pos)%cShapeSize)+cMapWidth+2)*cTileSize,
			(((tU8)shapeData(shapeNext, Pos)/cShapeSize)+2)*cTileSize,
			cTileSize, cTileSize,
			hTileDC, shapeNext*cTileSize, 0, SRCCOPY
		);
	// ------


	// Score:
	{
		char lScore[10];
		sprintf(lScore, "Score: %hhu", gameScore);

		Rectangle(															// Border
			hWinDC,
			(cMapWidth+1)*cTileSize-1, (cShapeSize+4)*cTileSize-1,
			(cMapWidth+7)*cTileSize+1, (cShapeSize+5)*cTileSize+1
		);
																			// Text
		TextOut(hWinDC, (cMapWidth+1)*cTileSize, (cShapeSize+4)*cTileSize, lScore, 8+(gameScore>9)+(gameScore>99));
	}
	// ------

}
