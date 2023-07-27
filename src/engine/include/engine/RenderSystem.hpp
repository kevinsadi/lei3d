#ifndef LEI3D_RENDERSYSTEM_HPP
#define LEI3D_RENDERSYSTEM_HPP

#include "engine/Entity.hpp"
#include "engine/Shader.hpp"
#include "engine/FlyCamera.hpp"
#include "engine/SkyBox.hpp"

namespace lei3d {

// TODO: temporary, put lights in here until logan's branch merge
class DirectionalLight {
public:
    DirectionalLight(glm::vec3 dir = glm::vec3{0, -1, 0}, glm::vec3 col = glm::vec3{1.f}, float intensity = 1.f) :
            direction(glm::normalize(dir)), color(col), intensity(intensity) {}

    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

class RenderSystem {
public:
    RenderSystem() {}
    ~RenderSystem() {}

    void initialize(int width, int height);
    void draw(const Entity& object, const SkyBox& skyBox, FlyCamera* camera);

private:
    void lightingPass(const Entity& object, FlyCamera* camera);
    void environmentPass(const SkyBox& skyBox, FlyCamera* camera);
    void postprocessPass();

    // offscreen render target objects
    unsigned int FBO;
    unsigned int rawTexture;
    unsigned int saturationMask;
    unsigned int depthStencilTexture;
    unsigned int finalTexture;

    unsigned int dummyVAO;  // used to draw full-screen "quad"

    int scwidth, scheight;

    // shaders
    Shader forwardShader;
    Shader postprocessShader;

    ///< temporary light
    DirectionalLight directionalLight{{0.1, -0.5, 0.5}};
};

} // lei3d

#endif //LEI3D_RENDERSYSTEM_HPP
