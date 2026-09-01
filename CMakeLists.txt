cmake_minimum_required(VERSION 3.10.0)
project(crafter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
)

add_executable(craft ${SOURCES})

target_include_directories(craft PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

if(EXISTS "${CMAKE_SOURCE_DIR}/res")

    add_custom_command(
        TARGET craft POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/res"
                "$<TARGET_FILE_DIR:craft>/res"
        COMMENT "Copying resources..."
    )
    
endif()

target_compile_options(craft PRIVATE

    $<$<CONFIG:Release>:
        -O3
        -ffast-math
        -march=native
    >

    $<$<CONFIG:RelWithDebInfo>:
        -O2
        -g
    >

    $<$<CONFIG:Debug>:
        -g
    >

)