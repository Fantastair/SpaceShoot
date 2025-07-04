#ifndef __SCENE_H_
#define __SCENE_H_

#include <SDL.h>

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(SDL_Event& event) = 0;

private:
};

#endif
