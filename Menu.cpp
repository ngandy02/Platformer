#include "Menu.hpp"

void Menu::initialize(int lives) {
    gameState.nextSceneId = -1;
    
    gameState.player = new Entity();
    gameState.player->entityType = PLAYER;
    gameState.player->setPosition(glm::vec3(5, 0, 0));
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
    gameState.player->lives = lives;
    
    
   
    
//    state.player->height = 0.8f;
//    state.player->width = 0.8f;
        
}
void Menu::update(float deltaTime) {
    if (deltaTime ==-1) {
        gameState.nextSceneId = 1;
    }
}
void Menu::render(ShaderProgram *program) {
    Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "Pokemon Jumper", 1, -0.5, glm::vec3(1.7f, -2.0f, 0));
    Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "Press Return", 1, -0.5, glm::vec3(2.25f, -4.0f, 0));
    Utility::drawText(program, Utility::loadTexture("assets/font1.png"), "to Start!", 1, -0.5, glm::vec3(3.25f, -5.0f, 0));
}
