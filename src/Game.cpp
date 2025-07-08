#include "Game.h"
#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Game::Game(void) {}

Game &Game::getInstance(void)
{
    static Game instance;
    return instance;
}

Game::~Game(void)
{
    clean();
}

/**
 * @brief 运行游戏主循环
 */
void Game::run(void)
{
    while (isRunning)
    {
        auto frameStart = SDL_GetTicks();

        handleEvents();
        update(deltaTime);
        render();
        
        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;
        if (diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }
    }
}

/**
 * @brief 初始化游戏
 */
void Game::init(void)
{
    frameTime = 1000 / FPS;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    window = SDL_CreateWindow("太空战机", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s\n", IMG_GetError());
        isRunning = false;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_WAVPACK) != (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_WAVPACK))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        isRunning = false;
    }

    // 设置音效channel数量
    Mix_AllocateChannels(32);

    // 设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_Volume(-1, MIX_MAX_VOLUME / 2);

    currentScene = new SceneMain();
    currentScene->init();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

/**
 * @brief 清理游戏资源
 */
void Game::clean(void)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;
    }

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

/**
 * @brief 切换场景
 * @param newScene 新场景指针
 */
void Game::changeScene(Scene *newScene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = newScene;
    currentScene->init();
}

/**
 * @brief 处理事件
 */
void Game::handleEvents(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvents(event);
    }
}

/**
 * @brief 更新游戏状态
 */
void Game::update(float dt)
{
    currentScene->update(dt);
}

/**
 * @brief 渲染游戏内容
 */
void Game::render(void)
{
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);
}

/**
 * @brief 获取窗口
 * @return SDL_Window* 窗口指针
 */
SDL_Window *Game::getWindow(void)
{
    return window;
}

/**
 * @brief 获取渲染器
 * @return SDL_Renderer* 渲染器指针
 */
SDL_Renderer *Game::getRenderer(void)
{
    return renderer;
}

/**
 * @brief 获取窗口宽度
 * @return int 窗口宽度
 */
int Game::getWindowWidth(void)
{
    return windowWidth;
}

/**
 * @brief 获取窗口高度
 * @return int 窗口高度
 */
int Game::getWindowHeight(void)
{
    return windowHeight;
}

/**
 * @brief 获取FPS
 * @return int FPS值
 */
int Game::getFPS(void)
{
    return FPS;
}
