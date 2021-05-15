//
// Created by avalibit on 18/04/2021.
//

#include <ps3.hpp>

namespace RMGB::Graphics::PS3 {
    bool Backend::CheckAPISpecObj(APISpec Obj) {
        return Obj.getAPI() == ON_PS3_API;
    }

    void Backend::Init(int what) {

    }

    void Backend::Update() {

    }

    void Backend::Schedule(RenderCommand command) {

    }

    void Backend::toggleImGui() {

    }

    void Backend::changeSettings() {

    }

    void Backend::Destroy() {

    }
}