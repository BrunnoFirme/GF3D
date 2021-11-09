#ifndef __COLLISION_H__
#define __COLLISION_H__
#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

typedef struct
{
	Vector2D position;
	float width;
	float height;
}BoundingBox;

Uint8 IsColliding(BoundingBox* first, BoundingBox* second);

BoundingBox *boundingBox(Vector2D position, float width, float depth);

void boundingBoxFree(BoundingBox *boundingBox);

Vector2D GetPoint(int isTop, int isRight, BoundingBox* boundingBox);

#endif
