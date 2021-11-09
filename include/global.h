#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "entity.h"

Entity* Player;
Vector3D forward;
Vector3D right;
Entity* EquippedItem;
int damage;
Uint8 isAttacking;
BoundingBox* PlayerHurtBox;
#endif