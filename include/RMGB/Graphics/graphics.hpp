#ifndef GRAHPICS_HPP_INDLUDED
#define GRAHPICS_HPP_INDLUDED
#include <SDL.h>
#include <queue>
#include <Utils/logger.hpp>
#define ENOTRECOVERABLE 141	/* State not recoverable */
#define EOWNERDEAD 142	/* Previous owner died */
#include <entt/entt.hpp>
#ifdef ON_PS3
#define GLM_FORCE_CXX98
#endif
#include <glm/glm.hpp>
#include <queue>
#include <mutex>

// NOTE: This is heavily inspired by the Hazel RendererAPI Template

namespace RMGB { namespace Graphics {

    template<typename T>
    class queue
    {
    public:
#ifdef ON_ON_PS3
        queue() {
            m_mutex = SDL_CreateMutex();
        }
        ~queue() {
            SDL_DestroyMutex(m_mutex);
        }
#endif
        void push( const T& value )
        {
#ifdef ON_PS3
            SDL_LockMutex(m_mutex);
#else
            std::lock_guard<std::mutex> lock(m_mutex);
#endif
            m_queue.push(value);
#ifdef ON_PS3
            SDL_UnlockMutex(m_mutex);
#endif
        }

        T pop()
        {
#ifdef ON_PS3
            SDL_LockMutex(m_mutex);
#else
            std::lock_guard<std::mutex> lock(m_mutex);
#endif
            T object = m_queue.front();
            m_queue.pop();
#ifdef ON_PS3
            SDL_UnlockMutex(m_mutex);
#endif
            return object;
        }

    private:
        std::queue<T> m_queue;
#ifndef ON_PS3
        mutable std::mutex m_mutex;
#else
        SDL_mutex* m_mutex;
#endif
    };

    struct Settings {
        bool vsync;
        glm::vec<2,int,glm::defaultp> windowedSize;
        glm::vec<2,int,glm::defaultp> fullScreenSize;
        bool fullscreen;
        bool operator==(Settings other) {
            bool equals = true;
            equals &= windowedSize == other.windowedSize;
            equals &= fullScreenSize == other.fullScreenSize;
            equals &= vsync == other.vsync;
            equals &= fullscreen == other.fullscreen;
            return equals;
        }
        bool operator!=(Settings other) {
            return !operator==(other);
        }
    };

    enum GPU_API {
        OpenGL_API,
        Vulkan_API,
        ON_PS3_API
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
        // These should be here but C++ can't do virtual initalizers so these should be present in anly classes that inherit from this
        // Shader(std::string& content)
        // Shader(std::irstream& file)
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
        virtual void Init(int what = 0xFF) = 0;
        virtual void Update() = 0;
        virtual void Schedule(RenderCommand command) = 0;
        virtual void toggleImGui() = 0;
        virtual void Destroy() = 0;
        virtual void changeSettings() = 0;
        queue<RenderCommand> commands;
    };
} }

#ifdef OPENGL
#include "backends/opengl.hpp"
#endif

#ifdef VULKAN
#error "Vulkan Support is not yet implemented"
#endif

#ifdef ON_PS3
#include "backends/ps3.hpp"
#endif

#endif
