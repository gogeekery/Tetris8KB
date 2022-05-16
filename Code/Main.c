
#include "Game.h"
#include "Music.h"


HWND gWnd;	// A handle to the main Window!

HDC
	hWinDC,		// Handle to the Window's Device Contex
	hTileDC,	// Tile Bitmap Device Contex...
	hMapDC;		// TileMap Bitmap DC!


HBITMAP
	hTileBM,	// Bitmap Holds all the Tile images
	hMapBM;		// Suface for holding Map image data (Faster blitting)




// Windows Event Procedure Handler
LRESULT CALLBACK WndProc(HWND aWnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam) {

	switch (aMsg) {

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			gameKeyDown(aWParam);
		break;


		case WM_PAINT:
		{
			PAINTSTRUCT lPS;
			HDC lPHDC = BeginPaint(aWnd, &lPS);
			FillRect(lPHDC, &(lPS.rcPaint), (HBRUSH) (COLOR_WINDOW+1));
			BitBlt(hWinDC, 2, 1, cTileSize*cMapWidth, cTileSize*cMapHeight, hMapDC, 0, 0, SRCCOPY);
			gameDrawHUD();
			EndPaint(aWnd, &lPS);
		}
		break;


		default:
			return DefWindowProc(aWnd, aMsg, aWParam, aLParam);

	}

	return 0;

}



// ------------------------------------------
// ------ WIN MAIN -----------------------
// ------------------------------------

int WINAPI WinMain(HINSTANCE aHInst, HINSTANCE aHPrevInst, LPSTR aCmdLine, int aCmdShow) {

	WNDCLASSEX	lClass;
	MSG			lMsg;


	// The Window class structure
	lClass.cbSize			= sizeof(WNDCLASSEX);
	lClass.style			= CS_HREDRAW | CS_VREDRAW;
	lClass.lpfnWndProc		= WndProc;
	lClass.cbClsExtra		= 0;
	lClass.cbWndExtra		= 0;
	lClass.hInstance		= aHInst;
	lClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	lClass.hCursor			= NULL;//LoadCursor(NULL, IDC_ARROW);
	lClass.hbrBackground	= NULL;
	lClass.lpszMenuName		= NULL;
	lClass.lpszClassName	= WINCLASS;
	lClass.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	// Register the class
	RegisterClassEx(&lClass);

	// Create the main Window:
	gWnd = CreateWindowEx(
		0, WINCLASS, WINTITLE,
		WS_BORDER | WS_SYSMENU | WS_CAPTION,
		32, 32, (cMapWidth+(8))*cTileSize, (cMapHeight)*cTileSize+32,
		NULL, NULL/*Menu*/, aHInst, NULL
	);


	ShowWindow(gWnd, SW_SHOW);      // Display window
	UpdateWindow(gWnd);             // Update window
	hWinDC = GetDC(gWnd);			// Get the Device Context

	gameInit();
	musicInit();

	// TODO: Add File->New Game menu?
	gameNew();

	for (;;) {

		const DWORD StartTime = GetTickCount();

		gameLoop();		// Perform Game Logic!

		while ((GetTickCount()-StartTime) < 50) {		// Wait 50ms (20 FPS)

			while (PeekMessage(&lMsg, NULL, 0, 0, PM_REMOVE)) {

				if (lMsg.message == WM_QUIT)
					return lMsg.wParam;					// Quitting!

				TranslateMessage(&lMsg);
				DispatchMessage(&lMsg);

			}

			musicStep();								// Not multi-threaded (saves memory)!
			Sleep(1);									// Sleep, freeing CPU!

		}

	}

}
