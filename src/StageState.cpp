/**
 * @file StageState.cpp
 * @author Alexandre Augusto de Sá dos Santos (150056940@aluno.unb.br)
 * @brief StageState class implementation file. Where happens the first part of the game.
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "StageState.h"
#include "Constants.h"
#include "Sound.h"
#include "TileMap.h"
#include "Game.h"
#include "Text.h"
#include "Clock.h"
#include "Calendar.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Collision.cpp"
#include "TitleState.h"
#include "EndState.h"
#include "GameData.h"
#include "ReputationArrow.h"
#include "Client.h"
#include "SirenBox.h"

StageState::StageState(bool loadGame) : State(), backgroundMusic("assets/audio/chill.ogg")
{
	// Remove any previous save data from state
	GameData::Reset();
	if (loadGame)
	{
		// Load from save file
		GameData::Load();
		GameData::Load();
	}

	// Background
	GameObject *bg = new GameObject();
	bg->AddComponent(new Sprite(*bg, "assets/img/placeholders/CenarioPixel.png", 1, 1.0));
	bg->AddComponent(new CameraFollower(*bg));
	bg->box.SetOrigin(0, 0);
	AddObject(bg);

	// HUD Dia + Hora + Dinheiro
	GameObject *hudGO = new GameObject();
	hudGO->AddComponent(new GameItem(*hudGO, "assets/img/placeholders/Tela 1-dia_dinheiro.png", 1, 1));
	hudGO->box.SetOrigin(0, 0);
	AddObject(hudGO);

	GameObject *dayHudText = new GameObject();
	dayHudText->AddComponent(new Text(*dayHudText, "assets/font/five.ttf", 40, Text::SOLID, ("Day " + std::to_string(GameData::currentDay)), {255, 255, 255, SDL_ALPHA_OPAQUE}));
	dayHudText->box.SetOrigin(115, 35);
	AddObject(dayHudText);

	GameObject *timeHudText = new GameObject();
	timeHudText->AddComponent(new Clock(*timeHudText, GameData::currentHour, GameData::currentMinute));
	timeHudText->box.SetOrigin(300, 35);
	AddObject(timeHudText);

	GameObject *moneyHudText = new GameObject();
	moneyHudText->AddComponent(new Text(*moneyHudText, "assets/font/five.ttf", 40, Text::SOLID, ("R$ " + std::to_string(GameData::currentMoney)), {255, 255, 255, SDL_ALPHA_OPAQUE}));
	moneyHudText->box.SetOrigin(500, 35);
	AddObject(moneyHudText);

	// HUD Reputação
	GameObject *hudReputationGO2 = new GameObject();
	hudReputationGO2->AddComponent(new GameItem(*hudReputationGO2, "assets/img/placeholders/tela 1-Rep.png", 1, 1));
	hudReputationGO2->box.SetOrigin(960, 0);
	AddObject(hudReputationGO2);

	// HUD Seta da Reputação
	GameObject *hudReputationArrowGO = new GameObject();
	hudReputationArrowGO->AddComponent(new ReputationArrow(*hudReputationArrowGO, "assets/img/placeholders/tela 1-Rep_seta.png", GameData::currentRep, 1, 1));
	hudReputationArrowGO->AddComponent(new ReputationArrow(*hudReputationArrowGO, "assets/img/placeholders/tela 1-Rep_seta.png", GameData::currentRep, 1, 1));
	hudReputationArrowGO->box.SetOrigin(960, 60);
	AddObject(hudReputationArrowGO);

	// HUD Pause
	GameObject *hudPauseGO = new GameObject();
	hudPauseGO->AddComponent(new GameItem(*hudPauseGO, "assets/img/placeholders/Tela 1-Pause.png", 1, 1));
	hudPauseGO->box.SetOrigin(1800, 0);
	AddObject(hudPauseGO);

	// HUD Sirene
	GameObject *hudGOSirene = new GameObject();
	hudGOSirene->AddComponent(new SirenBox(*hudGOSirene));
	hudGOSirene->box.SetOrigin(1800, 355);
	AddObject(hudGOSirene);

	// Balcão
	GameObject *balcaoGO = new GameObject();
	balcaoGO->AddComponent(new GameItem(*balcaoGO, "assets/img/placeholders/balcão.png", 1, 1));
	balcaoGO->box.SetBottom(0, GAME_SCREEN_HEIGHT);
	AddObject(balcaoGO);

	// Lojeiro
	GameObject *lojeiroGO = new GameObject();
	lojeiroGO->AddComponent(new GameItem(*lojeiroGO, "assets/img/placeholders/lojeiro_placeholder.png", 1, 1));
	lojeiroGO->box.SetBottom(1290, balcaoGO->box.y);
	AddObject(lojeiroGO);

	// Velho
	GameObject *velhoGO = new GameObject();
	velhoGO->AddComponent(new Client(*velhoGO, "assets/img/placeholders/velinho.png", 1, 1));
	velhoGO->box.SetBottom(0, GAME_SCREEN_HEIGHT);
	AddObject(velhoGO);
}

StageState::~StageState()
{
	objectArray.clear();
}

void StageState::Start()
{
	LoadAssets();
	StartArray();
	backgroundMusic.Play();
	started = true;
}

void StageState::LoadAssets()
{
}

void StageState::Pause()
{
	backgroundMusic.Stop(0);
	backgroundMusic.Stop(0);
}

void StageState::Resume()
{
	backgroundMusic.Play();
}

void StageState::Update(float dt)
{
	// update camera
	Camera::Update(dt);

	InputManager input = InputManager::GetInstance();

	// check if quit was requested
	if (input.QuitRequested())
	{
		quitRequested = true;
	}

	if (input.KeyPress(ESCAPE_KEY))
	{
		this->Pause();
		popRequested = true;
	}

	if (input.MousePress(LEFT_MOUSE_BUTTON))
	{
#ifdef DEBUG
		std::cout << "X:" << input.GetMouseX() << " Y:" << input.GetMouseY() << "\n";
#endif
	}

	// Update every object
	UpdateArray(dt);

	srand(time(NULL));
}

void StageState::Render()
{
	// Render every object
	RenderArray();
}
