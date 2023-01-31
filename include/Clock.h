/**
 * @file Clock.h
 * @author João Paulo Vaz Mendes (170002934@aluno.unb.br)
 * @brief Clock class header file
 * @version 0.1
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef CLOCK_HEADER
#define SPRITE_HEADER
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"
#include "Component.h"
#include "Text.h"
#include <string>

class Clock : public Component
{
private:
    /* data */
    float time;
    int hours;
    int minutes;
    std::string clockTime;
    Text *textClock;

public:
    Clock(GameObject &associated, int initialHour = 0, int initialMinute = 0);
    ~Clock();

    std::string type = "Clock";

    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void Restart();
    std::string GetClock();
    int GetHours();
    int GetMinutes();
    void WriteClock();
};

#endif