#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gfc_audio.h"

#include "entity.h"
#include "collision.h"
#include "time.h"
#include "global.h"
#include "powerUp.h"
#include "enemy.h"

void player_think(Entity* self)
{
    if (!self)return;
    self->position.z += self->velocity.z * deltaTime;
    if (self->position.z > 0) self->velocity.z += -96.04 *deltaTime;
    if (self->position.z < 0)
    {
        self->position.z = 0;
        self->velocity.z = 0;
    }
}

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    
    BoundingBox *Forward;
    BoundingBox *Right;
    BoundingBox *Back;
    BoundingBox *Left;
    BoundingBox* SusBB;

    BoundingBox* PlayerBoundingBox;

    float desiredRot = 0;

    Forward = boundingBox(vector2d(-65, 65), 130, 5);
    Back = boundingBox(vector2d(-65, -65), 130, 5);
    Right = boundingBox(vector2d(65, -65), 5, 130);
    Left = boundingBox(vector2d(-65, -65), 5, 130);


    PlayerBoundingBox = boundingBox(vector2d(0, 0), 1, 1);
    PlayerHurtBox = boundingBox(vector2d(-2.5f, -1.5f), 5, 10);
    SusBB = boundingBox(vector2d(-20, -30), 40, 50);

    //Forward = 0, Right = 1, Back =2, Left =3
    int currentDirection = 0;
    Sound *testSound;


    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init
    (
        "The Elder Scrolls V: Skryim",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
    gfc_audio_init
    (
        10,  //Max sounds
        10,  //channels
        10,  //channel groups
        10,  //max music
        1,   //mp3
        1    //ogg
    );
    gf3d_entity_manager_init(2048);
	slog_sync();

    gf3d_camera_set_scale(vector3d(1, 1, 1));

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    testSound = gfc_sound_load("sus.mp3", 10,0);

    slog_sync();


    /*
    Model* speedDownModel;
    speedDownModel = gf3d_model_load("SpeedPowerUp");
    Matrix4 speedDownModelMatrix;
    gfc_matrix_identity(speedDownModelMatrix);
    gfc_matrix_make_translation(
        speedDownModelMatrix,
        vector3d(0, -5, 0)
    );
    */
    Model* room;
    room = gf3d_model_load("RoomA");
    Matrix4 RoomModelMatrix;
    gfc_matrix_identity(RoomModelMatrix);
    gfc_matrix_make_translation(
            RoomModelMatrix,
            vector3d(0,0,-5)
    );
    
    Player = gf3d_entity_new();
    Player->position = vector3d(2, 40, 2);
    Player->rotation = vector3d(0, 0, 0);
    Player->think = player_think;
    Player->boudingBox = PlayerBoundingBox;
    Player->speed = 1;
    Player->jumpHeight = 1;

    Matrix4 PlayerHandModelMatrix;
    gfc_matrix_identity(PlayerHandModelMatrix);

    float  cameraSpeed = 20;
    float    cooldown = 0;
    

    Model *speedUpModel;
    speedUpModel = gf3d_model_load_M("su", "Colors");
    
    Matrix4 speedUpModelMatrix; gfc_matrix_identity(speedUpModelMatrix);

    Entity* speedUpPowerUp;
    speedUpPowerUp = powerUp_new(vector3d(0, -20, 1),10,10);
    speedUpPowerUp->model = speedUpModel;
    speedUpPowerUp->rotation = vector3d(0, 0, 0);
    speedUpPowerUp->powerUpFunc = powerUpSpeedAndJump;
    speedUpPowerUp->modelMat = speedUpModelMatrix;
    speedUpPowerUp->speed = 4;

    Model* speedDownModel;
    speedDownModel = gf3d_model_load_M("sd", "Colors");
    
    Matrix4 speedDownModelMatrix; gfc_matrix_identity(speedDownModelMatrix);
    
    Entity* speedDownPowerUp;
    speedDownPowerUp = powerUp_new(vector3d(40, -20, 1), 10, 10);
    speedDownPowerUp->model = speedDownModel;
    speedDownPowerUp->rotation = vector3d(0, 0, 0);
    speedDownPowerUp->powerUpFunc = powerDownSpeedAndJump;
    speedDownPowerUp->modelMat = speedDownModelMatrix;
    speedDownPowerUp->speed = 1;

    Model* goldPickUpModel;
    goldPickUpModel = gf3d_model_load_M("gold", "Colors");
    Matrix4 goldPickUpModelMatrix;
    gfc_matrix_identity(goldPickUpModelMatrix);

    Entity* goldPickUp;
    goldPickUp = powerUp_new(vector3d(-40, -20, 0), 10, 10);
    goldPickUp->model = goldPickUpModel;
    goldPickUp->rotation = vector3d(0, 0, 0);
    goldPickUp->powerUpFunc = powerUpGold;
    goldPickUp->modelMat = goldPickUpModelMatrix;
    goldPickUp->speed = 1;
    goldPickUp->_isItem = 0;
    

    Model* AxeModel = gf3d_model_load_M("Axe", "Colors");
    Model* ClubModel = gf3d_model_load_M("Club", "Colors");
    Model* Club2Model = gf3d_model_load_M("Club2", "Colors");
    Model* Club3Model = gf3d_model_load_M("Club3", "Colors");
    Model* Club4Model = gf3d_model_load_M("Club4", "Colors");

    Model* weaponModel = AxeModel;
    damage = 1;


    Model* TargetModel = gf3d_model_load_M("Target", "Colors");
    Model* Target2Model = gf3d_model_load_M("Target2", "Colors");
    Model* Target3Model = gf3d_model_load_M("Target3", "Colors");
    Model* Target4Model = gf3d_model_load_M("Target4", "Colors");
    Model* Target5Model = gf3d_model_load_M("Target5", "Colors");

    Matrix4 targetModelMatrix; gfc_matrix_identity(targetModelMatrix);
    Entity* target1 = enemy_new(
        vector3d(-60, 50, 0),
        5,
        5,
        1,
        "Target 1",
        targetModelMatrix,
        TargetModel
    );
    
    Matrix4 target2ModelMatrix; gfc_matrix_identity(target2ModelMatrix);
    Entity* target2 = enemy_new(
        vector3d(-40, 50, 0),
        5,
        5,
        3,
        "Target 2",
        target2ModelMatrix,
        Target2Model
    );

    Matrix4 target3ModelMatrix; gfc_matrix_identity(target3ModelMatrix);
    Entity* target3 = enemy_new(
        vector3d(-20, 50, 0),
        5,
        5,
        5,
        "Target 3",
        target3ModelMatrix,
        Target3Model
    );

    Matrix4 target4ModelMatrix; gfc_matrix_identity(target4ModelMatrix);
    Entity* target4 = enemy_new(
        vector3d(0, 50, 0),
        5,
        5,
        10,
        "Target 4",
        target4ModelMatrix,
        Target4Model
    );

    Matrix4 target5ModelMatrix; gfc_matrix_identity(target5ModelMatrix);
    Entity* target5 = enemy_new(
        vector3d(20, 50, 0),
        5,
        5,
        30,
        "Boss Target",
        target5ModelMatrix,
        Target5Model
    );
    target5->_isBoss = 1;

    desiredRot = Player->rotation.z;
    while(!done)
    {
        UpdateTime();
        gfc_matrix_make_translation(
            speedUpModelMatrix,
            vector3d(speedUpPowerUp->position.x, speedUpPowerUp->position.y, speedUpPowerUp->position.z)
        );
        gfc_matrix_make_translation(
            speedDownModelMatrix,
            vector3d(speedDownPowerUp->position.x, speedDownPowerUp->position.y, speedDownPowerUp->position.z)
        );

        gfc_matrix_make_translation(
            PlayerHandModelMatrix,
            vector3d(Player->position.x + 2, Player->position.y + 4, Player->position.z - 7)
        );

        //vector3d(Player->position.x + forward.x + 2, Player->position.y + forward.y + 4, Player->position.z + forward.z - 7)

        gfc_matrix_rotate(
            PlayerHandModelMatrix,
            PlayerHandModelMatrix,
            Player->rotation.y,
            vector3d(0, 0, 1)
        );

        //vector3d(Player->rotation.x, Player->rotation.x, Player->rotation.x),
        Player->think(Player);
        speedUpPowerUp->think(speedUpPowerUp);
        speedDownPowerUp->think(speedDownPowerUp);
        if (goldPickUp->_inuse)
            goldPickUp->think(goldPickUp);
        if (target1->_inuse)
            target1->think(target1);
        if (target2->_inuse)
            target2->think(target2);
        if (target3->_inuse)
            target3->think(target3);
        if (target4->_inuse)
            target4->think(target4);
        if (target5->_inuse)
            target5->think(target5);
        //slog("X: %f  Y: %f  Z: %f", Player->position.x, Player->position.y, Player->position.z);
        //slog("X: %f  Y: %f  Z: %f", susPos.x, susPos.y, susPos.z);
        //slog("X: %f  Y: %f  Z: %f", rotation.x/(GFC_DEGTORAD), rotation.y / (GFC_DEGTORAD), rotation.z / (GFC_DEGTORAD));

        if (cooldown > 0)
        {
            cooldown -= deltaTime;
            if (cooldown < 0)
                cooldown = 0;
        }
        
        if (Player->rotation.z - desiredRot > -.01f && Player->rotation.z - desiredRot < .01f)
        {

        }
        else
        {
            if (Player->rotation.z < desiredRot)
                Player->rotation.z += deltaTime*10;
            else
                Player->rotation.z -= deltaTime*10;
            if (Player->rotation.z - desiredRot > -.01f && Player->rotation.z - desiredRot < .01f)
                Player->rotation.z = desiredRot;
        }
        
        PlayerBoundingBox->position = vector2d(Player->position.x, Player->position.y);
        PlayerHurtBox->position = vector2d(Player->position.x-2.5f, Player->position.y);

        if (IsColliding(SusBB, PlayerBoundingBox) == 1 && keys[SDL_SCANCODE_SPACE] && speedUpPowerUp->position.z == 0)
        {
            speedUpPowerUp->velocity.z += 100;
            gfc_sound_play(testSound, 0, 1, -1, -1);
        }

        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        
        //gf3d_vgraphics_rotate_camera(0.001);

        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());
       
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
            //Swap this with for loop for array of models
            //Never mind make an entire fuckin entity system dumbass
                if (speedUpPowerUp->_isEquipped == 0)
                    gf3d_model_draw(speedUpPowerUp->model,bufferFrame,commandBuffer,speedUpModelMatrix);
                if (speedDownPowerUp->_isEquipped == 0)
                    gf3d_model_draw(speedDownPowerUp->model, bufferFrame, commandBuffer, speedDownModelMatrix);
                if (goldPickUp->_inuse)
                    gf3d_model_draw(goldPickUp->model, bufferFrame, commandBuffer, goldPickUpModelMatrix);

                if (target1->_inuse)
                    gf3d_model_draw(target1->model, bufferFrame, commandBuffer, targetModelMatrix);
                if (target2->_inuse)
                    gf3d_model_draw(target2->model, bufferFrame, commandBuffer, target2ModelMatrix);
                if (target3->_inuse)
                    gf3d_model_draw(target3->model, bufferFrame, commandBuffer, target3ModelMatrix);
                if (target4->_inuse)
                    gf3d_model_draw(target4->model, bufferFrame, commandBuffer, target4ModelMatrix);
                if (target5->_inuse)
                    gf3d_model_draw(target5->model, bufferFrame, commandBuffer, target5ModelMatrix);

                gf3d_model_draw(weaponModel, bufferFrame, commandBuffer, PlayerHandModelMatrix);
                gf3d_model_draw(room,bufferFrame,commandBuffer,RoomModelMatrix);
                //gf3d_model_draw(model2, bufferFrame, commandBuffer, modelMat3);
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

        if (keys[SDL_SCANCODE_Z])
        {
            gfc_sound_play(testSound,0,1,-1,-1);
        }
        
        float speed = 100;
        Vector3D oldPos = vector3d(Player->position.x, Player->position.y, Player->position.z);
        vector3d_set_magnitude(&forward, 0);

        if (currentDirection == 0)
        {
            //Forward
            forward = vector3d(0, 1, 0);
            vector3d_set_magnitude(&forward, deltaTime*speed);
        }
        else if (currentDirection == 1)
        {
            //Right
            forward = vector3d(-1, 0, 0);
            vector3d_set_magnitude(&forward, deltaTime* speed);
        }
        else if (currentDirection == 2)
        {
            //Backward
            forward = vector3d(0, -1, 0);
            vector3d_set_magnitude(&forward, deltaTime * speed);
        }
        else if (currentDirection == 3)
        {
            //Left
            forward = vector3d(1, 0, 0);
            vector3d_set_magnitude(&forward, deltaTime * speed);
        }
        right = vector3d(forward.y, -forward.x, 0);
        vector3d_set_magnitude(&right, deltaTime * speed * Player->speed);

        if (keys[SDL_SCANCODE_W])
            vector3d_add(Player->position, Player->position, forward);
        if (keys[SDL_SCANCODE_S])
            vector3d_add(Player->position, Player->position, -forward);
        if (keys[SDL_SCANCODE_D])
            vector3d_add(Player->position, Player->position, right);
        if (keys[SDL_SCANCODE_A])
            vector3d_add(Player->position, Player->position, -right);


        

        if (keys[SDL_SCANCODE_LSHIFT])Player->position.z -= 0.10;
        if (keys[SDL_SCANCODE_SPACE] && Player->position.z == 0)Player->velocity.z += (50 * Player->jumpHeight);

        Player->rotation.y = 180 * (GFC_DEGTORAD);


        if (isAttacking == 1)
            isAttacking = 0;

        if (cooldown == 0)
        {
            if (keys[SDL_SCANCODE_Q] && isAttacking == 0)
            {
                isAttacking = 1;
                cooldown = .1;
            }

            if (keys[SDL_SCANCODE_1])
            {
                weaponModel = AxeModel;
                damage = 1;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_2])
            {
                weaponModel = ClubModel;
                damage = 2;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_3])
            {
                weaponModel = Club2Model;
                damage = 3;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_4])
            {
                weaponModel = Club3Model;
                damage = 4;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_5])
            {
                weaponModel = Club4Model;
                damage = 5;
                slog("You have %d damage", damage);
                cooldown = .1;
            }

            int dir = 0;
            if (keys[SDL_SCANCODE_LEFT])
                dir += 1;
            if (keys[SDL_SCANCODE_RIGHT])
                dir -= 1;
            if (dir != 0)
            {
                cooldown = .1;
                int oldDir = currentDirection;
                currentDirection = changeDir(currentDirection,dir);
                if (currentDirection == 0)
                    desiredRot = 0;
                if (currentDirection == 1)
                {
                    if (oldDir == 2)
                        Player->rotation.z = 180 * (GFC_DEGTORAD);
                    desiredRot = 90 * (GFC_DEGTORAD);
                }
                if (currentDirection == 2)
                {
                    if (oldDir == 1)
                        Player->rotation.z = -270 * (GFC_DEGTORAD);
                    desiredRot = -180 * (GFC_DEGTORAD);
                }
                if (currentDirection == 3)
                    desiredRot = -90 * (GFC_DEGTORAD);
            }

            Player->rotation.x = 180 * (GFC_DEGTORAD);
        }

        PlayerBoundingBox->position = vector2d(Player->position.x, Player->position.y);

        if (IsColliding(Forward, PlayerBoundingBox) == 1 || IsColliding(Back, PlayerBoundingBox) == 1 || IsColliding(Right, PlayerBoundingBox) == 1 || IsColliding(Left, PlayerBoundingBox) == 1)
            Player->position = oldPos;

        vector3d_set_magnitude(&forward, 1);
        gf3d_camera_set_position(Player->position);
        gf3d_camera_set_rotation(Player->rotation);
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    gfc_sound_close();
    //cleanup
    slog("gf3d program end");
    slog_sync();
    gf3d_entity_manager_close();
    return 0;


}
/*eol@eof*/

int changeDir(int currentDirection, int isRight)
{
    int output = currentDirection;

    if (isRight == 1)
        output += 1;
    else
        output -= 1;

    if (output == 4)
        output = 0;
    if (output == -1)
        output = 3;

    return output;
}


