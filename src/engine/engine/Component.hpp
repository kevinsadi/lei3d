#pragma once

#include <string>

namespace lei3d
{
    template<typename C>
    std::string GetComponentName();

#define DEFINE_COMPONENT(name, type) \
template<> std::string GetComponentName<C>() { return name; }

    class Component
    {
    public:
        virtual void Start() {};
        virtual void Update(float deltaTime) {};
        virtual void Render() {};
    };
}