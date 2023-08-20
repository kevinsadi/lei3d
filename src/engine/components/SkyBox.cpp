#include "SkyBox.hpp"

#include "core/SceneManager.hpp"
#include "logging/GLDebug.hpp"

namespace lei3d
{
    // DEFINE_COMPONENT(SkyBox, "SkyBox");

    SkyBox::SkyBox(Entity& entity) : Component(entity)
    {
    }

    SkyBox::~SkyBox() {
        GLCall(glDeleteBuffers(1, &skyboxVBO));
        GLCall(glDeleteVertexArrays(1, &skyboxVBO));
    }

    //std::string SkyBox::GetComponentName() {
    //    return "SkyBox";
    //}

    void SkyBox::Init(const std::vector<std::string>& faces) {
        loadCubemap(faces);

        skyboxShader.setInt("u_TexSkybox", 0);
    }

    void SkyBox::loadCubemap(const std::vector<std::string>& faces)
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
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindVertexArray(0));

        this->cubeMapTexture = textureID;
        this->skyboxVAO = skyboxVAO;
        this->skyboxVBO = skyboxVBO;
    }

    void SkyBox::Render()
    {
        auto camera = SceneManager::ActiveScene().MainCamera();

        //C++ doesn't like it if you don't declare these beforehand (initial ref. to non-const value)
        glm::mat4 proj = camera.GetProj();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetView()));
        glm::mat4 model = glm::identity<glm::mat4>();
        skyboxShader.setUniformMat4("u_Proj", proj);
        skyboxShader.setUniformMat4("u_View", skyboxView);
        skyboxShader.setUniformMat4("u_Model", model);
        skyboxShader.bind();

        GLCall(glDepthFunc(GL_LEQUAL)); // we change the depth function here to it passes when testingdepth value is equal to what is current stored
        GLCall(glBindVertexArray(skyboxVAO));
        GLCall(glActiveTexture(GL_TEXTURE0)); //! could be the problem
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        GLCall(glBindVertexArray(0));
        GLCall(glDepthFunc(GL_LESS)); // set depth function back to normal
    }
}