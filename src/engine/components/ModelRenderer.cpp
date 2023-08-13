#include "ModelRenderer.hpp"

#include "logging/LogGLM.hpp"

namespace lei3d
{
    ModelRenderer::ModelRenderer(Entity& entity) : Component(entity)
    {      
    }

    ModelRenderer::~ModelRenderer()
    {
    }

    void ModelRenderer::Init(Model* model, Shader* shader) {
        m_Model = model;
        m_Shader = shader;
    }

    void ModelRenderer::Render()
    {
        glm::mat4 model = m_Entity.GetModelMat();
        FlyCamera& camera = SceneManager::ActiveScene().MainCamera();
        glm::mat4 view = camera.GetView();
        glm::mat4 proj = camera.GetProj();
        m_Shader->setUniformMat4("u_Model", model);
        m_Shader->setUniformMat4("u_View", view);
        m_Shader->setUniformMat4("u_Proj", proj);
        
        if (m_Model) {
            m_Model->Draw(*m_Shader);
        }
    }
}