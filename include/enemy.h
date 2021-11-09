#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "entity.h"

Entity* enemy_new(Vector3D position, float width, float depth, float health, char* name, Matrix4 modelMatrix, Model* _model);
void enemy_think(Entity* self);


#endif