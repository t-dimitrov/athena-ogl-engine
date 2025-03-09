#include "Core/Log.h"
#include "Core/Core.h"
#include <vector>

int main() 
{
    Athena::Logger::Init("");


    Athena::Logger::Shutdown();

    std::vector<int> vec = {1, 2, 3, 4};
    LOG_INFO("My message to the {1} is {0}, {1}", "Hello", "World");
    LOG_DEBUG("Here is my array: {}", vec);
    LOG_WARN("This is a warning because: {} potential error ({})", "Something might not work", 123);
    LOG_ERROR("This is an error message: status ({})", 321);

    std::string statusCode = "F@7AL";
    LOG_FATAL("This is a fatal message: {}", statusCode);

    ATH_ASSERT(vec.size() == 0, "Array is full!");
}
