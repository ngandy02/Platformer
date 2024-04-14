#include "Entity.hpp"
#include <iostream>
using namespace std;

Entity::Entity()
{
    mPosition = glm::vec3(0);
    mMovement = glm::vec3(0);
    mSpeed = 0;
    mVelocity = glm::vec3(0);

    mAcceleration = glm::vec3(0);
    
    mModelMatrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
    delete [] animationLeft;
    delete [] animationRight;
    delete [] animationUp;
    delete [] animationDown;
    delete [] trainerWalkingAnimations;
}

//------GETTERS-------//

const glm::vec3 Entity::getPosition() const {return mPosition;};
const glm::vec3 Entity::getMovement() const {return mMovement;};

//------SETTERS------//
void Entity::setPosition(glm::vec3 newPosition) {mPosition = newPosition;};
void Entity::setMovement(glm::vec3 newMovement) {mMovement = newMovement;};

void Entity::pitAvoidance(Map *map) {
    glm::vec3 leftBottom = glm::vec3(mPosition.x - (width / 2), mPosition.y - (2*(height / 2)), mPosition.z);
    glm::vec3 rightBottom = glm::vec3(mPosition.x + (width / 2), mPosition.y - (2*(height / 2)), mPosition.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if(!(map->isSolid(leftBottom, &penetration_x, &penetration_y) && mVelocity.x < 0)){
        mMovement.x = 1;
    }
    if(!(map->isSolid(rightBottom, &penetration_x, &penetration_y) && mVelocity.x < 0)){
        mMovement.x = -1;
    }

}

void Entity::aiActivate(Entity *poketrainer, Map *map){
    switch(aiType) {
        case WALKER:
            aiWalk(map);
            break;
        case GUARD:
            aiGuard(poketrainer);
            break;
        case JUMPER:
            aiJump();
            break;
    }
}

void Entity::aiJump() {
    if (jump) {
        jump = false;
        mVelocity.y += jumpPower;
    }
}

void Entity::aiWalk(Map *map) {
    if (mMovement == glm::vec3(0)) {
        mMovement = glm::vec3(-1, 0, 0);
    }
    pitAvoidance(map);
}

void Entity::aiGuard(Entity *poketrainer){
    switch(aiState) {
        case IDLE:
            if (glm::distance(mPosition, poketrainer->mPosition) < 3.75f) {
                aiState = WALKING;
            }
            break;
            
        case WALKING:
            if (poketrainer->mPosition.x < mPosition.x) {
                mMovement = glm::vec3(-1, 0, 0);
            }
            else {
                mMovement = glm::vec3(1, 0, 0);
            }
            break;
            
        case JUMPING:
            break;
            
        case ATTACKING:
            break;
    }
}
bool Entity::collisionCheck(Entity *entity){
    if (isAlive == false || entity->isAlive == false) return false;
    float xdist = fabs(mPosition.x - entity->mPosition.x) - ((width + entity->width) / 2.0f);
    float ydist = fabs(mPosition.y - entity->mPosition.y) - ((height + entity->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::collisionYCheck(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (collisionCheck(object))
       {
           float ydist = fabs(mPosition.y - object->mPosition.y);
           float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
           if (mVelocity.y > 0) {
               mPosition.y -= penetrationY;
               mVelocity.y = 0;
               hitsTop = true;
               object->hitsBottom = true;
           }
           else if (mVelocity.y < 0) {
               mPosition.y += penetrationY;
               mVelocity.y = 0;
               hitsBottom= true;
               object->hitsTop = true;
} }
} }

void Entity::collisionXCheck(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (collisionCheck(object))
       {
           float xdist = fabs(mPosition.x - object->mPosition.x);
           float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
           if (mVelocity.x > 0) {
               mPosition.x -= penetrationX;
               mVelocity.x = 0;
               hitsRight = true;
               object->hitsLeft = true;
           }
           else if (mVelocity.x < 0) {
               mPosition.x += penetrationX;
               mVelocity.x = 0;
               hitsLeft = true;
               object->hitsRight = true;
} }
} }

void Entity::checkEnemyCollision(Entity *enemies, int enemyCount) { //players collision with the enemies
    hitsLeft = false;
    hitsRight = false;
    hitsTop = false;
  
    collisionYCheck(enemies, enemyCount); // check for hitsTop and hitsBottom for the player
    if (hitsTop){ // if enemies collides with the top of the player then the player dies
        isAlive = false;
    }
    if (hitsBottom){ // if enemies collides with the bottom of the player then enemies die
        for (int i = 0; i < enemyCount; ++i) {
            if (enemies[i].hitsTop == true) {//if enemies touch the player from the top
                enemies[i].isAlive = false; // enemies die
            }
        }
    }
    
    collisionXCheck(enemies, enemyCount); // check for hitsLeft and hitsRight for the player
    if ((hitsRight) || (hitsLeft)){ // if enemies collides with the left or right of the player then player dies
        isAlive = false;
    }
}

void Entity::mapCollisionYCheck(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(mPosition.x, mPosition.y + (height / 2), mPosition.z);
    glm::vec3 top_left = glm::vec3(mPosition.x - (width / 2), mPosition.y + (height / 2), mPosition.z);
    glm::vec3 top_right = glm::vec3(mPosition.x + (width / 2), mPosition.y + (height / 2), mPosition.z);
    glm::vec3 bottom = glm::vec3(mPosition.x, mPosition.y - (height / 2), mPosition.z);
    glm::vec3 bottom_left = glm::vec3(mPosition.x - (width / 2), mPosition.y - (height / 2), mPosition.z);
    glm::vec3 bottom_right = glm::vec3(mPosition.x + (width / 2), mPosition.y - (height / 2), mPosition.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->isSolid(top, &penetration_x,&penetration_y) && mVelocity.y > 0) {
        mPosition.y -= penetration_y;
        mVelocity.y = 0;
        
        hitsTop = true;
    }
    if (map->isSolid(bottom, &penetration_x,&penetration_y) && mVelocity.y < 0) {
        mPosition.y += penetration_y;
        mVelocity.y = 0;
        
        hitsBottom = true;
    }
    else if (map->isSolid(top_left,&penetration_x,&penetration_y) && mVelocity.y > 0) {
        mPosition.y -= penetration_y;
        mVelocity.y = 0;
        
        hitsTop = true;
    }
    else if (map->isSolid(bottom_left, &penetration_x, &penetration_y) && mVelocity.y < 0) {
        mPosition.y += penetration_y;
        mVelocity.y = 0;
        
        hitsBottom = true;
    }
    else if (map->isSolid(top_right, &penetration_x, &penetration_y) && mVelocity.y > 0) {
        mPosition.y -= penetration_y;
        mVelocity.y = 0;
        
        hitsTop = true;
    }
   
    else if (map->isSolid(bottom_right, &penetration_x, &penetration_y) && mVelocity.y < 0) {
        mPosition.y += penetration_y;
        mVelocity.y = 0;
        
        hitsBottom = true;
    }
}

void Entity::mapCollisionXCheck(Map *map)
{
    glm::vec3 left = glm::vec3(mPosition.x - (width / 2), mPosition.y, mPosition.z);
    glm::vec3 right = glm::vec3(mPosition.x + (width / 2), mPosition.y, mPosition.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->isSolid(left, &penetration_x,&penetration_y) && mVelocity.x < 0) {
        mPosition.x += penetration_x;
        mVelocity.x = 0;
        
        hitsLeft = true;
    }
    if (map->isSolid(right, &penetration_x, &penetration_y) && mVelocity.x > 0) {
        mPosition.x -= penetration_x;
        mVelocity.x = 0;
        
        hitsRight = true;
    }
}


void Entity::update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map)
{
    if (isAlive == false) {
        return;
    }
    hitsTop = false;
    hitsBottom = false;
    hitsLeft = false;
    hitsRight = false;
    
    pitLeft = false;
    pitRight = false;
    
    if (entityType == ENEMY) {
        aiActivate(player, map);
    }
    
    if (indices != NULL) {
        if (glm::length(mMovement) != 0) {
            animationTime += deltaTime;

            if (animationTime >= 0.25f)
            {
                animationTime = 0.0f;
                index++;
                if (index >= frames)
                {
                    index = 0;
                }
            }
        } else {
            index = 0;
        }
    }
    
    if (jump) {
        jump = false;
        mVelocity.y += jumpPower;
    }
    
    mVelocity.x = mMovement.x * mSpeed;
    mVelocity += mAcceleration * deltaTime;
    
    mPosition.y += mVelocity.y * deltaTime;
    mapCollisionYCheck(map);
    
    mPosition.x += mVelocity.x * deltaTime;
    mapCollisionXCheck(map);
    
    
    if(entityType == PLAYER) {
       
        checkEnemyCollision(objects, objectCount);
    }
    
    mModelMatrix = glm::mat4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, mPosition);
}

void Entity::drawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % cols) / (float)cols;
    float v = (float)(index / cols) / (float)rows;
    
    float width = 1.0f / (float)cols;
    float height = 1.0f / (float)rows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::render(ShaderProgram *program) {
    if (isAlive == false) {
        return;
    }
    
    program->set_model_matrix(mModelMatrix);
    
    if (indices != NULL) {
        drawSpriteFromTextureAtlas(program, textureID, indices[index]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
