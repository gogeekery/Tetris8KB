
#include "Defines.h"


#define cShapeSize		4
#define cShapeNum		7


extern tU8 shapeNext;			// Next shape
extern tS8 shapeX, shapeY;		// Shape X/Y coords

extern tU8 shapeCurrent[cShapeSize][cShapeSize];	// Holds the current shape's data

const extern tU8 _RawShapeData[cShapeNum*cShapeSize];

#define shapeData(Shape, Pos)	_RawShapeData[(Shape*(cShapeSize))+Pos]


//
void shapeNew();

void shapeRotate();


void shapeMove(const tS8 aXAdd, const tS8 aYAdd);

// Returns true if the active shape will collide with a map tile at pos X/Y
bool shapeCheck(const tS8 XPos, const tS8 YPos);

// Places the shape on the map (selects next shape, updates map)
void shapePlace();

