#ifndef OPENGL_BACKEND_H
#define OPENGL_BACKEND_H

#include <Graphics/graphics.hpp>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <SDL_opengl.h>
#include <TracyC.h>
#include <Tracy.hpp>
#include <atomic>

#ifdef NDEBUG
#include "shaders-OpenGL.hpp"
#else
#define RMGB_SHADER_FROM_FILE
#endif

namespace RMGB::Graphics::OpenGL {
    class Backend : private RMGB::Graphics::Backend {
        bool CheckAPISpecObj(APISpec Obj);

        void Init(int what = 0xFF) override;

        void Update() override;

        void Schedule(RenderCommand command) override;

        void toggleImGui() override;

        void changeSettings() override;

        void Destroy() override;

    private:
        bool imGui = true;
        SDL_Window* window;
        Settings settings{};
        Settings settingsbak{};
        SDL_GLContext gl_context;
    };

    class Shader : private RMGB::Graphics::Shader {
    public:
        Shader(std::string& content);
        Shader(std::ifstream& content);
        void fromString(std::string& content) override;
        void fromFile(std::ifstream& content) override;
    };


    enum InitFlags {
        SDL = 0b1,
        SDL_GL = 0b01,
        WINDOW = 0b001,
        IMGUI = 0b0001,
        SHADERS = 0b00001
    };
}
#endif
