#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.hpp"

enum EntityType {PLAYER, OBJECT, ENEMY};

enum AIType {WALKER, GUARD, JUMPER};
enum AIState {WALKING, IDLE, ATTACKING, JUMPING};

class Entity {
private:
    int* animationRight = NULL;
    int* animationLeft = NULL;
    int* animationUp = NULL;
    int* animationDown = NULL;
    
    glm::vec3 mPosition;
    glm::vec3 mMovement;
    
    
    
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    glm::vec3 mAcceleration;
    glm::vec3 mVelocity;
    
//-------TRANSFORMATIONS-------//
    float mSpeed;
    glm::mat4 mModelMatrix;
    
    
    float width = 1.0f;
    float height = 1.0f;
    
    bool jump = false; // jump in initially set to false
    float jumpPower = 0; // no jump power initially -> will be added in update of main where it call entities' update method which then calls entities' collisionYcheck method on the jumper enemy changing jump to true and calling the entities' aiactivate method to add the jump power.
    
    
    GLuint textureID;
    
// ————— STATIC VARIABLES ————— //
    static const int FRAMES_PER_SECOND = 5;
    static const int LEFT  = 0,
                        RIGHT = 1,
                        UP    = 2,
                        DOWN  = 3;
//------ANIMATION-------//
    int **trainerWalkingAnimations = new int* [4]{
        animationLeft,
        animationRight,
        animationUp,
        animationDown
    };

    int frames = 0;
    int index = 0;
    int cols = 0;
    int rows = 0;
    
    int* indices = NULL;
    float animationTime = 0.0f;

    
    bool hitsTop = false;
    bool hitsBottom = false;
    bool hitsLeft = false;
    bool hitsRight = false;
    
    bool pitRight = false;
    bool pitLeft = false;
    
    int lives;//
    bool isAlive = true;

    Entity();
    ~Entity();
    
    void drawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
//------GETTERS-------//
    const glm::vec3 getPosition() const ;
    const glm::vec3 getMovement() const;
    
//------SETTERS------//
    void setPosition(glm::vec3 newPosition);
    void setMovement(glm::vec3 newMovement);
    
    bool collisionCheck(Entity* entity);
    void collisionXCheck(Entity *collidableobjs, int objCount);
    void collisionYCheck(Entity *collidableobjs, int objCount);
    void checkEnemyCollision(Entity *wildPokemons, int pokemonCount);
    
    void mapCollisionXCheck(Map *map);
    void mapCollisionYCheck(Map *map);
    

    void aiActivate(Entity *poketrainer, Map *map);
    void aiWalk(Map* map);
    void aiGuard(Entity *poketrainer);
    void aiJump();
    
    void pitAvoidance(Map *map);
    
    void update(float deltaTime, Entity *poketrainer, Entity *platforms, int platformCount, Map* map);
    void render(ShaderProgram* program);

   
        
  
};
