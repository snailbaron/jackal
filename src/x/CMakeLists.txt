add_library(x STATIC
    exceptions.cpp
    fs.cpp
    mmap.cpp
)
target_include_directories(x PUBLIC include)