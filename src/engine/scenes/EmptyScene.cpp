#include "EmptyScene.hpp"

//This is just an empty scene with a skybox. 
//Feel free to duplicate it into another scene and add more objects to it, but keep this scene as is.
namespace lei3d {
    EmptyScene::EmptyScene()
    {
        //clown emoji
    }

    EmptyScene::~EmptyScene()
    {
        //clown emoji
    }

    void EmptyScene::OnLoad()
    {
        //std::unique_ptr<Entity> skyboxObj = std::make_unique<Entity>();
        //SkyBox* skybox = skyboxObj->AddComponent<SkyBox>();
        //std::vector<std::string> faces
        //{
        //    "data/skybox/anime_etheria/right.jpg",
        //    "data/skybox/anime_etheria/left.jpg",
        //    "data/skybox/anime_etheria/up.jpg",
        //    "data/skybox/anime_etheria/down.jpg",
        //    "data/skybox/anime_etheria/front.jpg",
        //    "data/skybox/anime_etheria/back.jpg"
        //};
        //skybox->Init(faces);
        //m_Entities.push_back(std::move(skyboxObj));
    }
}
