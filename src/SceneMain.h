#ifndef __SCENE_MAIN_H_
#define __SCENE_MAIN_H_

#include "Scene.h"
#include "Object.h"

#include <map>
#include <list>
#include <random>

#include <SDL_mixer.h>

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
    void playerActiveShield(void);
    void playerDeactivateShield(void);
    void playerSpeedUp(void);
    void playerSpeedDown(void);

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
    
    void updateItem(float dt);
    void renderItem(void);
    void dropItem(Enemy* enemy);
    void pickupItem(Item* item);

    void updateShield(float dt);
    void renderShield(void);

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    Game& game;
    Background background;

    Mix_Music* bgm = nullptr;
    std::map<std::string, Mix_Chunk*> sounds;

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

    Item itemHealthTemplate;
    Item itemShieldTemplate;
    Item itemFuelTemplate;
    std::list<Item*> itemList;

    Shield playerShield;
};

float getDistance2(float x1, float y1, float x2, float y2);

#endif
