#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"
#include "enemy.h"
#include "global.h"
#include "time.h"

Entity* enemy_new(Vector3D position, float width, float depth, float health, char* name, Matrix4 modelMatrix, Model* _model)
{
	Entity* ent = NULL;
	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("Cant make Enemy");
		return NULL;
	}

	ent->speed = 1;
	ent->think = enemy_think;
	ent->position = position;
	ent->boudingBox = boundingBox(vector2d(position.x, position.y), width, depth);
	ent->health = health;
	ent->name = name;
	ent->model = _model;
	ent->modelMat = modelMatrix;
	return ent;
}

void enemy_think(Entity* self)
{
	if (!Player || !self || !PlayerHurtBox)
		return;
	self->boudingBox->position = vector2d(self->position.x - (self->boudingBox->width / 2), self->position.y - (self->boudingBox->height / 2));

	gfc_matrix_make_translation(
		self->modelMat,
		vector3d(self->position.x, self->position.y, self->position.z-5)
	);

	if (isAttacking == 1 && IsColliding(PlayerHurtBox, self->boudingBox))
	{
		if (self->_isBoss == 1 && damage < 5)
		{
			printf("You dont have enough damage to hurt ");
			printf(self->name);
			printf("! \n");
			return;
		}
		self->health -= damage;
		if (self->name)
		{
			printf("You delt %d damage to ", damage);
			printf(self->name);
			printf("! \n");
		}

		if (self->health <= 0)
			gf3d_entity_free(self);
	}
}
