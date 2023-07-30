#include "Backpack.hpp"

#include "util/Log.hpp"

// THIS IS JUST A TEST COMPONENT. IT HAS NO ACTUAL USE.

namespace lei3d {
    // DEFINE_COMPONENT(Backpack, "Backpack");

    Backpack::Backpack(Entity* entity) : Component(entity) {

    }

    //std::string Backpack::GetComponentName() {
    //    return "Backpack";
    //}

    void Backpack::Start() {
        LEI_TRACE("BACKPACK STARTED");
    }
}
