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

namespace RMGB::Graphics::OpenGL {
    class Backend : private RMGB::Graphics::Backend {
        /// Returns true if Obj matches the internal API
        /*!
         * Returns true if Obj == OpenGL_API
         * @param Obj an APISpec object
         */
        bool CheckAPISpecObj(APISpec Obj);

        void Init(InitFlags what = (InitFlags)0xFF) override;

        void Update() override;

        void Schedule(RenderCommand command) override;

        void toggleImGui() override;

        void changeSettings() override;

        void Destroy() override;

    private:
        bool imGui = true; /// ImGui toggle
        SDL_Window* window; /// The SDL_Window object
        Settings settings{}; /// A struct containing the settings
        Settings settingsbak{}; /// A backup of the settings - used for comparing whether the settings have changed
        SDL_GLContext gl_context; /// The OpenGL Context for SDL
    };

    class Shader : private RMGB::Graphics::Shader {
    public:
        Shader(std::string& content); /// Uses a reference to a string as the shader source (no copy)
        Shader(std::string content); /// Uses a string as the shader source (copies it)
        Shader(std::ifstream& file); /// Reads the source from a file
        void fromString(std::string& content) override; /// Same as the reference constructor
        void fromFile(std::ifstream& file) override; /// Same as file constructor
    };


}
#endif
