#pragma once

#include "core/Component.hpp"
#include "rendering/Shader.hpp"
#include "core/FlyCamera.hpp"
#include "core/Scene.hpp"

namespace lei3d {

    class ModelInstance;
    class SkyBox;

class RenderSystem {
    public:
        RenderSystem() {}
        ~RenderSystem() {}

        void initialize(int width, int height);
        void draw(const Scene& scene);

    private:
        void lightingPass(const std::vector<ModelInstance*>& objects, FlyCamera& camera);
        void environmentPass(const SkyBox& skyBox, FlyCamera& camera);
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
};

} // lei3d
