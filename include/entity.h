#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_model.h"

#include "collision.h"
//Pointer arrow
//Memory dot
typedef struct Entity_S
{
	Uint8 _inuse;
	Uint8 _isPlayer;
	Uint8 _isBoss;
	Uint8 _isEquipped;
	Uint8 _isItem;
	Matrix4* modelMat;
	Model* model;
	Vector3D velocity;
	Vector3D position;
	Vector3D rotation;
	BoundingBox* boudingBox;
	int gold;
	int health;
	float speed;
	float jumpHeight;
	void (*think)(struct Entity_S* self);
	void (*powerUpFunc)(void);
	char* name;
}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity* gf3d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void gf3d_entity_free(Entity* self);


#endif