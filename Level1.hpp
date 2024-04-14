#include "Scene.hpp"

class Level1 : public Scene {
public:
    void initialize(int numLives) override;
    void update(float deltaTime) override;
    void render(ShaderProgram *program) override;
};
