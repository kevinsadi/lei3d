#pragma once

#include <glad/glad.h> 
#include <vector>
#include <string>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "rendering/Shader.hpp"

#include "core/Component.hpp"

namespace lei3d
{
    class Component;

    class SkyBox : public Component
    {
    public:
        unsigned int skyboxVAO;
        unsigned int skyboxVBO;
        Shader skyboxShader;
        unsigned int cubeMapTexture;

        SkyBox(Entity* entity);
        ~SkyBox();

        //std::string GetComponentName() override;
        void Init(std::vector<std::string> faces);

        void Render() override;

    private:
        void loadCubemap(std::vector<std::string> faces);
    };
}