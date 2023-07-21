#include "Backpack.hpp"

#include "util/Log.hpp"

namespace lei3d {
    DEFINE_COMPONENT(Backpack, "Backpack");

    Backpack::Backpack(Entity* entity) : Component(entity) {

    }

    void Backpack::Start() {
        LEI_TRACE("BACKPACK STARTED");
    }
}
