#pragma once 

#include "Scene.hpp"


#include "engine/Entity.hpp"
#include "engine/Model.hpp"
#include "engine/Shader.hpp"
#include "engine/SkyBox.hpp"

namespace lei3d {
    class TestScene : public Scene {
    private:
        Shader m_MainShader; 
        Entity m_Backpack;
        SkyBox m_Skybox;
    public:
        TestScene();
        ~TestScene();

        void LoadObjects() override;
        void OnUpdate() override;
        void OnRender() override;
    };
}