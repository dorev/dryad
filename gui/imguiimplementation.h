#pragma once

template <class ImGuiBackend>
class ImGuiImplementation
{
#define BRIDGE_WITH_BACKEND(method) \
    template <class... args_t> \
    auto method(args_t&&... args) \
    { \
        return _Backend.method(std::forward<args_t>(args)...); \
    } \

public:
    BRIDGE_WITH_BACKEND(Init);
    BRIDGE_WITH_BACKEND(NewFrame);
    BRIDGE_WITH_BACKEND(HandleEvents);
    BRIDGE_WITH_BACKEND(Render);
    BRIDGE_WITH_BACKEND(Shutdown);

private:
    ImGuiBackend _Backend;
};

struct ImGuiImplementationConfig
{
    const char* name;
    bool* shuttingDown;
};
