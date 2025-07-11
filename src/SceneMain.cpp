#include "SceneMain.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance()) {}

SceneMain::~SceneMain() {}

/**
 * @brief 初始化场景
 */
void SceneMain::init()
{
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    bgm = Mix_LoadMUS("./assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background music: %s", Mix_GetError());
        return;
    }
    if (Mix_PlayMusic(bgm, -1) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to play background music: %s", Mix_GetError());
        return;
    }

    sounds["player_shoot"] = Mix_LoadWAV("./assets/sound/laser_shoot4.wav");
    sounds["enemy_shoot"] = Mix_LoadWAV("./assets/sound/xs_laser.wav");
    sounds["player_explode"] = Mix_LoadWAV("./assets/sound/explosion1.wav");
    sounds["enemy_explode"] = Mix_LoadWAV("./assets/sound/explosion3.wav");
    sounds["hit"] = Mix_LoadWAV("./assets/sound/eff11.wav");
    sounds["get_item"] = Mix_LoadWAV("./assets/sound/eff5.wav");

    background.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bg.png");
    if (background.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(background.texture, nullptr, nullptr, &background.width, &background.height);
    background.width = game.getWindowWidth();
    background.height = game.getWindowHeight();
    background.decoration1 = IMG_LoadTexture(game.getRenderer(), "./assets/image/Stars-A.png");
    if (background.decoration1 == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background decoration1 texture: %s", IMG_GetError());
        return;
    }
    background.decoration2 = IMG_LoadTexture(game.getRenderer(), "./assets/image/Stars-B.png");
    if (background.decoration2 == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background decoration2 texture: %s", IMG_GetError());
        return;
    }
    background.decoration1Width = background.decoration1Height = background.decoration2Width = background.decoration2Height = background.width;
    background.decoration1Offset = background.decoration2Offset = 0;

    player.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position = {(game.getWindowWidth() - player.width) / 2.0f, game.getWindowHeight() - player.height - 0.0f};
    player.hspeedmax = player.original_hspeedmax;
    player.vspeedmax = player.original_vspeedmax;
    player.ha = player.original_ha;
    player.va = player.original_va;
    player.coolDown = player.original_coolDown;

    playerFire.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/fire.png");
    if (playerFire.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player fire texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(playerFire.texture, nullptr, nullptr, &playerFire.width, &playerFire.height);
    playerFire.width /= 4;
    playerFire.height /= 4;

    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bullet.png");
    if (projectilePlayerTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load projectilePlayerTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/insect-2.png");
    if (enemyTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemyTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;
    enemyTemplate.HP_BarWidth = static_cast<int>(enemyTemplate.width * 1.2f);

    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bullet-1.png");
    if (projectileEnemyTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load projectileEnemyTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 3;
    projectileEnemyTemplate.height /= 3;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/effect/explosion.png");
    if (explosionTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosionTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;

    playerHP.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/Health UI Black.png");
    if (playerHP.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load playerHP texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(playerHP.texture, nullptr, nullptr, &playerHP.width, &playerHP.height);

    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bonus_health.png");
    if (itemHealthTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemHealthTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(itemHealthTemplate.texture, nullptr, nullptr, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.width /= 3;
    itemHealthTemplate.height /= 3;
    itemHealthTemplate.type = ItemType::Health;

    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bonus_shield.png");
    if (itemShieldTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemShieldTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(itemShieldTemplate.texture, nullptr, nullptr, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.width /= 3;
    itemShieldTemplate.height /= 3;
    itemShieldTemplate.type = ItemType::Shield;

    itemFuelTemplate.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/bonus_fuel.png");
    if (itemFuelTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemFuelTemplate texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(itemFuelTemplate.texture, nullptr, nullptr, &itemFuelTemplate.width, &itemFuelTemplate.height);
    itemFuelTemplate.width /= 3;
    itemFuelTemplate.height /= 3;
    itemFuelTemplate.type = ItemType::Fuel;

    playerShield.texture = IMG_LoadTexture(game.getRenderer(), "./assets/image/shield.png");
    if (playerShield.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load playerShield texture: %s", IMG_GetError());
        return;
    }
    SDL_QueryTexture(playerShield.texture, nullptr, nullptr, &playerShield.width, &playerShield.height);
    playerShield.width /= 4;
    playerShield.height /= 4;
}

/**
 * @brief 更新场景
 */
void SceneMain::update(float dt)
{
    updateBackground(dt);
    keyboardControl(dt);
    updateProjectilePlayer(dt);
    updateProjectileEnemy(dt);
    CreateEnemy();
    updatePlayer(dt);
    updateEnemy(dt);
    updateExplosion(dt);
    updateItem(dt);
    updateShield(dt);
}

/**
 * @brief 渲染场景
 */
void SceneMain::render()
{
    renderBackground();
    renderExplosion();
    renderProjectileEnemy();
    renderProjectilePlayer();
    renderItem();
    renderEnemy();
    renderPlayer();
    renderShield();
    renderPlayerInfo();
}

/**
 * @brief 清理场景资源
 */
void SceneMain::clean()
{
    if (background.texture != nullptr)
    {
        SDL_DestroyTexture(background.texture);
        background.texture = nullptr;
    }
    if (background.decoration1 != nullptr)
    {
        SDL_DestroyTexture(background.decoration1);
        background.decoration1 = nullptr;
    }
    if (background.decoration2 != nullptr)
    {
        SDL_DestroyTexture(background.decoration2);
        background.decoration2 = nullptr;
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if (playerFire.texture != nullptr)
    {
        SDL_DestroyTexture(playerFire.texture);
        playerFire.texture = nullptr;
    }
    if (playerHP.texture != nullptr)
    {
        SDL_DestroyTexture(playerHP.texture);
        playerHP.texture = nullptr;
    }

    for (auto projectile : projectilePlayerList)
    {
        if (projectile != nullptr)
        {
            projectile->texture = nullptr;
            delete projectile;
        }
    }
    projectilePlayerList.clear();
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
        projectilePlayerTemplate.texture = nullptr;
    }

    for (auto enemy : enemyList)
    {
        if (enemy != nullptr)
        {
            enemy->texture = nullptr;
            delete enemy;
        }
    }
    enemyList.clear();
    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
        enemyTemplate.texture = nullptr;
    }

    for (auto projectile : projectileEnemyList)
    {
        if (projectile != nullptr)
        {
            projectile->texture = nullptr;
            delete projectile;
        }
    }
    projectileEnemyList.clear();
    if (projectileEnemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
        projectileEnemyTemplate.texture = nullptr;
    }

    for (auto explosion : explosionList)
    {
        if (explosion != nullptr)
        {
            explosion->texture = nullptr;
            delete explosion;
        }
    }
    explosionList.clear();
    if (explosionTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(explosionTemplate.texture);
        explosionTemplate.texture = nullptr;
    }
    for (auto item : itemList)
    {
        if (item != nullptr)
        {
            item->texture = nullptr;
            delete item;
        }
    }
    itemList.clear();
    if (itemHealthTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(itemHealthTemplate.texture);
        itemHealthTemplate.texture = nullptr;
    }
    if (itemShieldTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(itemShieldTemplate.texture);
        itemShieldTemplate.texture = nullptr;
    }
    if (itemFuelTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(itemFuelTemplate.texture);
        itemFuelTemplate.texture = nullptr;
    }
    if (playerShield.texture != nullptr)
    {
        SDL_DestroyTexture(playerShield.texture);
        playerShield.texture = nullptr;
    }

    for (auto sound : sounds)
    {
        if (sound.second != nullptr)
        {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();

    if (bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

/**
 * @brief 处理事件
 * @param event SDL事件
 */
void SceneMain::handleEvents(SDL_Event&) {}

/**
 * @brief 键盘控制
 */
void SceneMain::keyboardControl(float dt)
{
    if (playerIsDead) { return; }
    auto keyboardState = SDL_GetKeyboardState(NULL);

    if (player.vmove_flag == 0)
    {
        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
        {
            if (!keyboardState[SDL_SCANCODE_S] && !keyboardState[SDL_SCANCODE_DOWN])
            {
                player.vmove_flag = 1;
            }
        }
        else if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
        {
            if (!keyboardState[SDL_SCANCODE_W] && !keyboardState[SDL_SCANCODE_UP])
            {
                player.vmove_flag = 2;
            }
        }
    }
    else if (player.vmove_flag == 1)
    {
        if (!keyboardState[SDL_SCANCODE_W] && !keyboardState[SDL_SCANCODE_UP])
        {
            if (!keyboardState[SDL_SCANCODE_S] && !keyboardState[SDL_SCANCODE_DOWN])
            {
                player.vmove_flag = 0;
            }
            else
            {
                player.vmove_flag = 2;
            }
            
        }

    }
    else if (player.vmove_flag == 2)
    {
        if (!keyboardState[SDL_SCANCODE_S] && !keyboardState[SDL_SCANCODE_DOWN])
        {
            if (!keyboardState[SDL_SCANCODE_W] && !keyboardState[SDL_SCANCODE_UP])
            {
                player.vmove_flag = 0;
            }
            else
            {
                player.vmove_flag = 1;
            }
        }
    }

    if (player.hmove_flag == 0)
    {
        if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
        {
            if (!keyboardState[SDL_SCANCODE_D] && !keyboardState[SDL_SCANCODE_RIGHT])
            {
                player.hmove_flag = 1;
            }
        }
        else if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
        {
            if (!keyboardState[SDL_SCANCODE_A] && !keyboardState[SDL_SCANCODE_LEFT])
            {
                player.hmove_flag = 2;
            }
        }
    }
    else if (player.hmove_flag == 1)
    {
        if (!keyboardState[SDL_SCANCODE_A] && !keyboardState[SDL_SCANCODE_LEFT])
        {
            if (!keyboardState[SDL_SCANCODE_D] && !keyboardState[SDL_SCANCODE_RIGHT])
            {
                player.hmove_flag = 0;
            }
            else
            {
                player.hmove_flag = 2;
            }
            
        }

    }
    else if (player.hmove_flag == 2)
    {
        if (!keyboardState[SDL_SCANCODE_D] && !keyboardState[SDL_SCANCODE_RIGHT])
        {
            if (!keyboardState[SDL_SCANCODE_A] && !keyboardState[SDL_SCANCODE_LEFT])
            {
                player.hmove_flag = 0;
            }
            else
            {
                player.hmove_flag = 1;
            }
        }
    }

    if (player.vmove_flag == 1)
    {
        player.vspeed -= static_cast<int>(player.va * dt);
        if (player.vspeed < -player.vspeedmax) { player.vspeed = -player.vspeedmax; }
        player.position.y += dt * player.vspeed;
    }
    else if (player.vmove_flag == 2)
    {
        player.vspeed += static_cast<int>(player.va * dt);
        if (player.vspeed > player.vspeedmax) { player.vspeed = player.vspeedmax; }
        player.position.y += dt * player.vspeed;
    }
    else if (player.vmove_flag == 0)
    {
        if (player.vspeed > 0)
        {
            player.vspeed -= static_cast<int>(player.va * dt);
            if (player.vspeed < 0) { player.vspeed = 0; }
        }
        else if (player.vspeed < 0)
        {
            player.vspeed += static_cast<int>(player.va * dt);
            if (player.vspeed > 0) { player.vspeed = 0; }
        }
        player.position.y += dt * player.vspeed;
    }

    if (player.hmove_flag == 1)
    {
        player.hspeed -= static_cast<int>(player.ha * dt);
        if (player.hspeed < -player.hspeedmax) { player.hspeed = -player.hspeedmax; }
        player.position.x += dt * player.hspeed;
    }
    else if (player.hmove_flag == 2)
    {
        player.hspeed += static_cast<int>(player.ha * dt);
        if (player.hspeed > player.hspeedmax) { player.hspeed = player.hspeedmax; }
        player.position.x += dt * player.hspeed;
    }
    else if (player.hmove_flag == 0)
    {
        if (player.hspeed > 0)
        {
            player.hspeed -= static_cast<int>(player.ha * dt);
            if (player.hspeed < 0) { player.hspeed = 0; }
        }
        else if (player.hspeed < 0)
        {
            player.hspeed += static_cast<int>(player.ha * dt);
            if (player.hspeed > 0) { player.hspeed = 0; }
        }
        player.position.x += dt * player.hspeed;
    }

    if (player.position.x < 0) { player.position.x = 0; }
    else if (player.position.x > game.getWindowWidth() - player.width) { player.position.x = static_cast<float>(game.getWindowWidth() - player.width); }
    if (player.position.y < 0) { player.position.y = 0; }
    else if (player.position.y > game.getWindowHeight() - player.height) { player.position.y = static_cast<float>(game.getWindowHeight() - player.height); }

    if (keyboardState[SDL_SCANCODE_J])
    {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime >= player.coolDown)
        {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }
}

/**
 * @brief 更新背景
 * @param dt 时间增量
 */
void SceneMain::updateBackground(float dt)
{
    background.decoration1Offset += dt * background.decoration1Speed;
    if (background.decoration1Offset >= background.decoration1Height)
    {
        background.decoration1Offset = 0;
    }

    background.decoration2Offset += dt * background.decoration2Speed;
    if (background.decoration2Offset >= background.decoration2Height)
    {
        background.decoration2Offset = 0;
    }
}

/**
 * @brief 渲染背景
 */
void SceneMain::renderBackground(void)
{
    SDL_Rect bgRect = {0, 0, game.getWindowWidth(), game.getWindowHeight()};
    SDL_RenderCopy(game.getRenderer(), background.texture, NULL, &bgRect);

    SDL_Rect decoration1Rect = {0, static_cast<int>(background.decoration1Offset), background.decoration1Width, background.decoration1Height};
    SDL_RenderCopy(game.getRenderer(), background.decoration1, NULL, &decoration1Rect);
    if (background.decoration1Offset > 0)
    {
        decoration1Rect.y = static_cast<int>(background.decoration1Offset - background.decoration1Height);
        SDL_RenderCopy(game.getRenderer(), background.decoration1, NULL, &decoration1Rect);
    }
    if (background.decoration1Offset < background.height - background.decoration1Height)
    {
        decoration1Rect.y = static_cast<int>(background.decoration1Offset + background.decoration1Height);
        SDL_RenderCopy(game.getRenderer(), background.decoration1, NULL, &decoration1Rect);
    }

    SDL_Rect decoration2Rect = {0, static_cast<int>(background.decoration2Offset), background.decoration2Width, background.decoration2Height};
    SDL_RenderCopy(game.getRenderer(), background.decoration2, NULL, &decoration2Rect);
    if (background.decoration2Offset > 0)
    {
        decoration2Rect.y = static_cast<int>(background.decoration2Offset - background.decoration2Height);
        SDL_RenderCopy(game.getRenderer(), background.decoration2, NULL, &decoration2Rect);
    }
    if (background.decoration2Offset < background.height - background.decoration2Height)
    {
        decoration2Rect.y = static_cast<int>(background.decoration2Offset + background.decoration2Height);
        SDL_RenderCopy(game.getRenderer(), background.decoration2, NULL, &decoration2Rect);
    }
}

/**
 * @brief 更新玩家
 * @param dt 时间增量
 */
void SceneMain::updatePlayer(float)
{
    // playerIsDead = false;
    // player.HP = player.maxHP;

    if (player.HP > playerHP.showHP)
    {
        playerHP.showHP++;
    }
    else if (player.HP < playerHP.showHP)
    {
        playerHP.showHP--;
    }

    if (playerIsDead) { return; }

    if (player.HP <= 0)
    {
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + (player.width - explosion->width) / 2.0f;
        explosion->position.y = player.position.y + (player.height - explosion->height) / 2.0f;
        explosion->startTime = SDL_GetTicks();
        explosionList.push_back(explosion);
        playerIsDead = true;
        player.HP = 0;
        Mix_PlayChannel(-1, sounds["player_explode"], 0);
        return;
    }

    if (player.isSpeedingUp)
    {
        if (SDL_GetTicks() - player.startSpeedUpTime >= player.totalSpeedUpTime)
        {
            playerSpeedDown();
        }
    }

    for (auto enemy : enemyList)
    {
        if (getDistance2(player.position.x + player.width / 2.0f, player.position.y + player.height / 2.0f,
            enemy->position.x + enemy->width / 2.0f, enemy->position.y + enemy->height / 2.0f) <= player.hitRadius2)
        {
            if (player.shieldActive)
            {
                playerShield.value -= enemy->HP / 2;
            }
            else
            {
                player.HP -= enemy->HP / 2;
            }
            enemy->HP -= player.hitDamage;
            Mix_PlayChannel(-1, sounds["hit"], 0);
        }
    }
}

/**
 * @brief 渲染玩家
 */
void SceneMain::renderPlayer(void)
{
    if (playerIsDead) { return; }
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    if (player.vmove_flag == 1)
    {
        SDL_Rect playerFireRect = {static_cast<int>(player.position.x + (player.width - playerFire.width) / 2.0f), static_cast<int>(player.position.y + player.height - playerFire.height / 2.0f), playerFire.width, playerFire.height};
        SDL_RenderCopy(game.getRenderer(), playerFire.texture, NULL, &playerFireRect);
    }
}

/**
 * @brief 渲染玩家信息
 */
void SceneMain::renderPlayerInfo(void)
{
    SDL_Rect playerHPRect = {static_cast<int>(playerHP.position.x - playerHP.width / 4.0f), static_cast<int>(playerHP.position.y), playerHP.width, playerHP.height};
    SDL_RenderCopy(game.getRenderer(), playerHP.texture, NULL, &playerHPRect);
    SDL_SetRenderDrawColor(game.getRenderer(), playerHP.BarFgColor.r, playerHP.BarFgColor.g, playerHP.BarFgColor.b, playerHP.BarFgColor.a);
    SDL_Rect hpBarRect;
    hpBarRect = {static_cast<int>(playerHP.position.x + playerHP.width - playerHP.BarBd), static_cast<int>(playerHP.position.y - playerHP.BarBd), playerHP.BarWidth + playerHP.BarBd * 2, playerHP.BarBd};
    SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    hpBarRect = {static_cast<int>(playerHP.position.x + playerHP.width - playerHP.BarBd), static_cast<int>(playerHP.position.y + playerHP.BarHeight), playerHP.BarWidth + playerHP.BarBd * 2, playerHP.BarBd};
    SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    hpBarRect = {static_cast<int>(playerHP.position.x + playerHP.width - playerHP.BarBd), static_cast<int>(playerHP.position.y), playerHP.BarBd, playerHP.BarHeight};
    SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    hpBarRect = {static_cast<int>(playerHP.position.x + playerHP.width + playerHP.BarWidth), static_cast<int>(playerHP.position.y), playerHP.BarBd, playerHP.BarHeight};
    SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    SDL_SetRenderDrawColor(game.getRenderer(), playerHP.BarBgColor.r, playerHP.BarBgColor.g, playerHP.BarBgColor.b, playerHP.BarBgColor.a);
    hpBarRect = {static_cast<int>(playerHP.position.x + playerHP.width), static_cast<int>(playerHP.position.y), playerHP.BarWidth * playerHP.showHP / player.maxHP, playerHP.BarHeight};
    SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    if (player.shieldActive)
    {
        hpBarRect = { static_cast<int>(playerHP.position.x + playerHP.width + playerHP.BarWidth), static_cast<int>(playerHP.position.y), static_cast<int>(playerHP.BarWidth * playerShield.showedValue / playerShield.maxValue), playerHP.BarHeight };
        SDL_SetRenderDrawColor(game.getRenderer(), playerShield.barBg.r, playerShield.barBg.g, playerShield.barBg.b, playerShield.barBg.a);
        SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    }
    if (player.isSpeedingUp)
    {
        int width = static_cast<int>(playerHP.BarWidth * (1 - (SDL_GetTicks() - player.startSpeedUpTime) / static_cast<float>(player.totalSpeedUpTime)));
        if (width > playerHP.BarWidth) { width = playerHP.BarWidth; }
        else if (width < 0) { width = 0; }
        if (player.showedSpeedupBarWidth < width) { player.showedSpeedupBarWidth += 4; }
        else if (player.showedSpeedupBarWidth > width) { player.showedSpeedupBarWidth--; }
        hpBarRect = { static_cast<int>(playerHP.position.x + playerHP.width), static_cast<int>(playerHP.position.y + playerHP.BarHeight + playerHP.BarBd), player.showedSpeedupBarWidth, playerHP.BarHeight / 4 };
        SDL_SetRenderDrawColor(game.getRenderer(), player.speedColor.r, player.speedColor.g, player.speedColor.b, player.speedColor.a);
        SDL_RenderFillRect(game.getRenderer(), &hpBarRect);
    }
    SDL_SetRenderDrawColor(game.getRenderer(), 0, 0, 0, 255);
}

/**
 * @brief 玩家激活护盾
 */
void SceneMain::playerActiveShield(void)
{
    player.shieldActive = true;
    playerShield.value = playerShield.maxValue;
}

/**
 * @brief 玩家护盾值关闭
 */
void SceneMain::playerDeactivateShield(void)
{
    player.shieldActive = false;
    playerShield.value = playerShield.showedValue = 0;
}

/**
 * @brief 玩家加速
 */
void SceneMain::playerSpeedUp(void)
{
    player.isSpeedingUp = true;
    player.startSpeedUpTime = SDL_GetTicks();
    player.hspeedmax = player.speedUp_hspeedmax;
    player.vspeedmax = player.speedUp_vspeedmax;
    player.ha = player.speedUp_ha;
    player.va = player.speedUp_va;
    player.coolDown = player.speedUp_coolDown;
}

/**
 * @brief 玩家减速
 */
void SceneMain::playerSpeedDown(void)
{
    player.isSpeedingUp = false;
    player.hspeedmax = player.original_hspeedmax;
    player.vspeedmax = player.original_vspeedmax;
    player.ha = player.original_ha;
    player.va = player.original_va;
    player.coolDown = player.original_coolDown;
}

/**
 * @brief 发射子弹
 */
void SceneMain::shootPlayer(void)
{
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + (player.width - projectile->width) / 2.0f;
    projectile->position.y = player.position.y;
    projectile->damage = static_cast<int>(projectile->damage * 0.8f + dis(gen) * 0.4f);
    projectilePlayerList.push_back(projectile);
    Mix_PlayChannel(-1, sounds["player_shoot"], 0);
}

/**
 * @brief 更新玩家子弹
 * @param dt 时间增量
 */
void SceneMain::updateProjectilePlayer(float dt)
{
    for (auto it = projectilePlayerList.begin(); it != projectilePlayerList.end();)
    {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * dt;
    

        if (projectile->position.y < -projectile->height)
        {
            delete projectile;
            it = projectilePlayerList.erase(it);
        }
        else
        {
            bool hit = false;
            for (auto enemy : enemyList)
            {
                if (getDistance2(projectile->position.x + projectile->width / 2.0f, projectile->position.y + projectile->height / 2.0f,
                    enemy->position.x + enemy->width / 2.0f, enemy->position.y + enemy->height / 2.0f) <= projectile->damageRadius2)
                {
                    enemy->HP -= projectile->damage;
                    delete projectile;
                    it = projectilePlayerList.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit) { ++it; }
        }
    }
}

/**
 * @brief 渲染玩家子弹
 */
void SceneMain::renderProjectilePlayer(void)
{
    for (const auto& projectile : projectilePlayerList)
    {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

/**
 * @brief 创建敌机
 */
void SceneMain::CreateEnemy(void)
{
    if (dis(gen) > 0.65f / game.getFPS()) { return; }
    auto enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = static_cast<float>(-enemy->height);
    enemy->speed = static_cast<int>(enemy->speed * (0.5f + dis(gen)));
    enemy->lastShootTime = static_cast<Uint32>(SDL_GetTicks() + dis(gen) * enemy->coolDown);
    enemyList.push_back(enemy);
}

/**
 * @brief 更新敌机
 * @param dt 时间增量
 */
void SceneMain::updateEnemy(float dt)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemyList.begin(); it != enemyList.end();)
    {
        auto enemy = *it;
        enemy->position.y += enemy->speed * dt;

        if (enemy->position.y > game.getWindowHeight())
        {
            delete enemy;
            it = enemyList.erase(it);
        }
        else
        {
            if (!playerIsDead && enemy->HP > enemy->maxHP * 0.2f && enemy->position.y + enemy->height / 2.0f < player.position.y + player.height / 2.0f && currentTime > enemy->lastShootTime && currentTime - enemy->lastShootTime >= enemy->coolDown)
            {
                auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
                projectile->position.x = enemy->position.x + (enemy->width - projectile->width) / 2.0f;
                projectile->position.y = enemy->position.y + enemy->height - projectile->height;
                projectile->direction.x = player.position.x + player.width / 2.0f - projectile->position.x - projectile->width / 2.0f;
                projectile->direction.y = player.position.y + player.height / 2.0f - projectile->position.y - projectile->height / 2.0f;
                float length = std::sqrt(projectile->direction.x * projectile->direction.x + projectile->direction.y * projectile->direction.y);
                if (length > 0.0f)
                {
                    projectile->direction.x /= length;
                    projectile->direction.y /= length;
                }
                enemy->lastShootTime = currentTime;
                projectileEnemyList.push_back(projectile);
                Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
            }
            if (enemy->HP <= 0)
            {
                EnemyExplode(enemy);
                it = enemyList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

/**
 * @brief 渲染敌机
 */
void SceneMain::renderEnemy(void)
{
    for (const auto& enemy : enemyList)
    {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);

        SDL_SetRenderDrawColor(game.getRenderer(), enemy->HP_BarBgColor.r, enemy->HP_BarBgColor.g, enemy->HP_BarBgColor.b, enemy->HP_BarBgColor.a);
        enemyRect = {static_cast<int>(enemy->position.x + (enemy->width - enemy->HP_BarWidth) / 2.0f), static_cast<int>(enemy->position.y - enemy->HP_BarHeight - enemy->HP_BarMargin),
                                   static_cast<int>((enemy->HP_BarWidth * (static_cast<float>(enemy->HP) / 100.0f)) * enemy->HP / enemy->maxHP), enemy->HP_BarHeight};
        SDL_RenderFillRect(game.getRenderer(), &enemyRect);
    }
    SDL_SetRenderDrawColor(game.getRenderer(), 0, 0, 0, 255);
}

/**
 * @brief 敌机爆炸处理
 * @param enemy 被销毁的敌机
 */
void SceneMain::EnemyExplode(Enemy *enemy)
{
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + (enemy->width - explosion->width) / 2.0f;
    explosion->position.y = enemy->position.y + (enemy->height - explosion->height) / 2.0f;
    explosion->startTime = SDL_GetTicks();
    explosionList.push_back(explosion);
    dropItem(enemy);
    Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
    delete enemy;
}

/**
 * @brief 更新敌机子弹
 * @param dt 时间增量
 */
void SceneMain::updateProjectileEnemy(float dt)
{
    for (auto it = projectileEnemyList.begin(); it != projectileEnemyList.end();)
    {
        auto projectile = *it;
        projectile->position.x += projectile->direction.x * projectile->speed * dt;
        projectile->position.y += projectile->direction.y * projectile->speed * dt;

        if (projectile->position.y < 0 || projectile->position.y > game.getWindowHeight() || projectile->position.x < 0 || projectile->position.x > game.getWindowWidth())
        {
            delete projectile;
            it = projectileEnemyList.erase(it);
        }
        else
        {
            if (!playerIsDead && getDistance2(projectile->position.x + projectile->width / 2.0f, projectile->position.y + projectile->height / 2.0f,
                player.position.x + player.width / 2.0f, player.position.y + player.height / 2.0f) <= projectile->damageRadius2)
            {
                if (player.shieldActive)
                {
                    playerShield.value -= projectile->damage;
                }
                else
                {
                    player.HP -= projectile->damage;
                }
                delete projectile;
                it = projectileEnemyList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

/**
 * @brief 渲染敌机子弹
 */
void SceneMain::renderProjectileEnemy(void)
{
    for (const auto& projectile : projectileEnemyList)
    {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        double angle = std::atan2(projectile->direction.y, projectile->direction.x) * 180.0 / M_PI - 90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

/**
 * @brief 更新爆炸效果
 * @param dt 时间增量
 */
void SceneMain::updateExplosion(float)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosionList.begin(); it != explosionList.end();)
    {
        auto explosion = *it;
        explosion->currentFrame = static_cast<int>((currentTime - explosion->startTime) * explosion->FPS / 1000.0f);
        if (explosion->currentFrame >= explosion->totalFrame)
        {
            delete explosion;
            it = explosionList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/**
 * @brief 渲染爆炸效果
 */
void SceneMain::renderExplosion(void)
{
    for (const auto& explosion : explosionList)
    {
        SDL_Rect src = {explosion->currentFrame * explosion->height, 0, explosion->height, explosion->height};
        SDL_Rect dst = {static_cast<int>(explosion->position.x), static_cast<int>(explosion->position.y), explosion->width, explosion->height};
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);
    }
}

/**
 * @brief 更新掉落物品
 * @param dt 时间增量
 */
void SceneMain::updateItem(float dt)
{
    for (auto it = itemList.begin(); it != itemList.end();)
    {
        auto item = *it;
        if (item->state == 0)
        {
            if (SDL_GetTicks() - item->startExistTime > item->totalExistTime)
            {
                delete item;
                it = itemList.erase(it);
                continue;
            }

            item->position.x += item->direction.x * item->speed * dt;
            item->position.y += item->direction.y * item->speed * dt;

            if (getDistance2(item->position.x + item->width / 2.0f, item->position.y + item->height / 2.0f,
                player.position.x + player.width / 2.0f, player.position.y + player.height / 2.0f) <= item->pickupRadius2)
            {
                item->state = 1;
                item->startPickupTime = SDL_GetTicks();
                item->direction.x = item->position.x + item->width / 2.0f;
                item->direction.y = item->position.y + item->height / 2.0f;
                it++;
                continue;
            }

            if (item->position.x < 0 || item->position.x > game.getWindowWidth() - item->width)
            {
                item->direction.x *= -1;
            }
            if (item->position.y < 0 || item->position.y > game.getWindowHeight() - item->height)
            {
                item->direction.y *= -1;
            }
        }
        else if (item->state == 1)
        {
            float process = (SDL_GetTicks() - item->startPickupTime) / static_cast<float>(item->totalPickupTime);
            if (process >= 1.0f)
            {
                pickupItem(item);
                delete item;
                it = itemList.erase(it);
                continue;
            }
            item->position.x = item->direction.x + process * (player.position.x + player.width / 2.0f - item->direction.x) - item->width / 2.0f;
            item->position.y = item->direction.y + process * (player.position.y + player.height / 2.0f - item->direction.y) - item->height / 2.0f;
        }
        it++;
    }
}

/**
 * @brief 渲染掉落物品
 */
void SceneMain::renderItem(void)
{
    for (const auto& item : itemList)
    {
        SDL_Rect itemRect = {static_cast<int>(item->position.x), static_cast<int>(item->position.y), item->width, item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }
}

/**
 * @brief 计算两点之间的平方距离
 * @param x1 第一个点的x坐标
 * @param y1 第一个点的y坐标
 * @param x2 第二个点的x坐标
 * @param y2 第二个点的y坐标
 * @return float 两点之间的平方距离
 */
float getDistance2(float x1, float y1, float x2, float y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

/**
 * @brief 掉落物品
 * @param enemy 被销毁的敌机
 */
void SceneMain::dropItem(Enemy* enemy)
{
    if (dis(gen) < 0.4f)
    {
        Item* item;
        auto n = dis(gen);
        if (n < 0.5f)
        {
            item = new Item(itemHealthTemplate);
        }
        else if (n < 0.8f)
        {
            item = new Item(itemShieldTemplate);
        }
        else
        {
            item = new Item(itemFuelTemplate);
        }
        item->position.x = enemy->position.x + (enemy->width - item->width) / 2.0f;
        item->position.y = enemy->position.y + (enemy->height - item->height) / 2.0f;
        itemList.push_back(item);
        n = dis(gen);
        if (n < 0.25f)
        {
            item->direction.x = 1.0f;
            item->direction.y = -1.0f;
        }
        else if (n < 0.5f)
        {
            item->direction.x = -1.0f;
            item->direction.y = -1.0f;
        }
        else if (n < 0.75f)
        {
            item->direction.x = -1.0f;
            item->direction.y = 1.0f;
        }
        else
        {
            item->direction.x = 1.0f;
            item->direction.y = 1.0f;
        }
        item->startExistTime = SDL_GetTicks();
    }
}

/**
 * @brief 拾取物品
 * @param item 被拾取的物品
 */
void SceneMain::pickupItem(Item *item)
{
    if (item->type == ItemType::Health)
    {
        player.HP += 20;
        if (player.HP > player.maxHP) { player.HP = player.maxHP; }
    }
    else if (item->type == ItemType::Shield)
    {
        playerActiveShield();
    }
    else if (item->type == ItemType::Fuel)
    {
        playerSpeedUp();
    }
    Mix_PlayChannel(-1, sounds["get_item"], 0);
}

/**
 * @brief 更新护盾状态
 * @param dt 时间增量
 */
void SceneMain::updateShield(float dt)
{
    if (player.shieldActive)
    {
        playerShield.value -= playerShield.loseRate * dt;
        if (playerShield.value <= 0)
        {
            player.shieldActive = false;
            return;
        }

        if (playerShield.showedValue < playerShield.value)
        {
            playerShield.showedValue++;
        }
        else if (playerShield.showedValue > playerShield.value)
        {
            playerShield.showedValue--;
        }
    }
}

/**
 * @brief 渲染护盾
 */
void SceneMain::renderShield(void)
{
    if (player.shieldActive)
    {
        SDL_Rect shieldRect = { static_cast<int>(player.position.x + (player.width - playerShield.width) / 2.0f), static_cast<int>(player.position.y - playerShield.height), playerShield.width, playerShield.height };
        SDL_RenderCopy(game.getRenderer(), playerShield.texture, NULL, &shieldRect);
    }
}
