#ifndef __COLLISION_H_
#define __COLLISION_H_

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

typedef struct
{
	Vector2D position;
	float width;
	float height;
}BoundingBox;

int IsColliding();

BoundingBox* boundingBox();

void boundingBoxFree(BoundingBox *boundingBox);

Vector2D GetPoint(int isTop, int isLeft, BoundingBox boundingBox);

#endif
