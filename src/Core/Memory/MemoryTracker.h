#pragma once
#include <unordered_map>

namespace Athena
{
    struct MemoryAllocInfo
    {
        size_t size;
        const char* tag;
        const char* file;
        int line;
    };

    struct MemoryTrackerData
    {
        std::unordered_map<void*, MemoryAllocInfo> allocations;
        size_t totalAllocatedBytes;
    };

    class MemoryTracker
    {
    public:
        static void Init();
        static void Shutdown();

        static void RecordAlloc(void* ptr, size_t size, const char* tag, const char* file, int line);
        static void FreeAlloc(void* ptr);

        static void DumpReport();

    private:
        static MemoryTrackerData* s_data;
    };

#if DEBUG
    #define ATH_RECORD_MEMORY(ptr, size, tag) ::Athena::MemoryTracker::RecordAlloc(ptr, size, tag, __FILE__, __LINE__)
    #define ATH_FREE_MEMORY(ptr) ::Athena::MemoryTracker::FreeAlloc(ptr)
#else
    #define ATH_RECORD_MEMORY(ptr, size, tag)
    #define ATH_FREE_MEMORY(ptr)
#endif
}
