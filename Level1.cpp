#include "Level1.hpp"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 2

using namespace std;

unsigned int level1_data[] =
{
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1
};

void Level1::initialize(int numLives) {
    
    gameState.nextSceneId = -1;
    
    GLuint mapTextureID = Utility::loadTexture("assets/firetiles.png");
    gameState.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 7, 1);
    // Initialize Player
    gameState.player = new Entity();
    gameState.player->entityType = PLAYER;
    gameState.player->setPosition(glm::vec3(1, -5, 0));
    gameState.player->setMovement(glm::vec3(0));
    gameState.player->mAcceleration = glm::vec3(0, -9.81f, 0);
    gameState.player->mSpeed = 3.0f;
    gameState.player->textureID = Utility::loadTexture("assets/pokemontrainer.png");
    
    gameState.player->trainerWalkingAnimations[gameState.player->LEFT] = new int[4] {4, 5, 6, 7};
    gameState.player->trainerWalkingAnimations[gameState.player->RIGHT] = new int[4] {8, 9, 10, 11};
    gameState.player->trainerWalkingAnimations[gameState.player->UP] = new int[4] {12, 13, 14, 15};
    gameState.player->trainerWalkingAnimations[gameState.player->DOWN] = new int[4] {0, 1, 2, 3};
    
    gameState.player->indices = gameState.player->trainerWalkingAnimations[gameState.player->DOWN]; // set indices to an animation up, down, left, or right animation on the spritesheet
  
    
    gameState.player->frames = 4;
    gameState.player->index = 0;
    gameState.player->cols = 4;
    gameState.player->rows = 4;
    gameState.player->animationTime = 0;
    
    gameState.player->jumpPower = 6.5f;
    gameState.player->lives = 3; //
    

    gameState.player->width = 0.9;
    gameState.player->height = 0.9;
    
    
    
    gameState.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    
    gameState.enemies[0].textureID = Utility::loadTexture("assets/giratina.png");
    gameState.enemies[0].entityType = ENEMY;
    gameState.enemies[0].aiType = JUMPER;
    gameState.enemies[0].aiState = JUMPING;
    gameState.enemies[0].setPosition(glm::vec3(4.0f, -4.0f, 0.0f));
    gameState.enemies[0].mAcceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    gameState.enemies[0].mSpeed = 1;
    gameState.enemies[0].jumpPower = 3.0f;

        
    gameState.enemies[1].textureID = Utility::loadTexture("assets/dialga.png");
    gameState.enemies[1].entityType = ENEMY;
    gameState.enemies[1].aiType = GUARD;
    gameState.enemies[1].aiState = IDLE;
    gameState.enemies[1].setPosition(glm::vec3(11.0f, 0.0f, 0.0f));
    gameState.enemies[1].mAcceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    gameState.enemies[1].mSpeed = 1;
    gameState.enemies[1].jumpPower = 0.0f;

    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        gameState.enemies[i].isAlive = true;
        gameState.enemies[i].width = 0.9;
        gameState.enemies[i].height = 0.9;
    }
}
void Level1::update(float deltaTime) {
    gameState.player->update(deltaTime, gameState.player, gameState.enemies, LEVEL1_ENEMY_COUNT, gameState.map);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        gameState.enemies[i].update(deltaTime, gameState.player, gameState.enemies, LEVEL1_ENEMY_COUNT, gameState.map);
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        if (gameState.enemies[i].hitsBottom){
            gameState.enemies[i].jump = true;
        }
    }
    if (gameState.player->lives == 0) {
        gameState.nextSceneId = 5;
    }

    if (gameState.player->getPosition().x >= 13){
        gameState.nextSceneId = 2;
    }
    if (!(gameState.player->isAlive) || gameState.player->getPosition().y <= -9) {
        gameState.player->lives -= 1;
        gameState.player->setPosition(glm::vec3(1, 0, 0));
        gameState.player->isAlive = true;
    }
}
void Level1::render(ShaderProgram *program) {
    if (gameState.paused == true){
        Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "PAUSED", 1.0f, -0.1f, glm::vec3(2, -1, 0));
        Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "Press S to resume", 1.0f, -0.1f, glm::vec3(6, -3, 0));
    }
    else{
        Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "Lives:" + to_string(gameState.player->lives), 1.0f, -0.1f, glm::vec3(1, -1, 0));
        Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "Next LVL->", 1.0f, -0.1f, glm::vec3(6, -3, 0));
        
        gameState.map->render(program);
        gameState.player->render(program);
        
        for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
            gameState.enemies[i].render(program);
        }
    }
}
