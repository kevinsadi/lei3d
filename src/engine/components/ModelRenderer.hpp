#pragma once

#include <vector>
#include <string>

#include "core/Component.hpp"

#include "rendering/Model.hpp"
#include "rendering/Shader.hpp"

#include "logging/Log.hpp"

namespace lei3d
{
    class Component;

    class ModelRenderer : public Component
    {
    public:
        Model* m_Model;
        Shader* m_Shader;    //This is temporary. We want to abstract the shader system eventually to handle them better.
        
        ModelRenderer(Entity* entity);
        ~ModelRenderer();
        
        void Init(Model* model, Shader* shader);

        void Render() override;

    private:
    };


}