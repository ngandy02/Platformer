/**
* Author: Andy Ng
* Assignment: Platformer
* Date due: 2024-04-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

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

#include <SDL_mixer.h>

#include <iostream>
#include <vector>

#include "Entity.hpp"
#include "Map.hpp"

#include "Scene.hpp"
#include "Menu.hpp"
#include "Win.hpp"
#include "Lose.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

#include "Utility.hpp"


using namespace std;




SDL_Window* displayWindow;
bool gameIsRunning = true;
bool paused = false;
int lives = 3;
float missionPass = 0; // 0 = ongoing, 1 = pass, -1 = fail

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

const int CD_QUAL_FREQ    = 44100,  // compact disk (CD) quality frequency
          AUDIO_CHAN_AMT  = 2,
          AUDIO_BUFF_SIZE = 4096;

const char BGM_FILEPATH[] = "assets/Magicdustbin.mp3";
const int  LOOP_FOREVER = -1;  // -1 means loop forever in Mix_PlayMusic; 0 means play once and loop zero times

Mix_Music *gMusic;

Mix_Chunk *gJump;

// Add some variables and switchScene function
Scene *currentScene; // currnet scene of the screen
Scene *scenes[6]; //array of size 6 with  pointers to scene objects

void switchScene(Scene *scene) {
    currentScene = scene; // set currentscene to scene passed in
    currentScene->initialize(lives);
}

void initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    // Start Audio
    Mix_OpenAudio(
        CD_QUAL_FREQ,        // the frequency to playback audio at (in Hz)
        MIX_DEFAULT_FORMAT,  // audio format
        AUDIO_CHAN_AMT,      // number of channels (1 is mono, 2 is stereo, etc).
        AUDIO_BUFF_SIZE      // audio buffer size in sample FRAMES (total samples divided by channel count)
    );
    
    
    // Similar to our custom function load_texture
    gMusic = Mix_LoadMUS(BGM_FILEPATH);
    gJump = Mix_LoadWAV("assets/wooshjump.mp3");
    
    // This will schedule the music object to begin mixing for playback.
    // The first parameter is the pointer to the mp3 we loaded
    // and second parameter is the number of times to loop.
    Mix_PlayMusic(gMusic, LOOP_FOREVER);

    // Set the music to half volume
    // MIX_MAX_VOLUME is a pre-defined constant
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.set_projection_matrix(projectionMatrix);
    program.set_view_matrix(viewMatrix);
    
    glUseProgram(program.get_program_id());
    
    glClearColor(0.17f, 0.13f, 0.07f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

   
    //different screens
    scenes[0] = new Menu();
    scenes[1] = new Level1();
    scenes[2] = new Level2();
    scenes[3] = new Level3();
    scenes[4] = new Win();
    scenes[5] = new Lose();
    switchScene(scenes[0]); // initialize to menu screen
    
}


void processInput() {
    
    currentScene->gameState.player->setMovement(glm::vec3(0));
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        // Some sort of action
                        if (currentScene->gameState.player->hitsBottom) {
                            currentScene->gameState.player->jump = true;
                            Mix_PlayChannel(-1, gJump, 0);
                        }
                        break;
                    case SDLK_RETURN:
                        if (currentScene == scenes[0]) {
                            currentScene->update(-1);
                        }
                    case SDLK_p:
                        currentScene->gameState.paused = true;
                    case SDLK_s:
                        currentScene->gameState.paused= false;
                       
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    //allows the player to hold down the left or right keys with the space bar to move diagonally
    if (keyState[SDL_SCANCODE_LEFT]) { // if the
        currentScene->gameState.player->setMovement(glm::vec3(-1.0f, 0.0f, 0.0f));
        currentScene->gameState.player->indices = currentScene->gameState.player->trainerWalkingAnimations[currentScene->gameState.player->LEFT];
    }
    else if (keyState[SDL_SCANCODE_RIGHT]) {
        currentScene->gameState.player->setMovement(glm::vec3(1.0f, 0.0f, 0.0f));
        currentScene->gameState.player->indices = currentScene->gameState.player->trainerWalkingAnimations[currentScene->gameState.player->RIGHT];
    }
    
    if (keyState[SDL_SCANCODE_UP]){
        currentScene->gameState.player->setMovement(glm::vec3(0.0f, 1.0f, 0.0f));
        currentScene->gameState.player->indices = currentScene->gameState.player->trainerWalkingAnimations[currentScene->gameState.player->UP];
      }
        
    else if (keyState[SDL_SCANCODE_DOWN]){
        currentScene->gameState.player->setMovement(glm::vec3(0.0f, -1.0f, 0.0f));
        currentScene->gameState.player->indices = currentScene->gameState.player->trainerWalkingAnimations[currentScene->gameState.player->DOWN];
      }
          
    // to nomralize the movment of the player if they end up moving more than 1.0
    if (glm::length(currentScene->gameState.player->getMovement()) > 1.0f) {
        currentScene->gameState.player->setMovement(glm::normalize(currentScene->gameState.player->getMovement()));
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void update() {
   float ticks = (float)SDL_GetTicks() / 1000.0f;
   float deltaTime = ticks - lastTicks;
   lastTicks = ticks;
   deltaTime += accumulator;
   if (deltaTime < FIXED_TIMESTEP) {
       accumulator = deltaTime;
       return;
   }
   while (deltaTime >= FIXED_TIMESTEP) {
       // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
       currentScene->update(FIXED_TIMESTEP);
       
       deltaTime -= FIXED_TIMESTEP;
   }
   accumulator = deltaTime;
    
    
    // Inside Update to move the view matrix with respect to the pokemon trainer
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->gameState.player->getPosition().x > 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->gameState.player->getPosition().x, 3.75, 0));
    } else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.set_view_matrix(viewMatrix);
    
    currentScene->render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}


void shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    initialize();
    
    while (gameIsRunning) {
        processInput();
        update();
        
        //updates the lives left and the scene after having updated the data
        
        if (currentScene->gameState.nextSceneId >= 0){
            switchScene(scenes[currentScene->gameState.nextSceneId]); 
        }
        lives = currentScene->gameState.player->lives;
        render();// show or render next level screen
    }
    
    shutdown();
    return 0;
}
