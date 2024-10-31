#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <cstdio>
#include <utility>

class Logger
{
public:

    using LineBuffer = std::unique_ptr<char[]>;
    static constexpr size_t MaxLineSize = 256;

    template <class... Args>
    static void Log(const char* format, Args&&... args)
    {
        LineBuffer buffer = std::make_unique<char[]>(MaxLineSize);
        std::snprintf(buffer.get(), MaxLineSize, format, std::forward<Args>(args)...);
        std::lock_guard lock(_LogMutex);
        _LogLines.push_back(std::move(buffer));
    }

    static const std::vector<LineBuffer>& GetLogLines()
    {
        return _LogLines;
    }

private:
    static inline std::vector<LineBuffer> _LogLines;
    static inline std::mutex _LogMutex;
};

#if defined(_MSC_VER)
    #define DRYAD_GUI_DEBUG_BREAK() __debugbreak()
    #define DRYAD_GUI_FUNCTION __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
    #define DRYAD_GUI_DEBUG_BREAK() __builtin_trap()
    #define DRYAD_GUI_FUNCTION __PRETTY_FUNCTION__
#else
    #define DRYAD_GUI_DEBUG_BREAK() assert(false)
    #define DRYAD_GUI_FUNCTION __FUNCTION__
#endif

#define DRYAD_GUI_LOG(format, ...) printf("[DryadGUI] " format "\n", ##__VA_ARGS__); Logger::Log(format "\n", ##__VA_ARGS__)
#define DRYAD_GUI_LOG_WARNING(format, ...) printf("[DryadGUI] [WARNING] {%s} " format "\n", DRYAD_GUI_FUNCTION, ##__VA_ARGS__); Logger::Log("[WARNING] {%s} " format "\n", DRYAD_GUI_FUNCTION, ##__VA_ARGS__)
#define DRYAD_GUI_LOG_ERROR(format, ...) printf("[DryadGUI] [ERROR] {%s} " format " [%s l.%d]\n", DRYAD_GUI_FUNCTION, ##__VA_ARGS__, __FILE__, __LINE__);

#define DRYAD_GUI_DEBUG_ASSERT(condition, format, ...) \
if (!(condition)) \
{ \
    DRYAD_GUI_LOG("[DryadGUI] [ASSERT] " format "\n", ##__VA_ARGS__); \
    DRYAD_GUI_DEBUG_BREAK(); \
}
