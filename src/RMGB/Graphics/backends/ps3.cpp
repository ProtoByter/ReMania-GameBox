//
// Created by avalibit on 18/04/2021.
//

#include <ps3.hpp>

namespace RMGB::Graphics::PS3 {
    bool Backend::CheckAPISpecObj(APISpec Obj) {
        return false;
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

    bool Backend::isMousePressed() {
        return false;
    }

    SDL_Keycode Backend::getLastKey() {
        return nullptr;
    }

    bool Backend::isKeyPressed(SDL_Keycode key) {
        return false;
    }

    glm::vec2 Backend::getMousePos() {
        return glm::vec2();
    }

    void Backend::Destroy() {

    }
}