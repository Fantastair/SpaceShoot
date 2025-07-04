#ifndef __OBJECT_H_
#define __OBJECT_H_

#include <SDL.h>

struct Player
{
    SDL_Texture* texture = nullptr;        // 玩家飞机材质
    SDL_FPoint position = {0, 0};          // 玩家飞机位置
    int width = 0;                         // 玩家飞机宽度
    int height = 0;                        // 玩家飞机高度
    int hspeedmax = 250;                   // 玩家水平最大速度，单位像素/秒
    int vspeedmax = 300;                   // 玩家垂直最大速度，单位像素/秒
    int ha = 500;                          // 玩家水平加速度，单位像素/秒^2
    int va = 800;                          // 玩家垂直加速度，单位像素/秒^2
    int hspeed = 0;                        // 玩家水平速度，单位像素/秒
    int vspeed = 0;                        // 玩家垂直速度，单位像素/秒
    int hmove_flag = 0;                    // 玩家水平移动标志，0表示不移动，1表示向左移动，2表示向右移动
    int vmove_flag = 0;                    // 玩家垂直移动标志，0表示不移动，1表示向上移动，2表示向下移动
    Uint32 coolDown = 500;                 // 子弹射击冷却时间，ms
    Uint32 lastShootTime = 0;              // 上次射击时间戳
    int HP = 100;                          // 玩家生命值
    int maxHP = 100;                       // 玩家最大生命值
    int hitDamage = 200;                   // 撞击伤害
    int hitRadius2 = 1600;                 // 撞击半径的平方，单位像素^2
};

struct PlayerFire
{
    SDL_Texture* texture = nullptr;        // 玩家尾焰材质
    int width = 0;                         // 玩家尾焰宽度
    int height = 0;                        // 玩家尾焰高度
};

struct PlayerHP
{
    SDL_Texture* texture = nullptr;    // 玩家生命值材质
    SDL_FPoint position = {16, 16};    // 玩家生命值位置
    int width = 0;                     // 玩家生命值宽度
    int height = 0;                    // 玩家生命值高度
    int BarWidth = 120;                // 玩家生命值条宽度
    int BarHeight = 28;                // 玩家生命值条高度
    int BarBd = 2;                     // 玩家生命值条边框宽度
    SDL_Color BarBgColor = {255, 0, 0, 255};        // 玩家生命值条背景颜色
    SDL_Color BarFgColor = {255, 255, 255, 255};    // 玩家生命值条前景颜色
};

struct Enemy
{
    SDL_Texture* texture = nullptr;    // 敌机材质
    SDL_FPoint position = {0, 0};      // 敌机位置
    int width = 0;                     // 敌机宽度
    int height = 0;                    // 敌机高度
    int speed = 175;                   // 敌机速度，单位像素/秒
    Uint32 coolDown = 1500;            // 子弹射击冷却时间，ms
    Uint32 lastShootTime = 0;          // 上次射击时间戳
    int HP = 100;                      // 敌机生命值
    int maxHP = 100;                   // 敌机最大生命值
    SDL_Color HP_BarBgColor = {0, 255, 0, 255};       // 敌机生命值条背景颜色
    int HP_BarWidth = 0;                // 敌机生命值条宽度
    int HP_BarHeight = 5;                // 敌机生命值条高度
    int HP_BarMargin = 5;                // 敌机生命值条与敌机之间的间距
};

struct ProjectilePlayer
{
    SDL_Texture* texture = nullptr;    // 玩家子弹材质
    SDL_FPoint position = {0, 0};      // 玩家子弹位置
    int width = 0;                     // 玩家子弹宽度
    int height = 0;                    // 玩家子弹高度
    int speed = 600;                   // 玩家子弹速度，单位像素/秒
    int damage = 50;                   // 玩家子弹伤害值
    int damageRadius2 = 400;           // 玩家子弹伤害半径的平方，单位像素^2
};

struct ProjectileEnemy
{
    SDL_Texture* texture = nullptr;    // 敌机子弹材质
    SDL_FPoint position = {0, 0};      // 敌机子弹位置
    SDL_FPoint direction = {0, 0};     // 敌机子弹方向
    int width = 0;                     // 敌机子弹宽度
    int height = 0;                    // 敌机子弹高度
    int speed = 400;                   // 敌机子弹速度，单位像素/秒
    int damage = 10;                   // 敌机子弹伤害值
    int damageRadius2 = 400;           // 敌机子弹伤害半径的平方，单位像素^2
};

struct Explosion
{
    SDL_Texture* texture = nullptr;    // 爆炸材质
    SDL_FPoint position = {0, 0};      // 爆炸位置
    int width = 0;                     // 爆炸宽度
    int height = 0;                    // 爆炸高度
    int currentFrame = 0;              // 当前爆炸帧
    int totalFrame = 0;                // 总爆炸帧数
    Uint32 startTime = 0;              // 爆炸开始时间戳
    Uint32 FPS = 10;                   // 爆炸帧率，单位帧/秒
};

struct Background
{
    SDL_Texture* texture = nullptr;        // 背景材质
    SDL_FPoint position = {0, 0};          // 背景位置
    int width = 0;                         // 背景宽度
    int height = 0;                        // 背景高度
    SDL_Texture* decoration1 = nullptr;    // 背景装饰材质1
    SDL_Texture* decoration2 = nullptr;    // 背景装饰材质2
    int decoration1Width = 0;              // 背景装饰1宽度
    int decoration1Height = 0;             // 背景装饰1高度
    int decoration2Width = 0;              // 背景装饰2宽度
    int decoration2Height = 0;             // 背景装饰2高度
    float decoration1Offset = 0;             // 背景装饰1偏移量
    float decoration2Offset = 0;             // 背景装饰2偏移量
    int decoration1Speed = 50;            // 背景装饰1滚动速度，单位像素/秒
    int decoration2Speed = 25;             // 背景装饰2滚动速度，单位像素/秒
};

#endif
