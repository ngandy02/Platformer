#include "Scene.hpp"

class Level3 : public Scene {
public:
    void initialize(int lives) override;
    void update(float deltaTime) override;
    void render(ShaderProgram *program) override;
};
