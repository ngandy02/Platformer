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

#include "Entity.hpp"
#include "Map.hpp"
#include "Utility.hpp"


struct GameState {
    Map *map;
    Entity *player;
    Entity *enemies;
    int nextSceneId;
    bool paused = false;
};

class Scene {
public:
    GameState gameState;
    virtual void initialize(int lives) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(ShaderProgram *program) = 0;
};
