/**
 * @file State.cpp
 * @author Alexandre Augusto de Sá dos Santos (150056940@aluno.unb.br)
 * @brief State class implementation file
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../include/State.h"
#include "../include/Game.h"
#include <math.h>

const double PI = M_PI;

State::State()
{
    started = false;
    quitRequested = false;

    State::LoadAssets();

    GameObject *gameObjectOcean = new GameObject();

    // Background
    Sprite *bg = new Sprite(*gameObjectOcean, "assets/img/ocean.jpg");
    CameraFollower *OceanFollower = new CameraFollower(*gameObjectOcean);
    gameObjectOcean->AddComponent(bg);
    gameObjectOcean->AddComponent(OceanFollower);

    State::AddObject(gameObjectOcean);

    GameObject *gameObjectMap = new GameObject();

    // Map 
    TileSet *tileSet = new TileSet(64, 64, "assets/img/tileset.png");
    TileMap *tileMap = new TileMap(*gameObjectMap, "assets/map/tileMap.txt", tileSet);
    gameObjectMap->AddComponent(tileMap);
    gameObjectMap->box.setPosition(Vec2(0, 0));

    State::AddObject(gameObjectMap);

    // Alien 
    GameObject *gameObjectAlien = new GameObject();
    Alien *alien = new Alien(*gameObjectAlien, 3);

    gameObjectAlien->AddComponent(alien);
    gameObjectAlien->box.setPosition((Vec2(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT) - gameObjectAlien->box.measures())/2);

    State::AddObject(gameObjectAlien);

    // Penguin Player
    GameObject *gameObjectPenguin = new GameObject();
    PenguinBody *penguin = new PenguinBody(*gameObjectPenguin);

    gameObjectPenguin->AddComponent(penguin);
    gameObjectPenguin->box.setPosition({704, 640});

    State::AddObject(gameObjectPenguin);

    Camera::Follow(gameObjectPenguin);

    music = new Music("assets/audio/stageState.ogg");
    music->Play();
    started = true;
    std::cout << "got here H\n";
}

State::~State()
{
    objectArray.clear();
}

void State::Start()
{
    for(int i = 0; i < (int) objectArray.size(); i++){
		objectArray[i]->Start();
	}
    started = true;
}

void State::LoadAssets()
{
    // Load music, images, fonts here.
}

void State::Update(float dt)
{
    Camera::Update(dt);
    InputManager instance = InputManager::GetInstance();

    for (int pos = 0; pos < (int)objectArray.size(); pos++)
    {
        objectArray[pos]->Update(dt);
    }
    // Check for dead GOs
    for (int pos = 0; pos < (int)objectArray.size(); pos++)
    {
        if (objectArray[pos].get()->IsDead())
        {
            objectArray.erase(objectArray.begin() + pos);
        }
    }

    // Quits game
    if (instance.IsKeyDown(ESCAPE_KEY) || instance.QuitRequested())
    {
        quitRequested = true;
    }

    // Updates entities' state
    if (QuitRequested())
    {
        music->~Music();
    }
}

void State::Render()
{
    for (int pos = 0; pos < (int)objectArray.size(); pos++)
    {
        objectArray[pos]->Render();
    }
}

bool State::QuitRequested()
{
    return quitRequested;
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go)
{
    std::shared_ptr<GameObject> tempPtr(go);
    objectArray.push_back(tempPtr);
    if (started)
    {
        tempPtr->Start();
    }
    std::weak_ptr<GameObject> returnPtr(tempPtr);
    return returnPtr;
}

std::weak_ptr<GameObject> State::GetObject(GameObject *go)
{
    for (auto object : objectArray)
    {
        if (object.get() == go)
        {
            std::weak_ptr<GameObject> tempPtr(object);
            return tempPtr;
        }
    }
    std::weak_ptr<GameObject> nullPtr;
    return nullPtr;
}
