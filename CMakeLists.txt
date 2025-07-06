cmake_minimum_required(VERSION 3.10.0)
project(SDL_Shooter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/W3 /permissive /utf-8)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/dist)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/dist)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
endif()


set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

if (MSVC)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2")
    message(STATUS "Using SDL2 from ${SDL2_DIR}")
    find_package(SDL2 REQUIRED PATHS ${SDL2_DIR})
    find_package(SDL2_image REQUIRED PATHS ${SDL2_DIR})
    find_package(SDL2_mixer REQUIRED PATHS ${SDL2_DIR})
    find_package(SDL2_ttf REQUIRED PATHS ${SDL2_DIR})
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()

add_executable(${TARGET}
    src/main.cpp
    src/Game.cpp
    src/Scene.cpp
    src/SceneMain.cpp
)

target_link_libraries(${TARGET}
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

if (MSVC)
    if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets
            )
    endif()
    add_custom_command(TARGET ${TARGET} POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_LIBDIR}/SDL2.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/SDL2.dll

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_LIBDIR}/SDL2_image.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/SDL2_image.dll

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_LIBDIR}/SDL2_mixer.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/SDL2_mixer.dll

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_LIBDIR}/SDL2_ttf.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/SDL2_ttf.dll
        )
else(APPLE)
    if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${TARGET}>/../Resources/assets
            )
    endif()
endif()
