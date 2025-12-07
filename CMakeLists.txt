cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(myproto
    LANGUAGES C
    VERSION 0.0.1
)

add_compile_definitions(
    MP_V_MAJOR=${PROJECT_VERSION_MAJOR}
    MP_V_MINOR=${PROJECT_VERSION_MINOR}
    MP_V_PATCH=${PROJECT_VERSION_PATCH}
)

set(SRC_LIST
    "src/main.c"
    "src/myproto.c"
)

add_executable(myproto ${SRC_LIST})
