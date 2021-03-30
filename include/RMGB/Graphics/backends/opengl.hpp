#ifndef OPENGL_BACKEND_H
#define OPENGL_BACKEND_H

#include <Graphics/graphics.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <SDL_opengl.h>
#include <TracyC.h>
#include <Tracy.hpp>
#include <atomic>

namespace RMGB::Graphics::OpenGL {
    class Backend : private RMGB::Graphics::Backend {
        bool CheckAPISpecObj(APISpec Obj);

        void Init(int what = 0xFF) override;

        void Update() override;

        void Schedule(RenderCommand command) override;

        void toggleImGui() override;

        void changeSettings() override;

        bool isMousePressed() override;

        SDL_KeyCode getLastKey() override;

        bool isKeyPressed(SDL_KeyCode key) override;

        glm::vec2 getMousePos() override;

        void Destroy() override;

    private:
        bool imGui = true;
        SDL_Window* window;
        glm::vec<2,int,glm::defaultp> mousePos;
        glm::vec<2,bool,glm::defaultp> mouseButtonState;
        std::queue<SDL_Keycode> keysPressed;
        Settings settings{};
        Settings settingsbak{};
        SDL_GLContext gl_context;
    };
}
#endif