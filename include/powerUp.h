#ifndef __POWERUP_H__
#define __POWERUP_H__

#include <SDL.h> 

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "entity.h"
#include "collision.h"

Entity* powerUp_new(Vector3D position, float width, float depth);

void powerUp_think(Entity* self);
void powerUpSpeedAndJump();
void powerDownSpeedAndJump();
void powerUpGold();

#endif