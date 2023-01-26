#include "Clock.h"
#include "Text.h"
#include "GameData.h"
#include <iostream>

Clock::Clock(GameObject &associated, int initialHour, int initialMinute) : Component(associated) {
    time = 0;
    hours = initialHour;
    minutes = initialMinute;

    WriteClock();

    textClock = new Text(associated, "assets/font/five.ttf", 40, Text::SOLID, (clockTime),{255, 255, 255, SDL_ALPHA_OPAQUE}) ;
    associated.AddComponent(textClock);
}


Clock::~Clock() {

}

void Clock::Update(float dt){

    time += dt;

    if (time >= 1)
    {
        if (minutes < 59)
        {
            minutes++;
            //std::cout << "minute: " << minutes << std::endl;
        }
        else
        {
            minutes = 0;
            if (hours < 23)
            {
                hours++;
            }
            else
            {
                std::cout << "Passou o dia" << std::endl;
                hours = 0;
                GameData::currentDay++;
            }
        }

        time = 0;
        WriteClock();
        GameData::currentMinute = minutes;
        GameData::currentHour = hours;

        textClock->SetText(clockTime);

    }
}

void Clock::Restart(){

    time = 0;
    hours = 0;
    minutes = 0;

    clockTime = "00:00";
}

std::string Clock::GetClock() {
    return clockTime;
}

int Clock::GetHours(){
    return hours;
}

int Clock::GetMinutes(){
    return minutes;
}

void Clock::WriteClock() {
    std::string hourString;
    std::string minuteString;
    if(hours >= 0 && hours < 10) {
        hourString = "0" + std::to_string(hours);
    } 
    if(hours >= 10 && hours < 24) {
        hourString = std::to_string(hours);
    } 
    if(minutes >= 0 && minutes < 10) {
        minuteString = "0" + std::to_string(minutes);
    } 
    if(minutes >= 10 && minutes < 60) {
        minuteString = std::to_string(minutes);
    } 
    clockTime = hourString + ":" + minuteString;
}

void Clock::Render()
{
}

bool Clock::Is(std::string type)
{
    return (type == Clock::type);
}