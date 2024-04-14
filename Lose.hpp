#include "Scene.hpp"

class Lose : public Scene {
public:
    void initialize(int lives) override;
    void update(float deltaTime) override;
    void render(ShaderProgram *program) override;
};
