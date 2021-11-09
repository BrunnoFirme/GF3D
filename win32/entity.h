#pragma once
#include <assert.h>

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_text.h"

#include "gf3d_texture.h"
#include "gf3d_mesh.h"
#include "gf3d_model.h"

typedef struct Entity_S
{
	Uint8 _inuse;
	Matrix4 modelMat;
	Model* model;
	void (*think)(struct Entity_S* self);
}Entity;

void entity_system_init(Uint32 maxEntities);
