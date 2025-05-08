#include "MemoryTracker.h"
#include "Core/Log.h"

namespace Athena
{
    MemoryTrackerData* MemoryTracker::s_data = nullptr;

    void MemoryTracker::Init()
    {
        s_data = new MemoryTrackerData();
    }

    void MemoryTracker::Shutdown()
    {
        DumpReport();
        delete s_data;
    }

    void MemoryTracker::RecordAlloc(void* ptr, size_t size, const char* tag, const char* file, int line)
    {
        s_data->allocations[ptr] = { size, tag, file, line };
        s_data->totalAllocatedBytes += size;
    }

    void MemoryTracker::FreeAlloc(void* ptr)
    {
        auto it = s_data->allocations.find(ptr);
        if (it != s_data->allocations.end()) {
            s_data->totalAllocatedBytes -= it->second.size;
            s_data->allocations.erase(it);
        }
        else 
        {
            LOG_ERROR("[MemoryTracker] Attempted to free untracked pointer!");
        }
    }

    void MemoryTracker::DumpReport()
    {
        LOG_INFO("----- Memory Dump -----");
        for (const auto& [ptr, info] : s_data->allocations)
        {
            std::string ptrAddr = fmt::format("{:p}", fmt::ptr(ptr));
            LOG_WARN("Leaked {0} bytes at {1} (TAG: {2}) ({3}:{4})", info.size, ptrAddr, info.tag, info.file, info.line);
        }
        LOG_INFO("Total leaked: {} bytes", s_data->totalAllocatedBytes);
    }
}
