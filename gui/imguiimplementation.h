#pragma once

template <class ImGuiBackend>
class ImGuiImplementation
{
#define BRIDGE_WITH_BACKEND(method) \
    template <class... Args> \
    auto method(Args&&... args) \
    { \
        return _Backend.method(std::forward<Args>(args)...); \
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
