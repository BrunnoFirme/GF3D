#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"
#include "powerUp.h"
#include "global.h"
#include "time.h"

Entity* powerUp_new(Vector3D position, float width, float depth)
{
	Entity* ent = NULL;
	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("Cant make PowerUp");
		return NULL;
	}

	ent->speed = 1;
	ent->think = powerUp_think;
	ent->position = position;
	ent->boudingBox = boundingBox(vector2d(position.x, position.y),width,depth);
	ent->_isItem = 1;

	return ent;
}

void powerUp_think(Entity* self)
{
	if (!Player || !self || self->_isEquipped == 1 || !PlayerHurtBox)
		return;
	self->boudingBox->position = vector2d(self->position.x - (self->boudingBox->width/2), self->position.y - (self->boudingBox->height / 2));

	self->rotation.x += deltaTime*self->speed;
	
	gfc_matrix_make_translation(
		self->modelMat,
		vector3d(self->position.x, self->position.y, self->position.z)
	);
	

	gfc_matrix_rotate(self->modelMat,self->modelMat,self->rotation.x,vector3d(0,0,1));
	
	if (IsColliding(Player->boudingBox, self->boudingBox))
	{
		if (!self->powerUpFunc)
			slog("NO POWER UP FUNCTION");
		else
			self->powerUpFunc();
		if (self->_isItem == 1)
		{
			if (EquippedItem != NULL)
				EquippedItem->_isEquipped = 0;
			EquippedItem = self;
			self->_isEquipped = 1;
		}
		else
		{
			gf3d_entity_free(self);
		}
	}
}

void powerUpSpeedAndJump()
{
	slog("Equiped Ring Of Speed!");
	Player->speed = 2;
	Player->jumpHeight = 2;
}

void powerDownSpeedAndJump()
{
	slog("Equiped Mundane Ring!");
	Player->speed = 1;
	Player->jumpHeight = 1;
}

void powerUpGold()
{
	Player->gold = Player->gold + 20;
	slog("Player has %d gold", Player->gold);
}













