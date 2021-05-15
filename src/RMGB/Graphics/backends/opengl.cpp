#include <Graphics/backends/opengl.hpp>
#include <Utils/logger.hpp>

using namespace RMGB::Graphics;

namespace RMGB::Graphics::OpenGL {
    void Backend::Init(int what) {
        if (what & 0b1) {
            // Init SDL
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
                logger::log(logger::FATAL, "SDL Failed to initialize", "OpenGL Layer - Init", __FILE__, __LINE__);
            }
        }

        if (what & 0b01) {
            // Set OpenGL SDL attributes
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        }
        if (what & 0b001) {
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, settings.vsync ? 1 : 0);
        }

        if (what & 0b0001) {
            // Create the window and context
            SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
                                                              (settings.fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
            if (window != nullptr) {
                SDL_DestroyWindow(window);
            }

            window = SDL_CreateWindow("ReMania - GameBox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      settings.fullscreen ? settings.fullScreenSize.x : settings.windowedSize.x,
                                      settings.fullscreen ? settings.fullScreenSize.y : settings.windowedSize.y,
                                      window_flags);
            gl_context = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, gl_context);
            if (settings.vsync) {
                SDL_GL_SetSwapInterval(1); // Enable vsync
            }
        }
        if (what & 0b00001) {
            // Setup ImGui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

            ImGui::StyleColorsDark();
            ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
            ImGui_ImplOpenGL3_Init("#version 150");
        }

        if (what & 0b000001) {

        }

    }

    void Backend::Update() {
        TracyCZoneNC(clearScreen,"Clear screen",0x00ff00,true)
        // Reset window
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        TracyCZoneEnd(clearScreen)
        TracyCZoneNC(renderGeom,"Render Geometry",0x00ff00,true);
        TracyCZoneEnd(renderGeom);
        TracyCZoneNC(imgui_render,"Render ImGui",0x7f007f,true)
        if (imGui) {
            if (settings != settingsbak) {
                changeSettings();
            }
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();
            ImGui::Begin("Settings");
            ImGui::Checkbox("VSync",&settings.vsync);
            ImGui::Checkbox("Fullscreen",&settings.fullscreen);
            SDL_Rect screenSize{};
            SDL_GetDisplayBounds(0,&screenSize);
            auto& io = ImGui::GetIO();
            ImGui::LabelText("Fullscreen resolution","");
            ImGui::SliderInt("X",&settings.fullScreenSize.x,0,(int)ImGui::GetIO().DisplaySize.x);
            ImGui::SliderInt("Y",&settings.fullScreenSize.y,0,(int)ImGui::GetIO().DisplaySize.y);
            ImGui::LabelText("Window resolution","");
            ImGui::SliderInt("X",&settings.windowedSize.x,0,(int)ImGui::GetIO().DisplaySize.x);
            ImGui::SliderInt("Y",&settings.windowedSize.y,0,(int)ImGui::GetIO().DisplaySize.y);
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        TracyCZoneEnd(imgui_render)
        TracyCZoneNC(swap,"Swap window",0xff00,true)
        SDL_GL_SwapWindow(window);
        TracyCZoneEnd(swap)
    }

    void Backend::Schedule(RMGB::Graphics::RenderCommand command) {
        TracyCZoneNC(schedule,"Add command to render queue",0x00ff00,true)
        commands.push(command);
        TracyCZoneEnd(schedule)
    }

    void Backend::toggleImGui() {
        imGui = !imGui;
    }

    bool Backend::CheckAPISpecObj(RMGB::Graphics::APISpec Obj) {
        return Obj.getAPI() == OpenGL_API;
    }

    void Backend::changeSettings() {
        int what = 0xFF;
        what ^= 0b1;
        this->Init(what);
        settingsbak = settings;
    }

    void Backend::Destroy() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    Shader::Shader(std::string& content) {

    }

    Shader::Shader(std::ifstream& file) {

    }

    void Shader::fromFile(std::ifstream& file) {

    }

    void Shader::fromString(std::string& content) {

    }
};
