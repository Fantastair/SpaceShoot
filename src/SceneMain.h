#ifndef __SCENE_MAIN_H_
#define __SCENE_MAIN_H_

#include "Scene.h"
#include "Object.h"

#include <list>
#include <random>

class Game;

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update(float dt) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event& event) override;

    void keyboardControl(float dt);

    void updateBackground(float dt);
    void renderBackground(void);

    void updatePlayer(float dt);
    void renderPlayer(void);
    void renderPlayerInfo(void);

    void shootPlayer(void);
    void updateProjectilePlayer(float dt);
    void renderProjectilePlayer(void);

    void CreateEnemy(void);
    void updateEnemy(float dt);
    void renderEnemy(void);
    void EnemyExplode(Enemy* enemy);

    void updateProjectileEnemy(float dt);
    void renderProjectileEnemy(void);

    void updateExplosion(float dt);
    void renderExplosion(void);

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    Game& game;
    Background background;
    
    Player player;
    PlayerFire playerFire;
    PlayerHP playerHP;
    bool playerIsDead = false;

    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayerList;

    Enemy enemyTemplate;
    std::list<Enemy*> enemyList;

    ProjectileEnemy projectileEnemyTemplate;
    std::list<ProjectileEnemy*> projectileEnemyList;

    Explosion explosionTemplate;
    std::list<Explosion*> explosionList;
};

float getDistance2(float x1, float y1, float x2, float y2);

#endif
