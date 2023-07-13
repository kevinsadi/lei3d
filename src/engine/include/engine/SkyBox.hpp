#ifndef SKYBOX
#define SKYBOX

#include <glad/glad.h> 
#include <vector>
#include <string>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "engine/Shader.hpp"

namespace lei3d
{
    class SkyBox
    {
    public:
        unsigned int skyboxVAO;
        unsigned int skyboxVBO;
        Shader skyboxShader;
        unsigned int cubeMapTexture;

        SkyBox();

        void loadCubemap(std::vector<std::string> faces);
        void renderCubemap();
    };
}




#endif // SKYBOX