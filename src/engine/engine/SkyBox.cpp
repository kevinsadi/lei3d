#include "SkyBox.hpp"

#include "util/GLDebug.hpp"

namespace lei3d
{
    SkyBox::SkyBox()
    {
        // clown emoticon //
    }

    void SkyBox::loadCubemap(std::vector<std::string> faces)
    {
        // load cubemap textures
        stbi_set_flip_vertically_on_load(false);

        unsigned int textureID;
        GLCall(glGenTextures(1, &textureID));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                ); // valid because the cube maps are internally indexed.
                stbi_image_free(data);
            }
            else
            {
                LEI_WARN("Cubemap texture failed to load at path: " + faces[i]);
                stbi_image_free(data);
            }
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        
        // load skybox shader
        skyboxShader = Shader("./data/shaders/skybox.vert", "./data/shaders/skybox.frag");

        // set up VAO/VBO (remember, array object references the buffer object )
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };


        unsigned int skyboxVAO, skyboxVBO;
        GLCall(glGenVertexArrays(1, &skyboxVAO));
        GLCall(glGenBuffers(1, &skyboxVBO));
        GLCall(glBindVertexArray(skyboxVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

        this->skyboxShader = skyboxShader;
        this->cubeMapTexture = textureID;
        this->skyboxVAO = skyboxVAO;
        this->skyboxVBO = skyboxVBO;
    }

    void SkyBox::renderCubemap()
    {
        /*
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        // set the view and proj matrix
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        */
    }



}