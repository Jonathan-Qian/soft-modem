cmake_minimum_required(VERSION 3.28.0)
project(soft-modem
    VERSION 0.1.0
    DESCRIPTION "C++ software modem"
    LANGUAGES C CX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES
    # Nothing yet
)

add_executable(soft-modem ${SOURCE_FILES})

target_include_directories(soft-modem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
