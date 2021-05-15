#ifndef GRAHPICS_HPP_INDLUDED
#define GRAHPICS_HPP_INDLUDED

#include <SDL.h>
#include <queue>
#include <Utils/logger.hpp>
#include <entt/entt.hpp>

#ifdef ON_PS3

// Special Values for PS3 since otherwise this doesn't compile

#define ENOTRECOVERABLE 141
#define EOWNERDEAD 142

// Also GLM doesn't seem to like the PS3 so this fixes that

#define GLM_FORCE_CXX98
#endif

#include <glm/glm.hpp>
#include <queue>
#include <mutex>
#include <memory>

// NOTE: This is heavily inspired by the Hazel RendererAPI Template

namespace RMGB { namespace Graphics {


    /// The class used for multithreaded render queues
    /*!
      This is a custom class that represents a FIFO queue, it also uses SDL Mutexes on the PS3 whilst it uses C++11 \c lock_guard objects on other platforms
      @param T The type of object stored in the queue
     */
    template<typename T>
    class queue
    {
    public:
#ifdef ON_PS3
        /// Constructors and destructors for creating and destroying the \c SDL_mutex which is used on the PS3
        queue() {
            m_mutex = SDL_CreateMutex();
        }
        ~queue() {
            SDL_DestroyMutex(m_mutex);
        }
#endif
        /// Pushes an object onto the top of the queue
        /*!
          This function pushes an object onto the queue, it also handles locking and unlocking the various mutexes used on different platforms
          @param value The object to push onto the stack
         */
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

        /// This function takes the first object in the queue and returns it
        /*!
          This function pops an object off the queue and handles the mutexes used to keep this object thread safe on many platforms
          @returns The object that was popped off the stack
        */
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


    /// Struct used generally to hold video settings
    struct Settings {
        bool vsync;
        glm::vec<2,int,glm::defaultp> windowedSize;
        glm::vec<2,int,glm::defaultp> fullScreenSize;
        bool fullscreen;
        /// Used to check for equality (used to check if something has changed)
        bool operator==(Settings other) {
            bool equals = true;
            equals &= windowedSize == other.windowedSize;
            equals &= fullScreenSize == other.fullScreenSize;
            equals &= vsync == other.vsync;
            equals &= fullscreen == other.fullscreen;
            return equals;
        }

        /// Used to check inequality
        bool operator!=(Settings other) {
            return !operator==(other);
        }
    };

    /// Specifies the API used
    enum GPU_API {
        OpenGL_API, /// OpenGL
        Vulkan_API, /// Vulkan
        ON_PS3_API  /// PS3 (RSX)
    };

    /// Class used as a parent for all API Specific classes (Backends, Shaders etc)
    class APISpec {
    private:
        /// The API of the class
        GPU_API api;
    public:
        /// Gets the API of the object
        GPU_API getAPI() {
            return api;
        }
    };

    /// Parent class for all Shaders
    /*! These constructors should be present on all child classes but C++ can't do virtual initalizers:
         @code
         Shader(std::string& content)
         Shader(std::ifstream& file)
         @endcode
        */
    class Shader : private APISpec {
    public:
        virtual void fromString(std::string& content) = 0;
        virtual void fromFile(std::ifstream& content) = 0;
    };

    typedef SDL_Texture* Texture;

    /// Parent class for all API-Specific Index Buffers
    class IndexBuffer : private APISpec {
    public:
        /// Bind the Index Buffer
        virtual void bind() const = 0;
        /// Unbinds the Index Buffer
        virtual void unbind() const = 0;
        /// Gets the size of the elements in the index buffer
        virtual uint32_t getSize() const = 0;
        /// Sets the data in the index buffer
        virtual void setData(const void* data, size_t size) = 0;
    };

    /// Parent class for all API-Specific Vertex Buffers
    class VertexBuffer : private APISpec {
    public:
        /// Sets the size of each element
        /*!
         * This should reset the data within the vertex buffer
         */
        virtual void setSize(size_t elemSize) = 0;
        /// Binds the Vertex Buffer
        virtual const void bind() const = 0;
        /// Unbinds the Vertex Buffer
        virtual const void unbind() const = 0;
        /// Gets the size of the elements in the Vertex Buffer
        virtual uint32_t getSize() const = 0;
        /// Sets the data in the vertex buffer
        virtual void setData(const void* data, size_t size) = 0;
    };

    /// Parent class for all API-Specific Vertex Buffers
    class VertexArray : private APISpec {
    public:
        /// Binds the Vertex Array
        virtual const void bind() const = 0;
        /// Unbinds the Vertex Array
        virtual const void unbind() const = 0;
        /// Adds a Vertex Array
        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> VBO) = 0;
        /// Sets an Index Buffer
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> IBO) = 0;
        /// Gets an std::vector of all the vertex buffers in the Vertex Array
        virtual const std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const = 0;
        /// Gets the Index Buffer of the Vertex Array
        virtual const std::shared_ptr<IndexBuffer> getIndexBuffer() const = 0;
    };

    /// Command types for the RenderCommand
    enum Command_Type {
        Render,
        Clear,
        SetViewport,
        Refresh
    };

    /// Contains the commands that are stored on the Render Queue
    struct RenderCommand {
        Command_Type type; /// The command type
        // For set viewport command
        glm::vec2 viewport = {-1,-1}; /// The new viewport size
        // For clear command
        glm::vec4 clearColour = {-1,-1,-1,-1}; /// The new clear colour
        // For refresh
        // None needed ;)
        // For render command
        std::shared_ptr<Shader> shader; /// The shader to render an object with
        std::shared_ptr<VertexArray> renderObject; /// The object to render
        uint32_t indexCount = -1; /// The number of indexes to render
    };

    /// Parent class for all the Rendering backends
    class Backend {
    public:
        /// Initalizes the backend
        /*!
         * @param what What needs to be initalized
         */
        virtual void Init(int what = 0xFF) = 0;
        /// Update the screen
        virtual void Update() = 0;
        /// Add a command to the Render Queue
        virtual void Schedule(RenderCommand command) = 0;
        /// Turn ImGui on/off
        virtual void toggleImGui() = 0;
        /// Destory the Backend
        virtual void Destroy() = 0;
        /// Set the settings
        virtual void changeSettings() = 0;
        queue<RenderCommand> commands; /// The current render queue
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
