#ifndef __GAME_H_
#define __GAME_H_

#include <SDL.h>
#include "Scene.h"

class Game
{
public:
    static Game& getInstance(void);
    ~Game(void);

    void run(void);
    void init(void);
    void clean(void);

    void changeScene(Scene* newScene);

    void handleEvents(void);
    void update(float dt);
    void render(void);

    SDL_Window* getWindow(void);
    SDL_Renderer* getRenderer(void);
    int getWindowWidth(void);
    int getWindowHeight(void);
    int getFPS(void);
private:
    Game(void);
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool isRunning = true;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    int FPS = 60;
    Uint32 frameTime;
    float deltaTime;
};

#endif
