/*

	8KB Tetris in C			05/20/2013 - gogeekery.com

		1) Very small file size		(Optimized to 8KB)
		2) Negligible RAM usage		(Less than 2MB)
		3) Minimal CPU usage

*/


#include <Windows.h>
#include <MMSystem.h>	// Windows Multi Media system
#include <WinGDI.h>		// Windows Graphical Device Interface

#include <stdio.h>
#include <stdlib.h>



/// Windows Stuff ----------------------------------

#define WINCLASS	"TETRIS"
#define WINTITLE	"Falling Block Game"



/// Types ------------------------------------------

typedef unsigned char	tU8;	// Positive only
typedef unsigned short	tU16;
typedef unsigned long	tU32;

typedef signed char		tS8;	// Negative only
typedef signed short	tS16;
typedef signed long		tS32;



// C does NOT define bool

#ifndef bool
	#define bool	char
#endif



// Map constants

#define cTileSize	16

#define cMapWidth	10
#define cMapHeight	20

#define cTileNone	255



/// Global variables

extern HWND gWnd;	// A handle to the Main Window!


extern HDC
	hWinDC,		// Handle to the Window's Device Contex
	hTileDC,	// Tile Bitmap Device Contex...
	hMapDC;		// TileMap Bitmap DC!


extern HBITMAP
	hTileBM,	// Bitmap Holds all the Tile images
	hMapBM;		// Suface for holding Map image data (Faster blitting)


