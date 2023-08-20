#pragma once

#include <vector>
#include <string>

#include "core/Component.hpp"

#include "rendering/Model.hpp"
#include "rendering/Shader.hpp"

#include "logging/Log.hpp"

namespace lei3d
{
    class ModelInstance : public Component
    {
    public:
        Model* m_Model;

        ModelInstance(Entity& entity);
        ~ModelInstance();
        
        void Init(Model* model);

        void Draw(Shader* shader);

    private:
    };


}