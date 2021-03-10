#include <SDL.h>
#include <queue>
#include <variant>
#include <Utils/logger.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#ifdef OPENGL
#include <SDL_opengl.h>
#endif

#ifdef VULKAN
#include <SDL_vulkan.h>
#endif

// NOTE: This is heavily inspired by the Hazel RendererAPI Template

namespace RMGB::Graphics {

    enum GPU_API {
        OpenGL,
        Vulkan
    };

    class APISpec {
    private:
        GPU_API api;
    public:
        GPU_API getAPI() {
            return api;
        }
    };

    class Shader : private APISpec {
    public:
        virtual void fromString(std::string& content) = 0;
        virtual void fromFile(std::ifstream& content) = 0;
    };

    typedef SDL_Texture* Texture;

    class IndexBuffer : private APISpec {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual uint32_t getSize() = 0;
        virtual void setData(const void* data, size_t size) = 0;
    };

    class VertexBuffer : private APISpec {
    public:
        virtual void setSize(size_t elemSize) = 0;
        virtual const void bind() = 0;
        virtual const void unbind() = 0;
        virtual uint32_t getSize() = 0;
        virtual void setData(const void* data, size_t size) = 0;
    };

    class VertexArray : private APISpec {
    public:
        virtual const void bind() = 0;
        virtual const void unbind() = 0;
        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> VBO) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> IBO) = 0;
        virtual const std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() = 0;
        virtual const std::shared_ptr<IndexBuffer> getIndexBuffer() = 0;
    };

    enum Command_Type {
        Render,
        Clear,
        SetViewport,
        Refresh
    };

    struct RenderCommand {
        Command_Type type;
        // For set viewport command
        glm::vec2 viewport = {-1,-1};
        // For clear command
        glm::vec4 clearColour = {-1,-1,-1,-1};
        // For refresh
        // None needed ;)
        // For render command
        std::shared_ptr<VertexArray> renderObject;
        uint32_t indexCount = -1;
    };

    class Backend {
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Schedule(RenderCommand command) = 0;
        virtual void toggleImGui() = 0;
        std::queue<RenderCommand> commands;
    };
}