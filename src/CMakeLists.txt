if(MSVC)
    # /Wall is too much, but /W4 is not enough, so use /Wall and explicitly
    # disable some of the warnings.
    add_compile_options(/Wall /utf-8
        /wd4464  # relative include path contains '..'
        /wd4820  # padding after struct
        /wd4514  # unreferenced inline function has been removed
        /wd4623  # default constructor of base class is implicitly deleted
        /wd4625  # copy constructor of base class is implicitly deleted
        /wd4626  # assignment operator was implicitly defined as deleted
        /wd4668  # symbol is not defined as a preprocessor macro
        /wd5026  # move constructor was implicitly defined as deleted
        /wd5027  # move assignment operator was implicitly defined as deleted
        /wd5045  # compiler will insert Spectre mitigation for memory load
        /wd4365  # conversion: signed/unsigned mismatch
        /wd5262  # implicit fall-through
        /wd4868  # compiler may not enforce left-to-right evaluation order in braced initializer list
    )
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(client)
add_subdirectory(client-server-api)
add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(pack)
add_subdirectory(server)
add_subdirectory(x)