#pragma once
#include "Entity.h"
#include <string>
#include <SDL_ttf.h>

class Camera;

#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

class Application
{
private:
	//private variables
	static Application* m_application;
	SDL_Window *m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	AppState m_appState = AppState::INITILISING;
	float m_worldDeltaTime = 0.f;
	std::vector<Entity*> m_entities;
	Camera* m_mainCamera = nullptr;
	bool grounded = false;
	float m_randomNumber;
	int score = 137;
	int lives = 5;
	bool armor = false;
	SDL_Renderer* renderTarget = nullptr;
	SDL_Renderer* renderTarget1 = nullptr;

	
	//Game Sounds
	std::vector<std::string> soundList;
	std::vector<soundType> soundTypes;
	std::vector<std::string> soundsToUse;

	//private functions
	Application();
	void Init();
	void OpenGlInit();	
	void MousePos();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();
	void spawnSomethingBone();
	void spawnSomethingSpider();
	void CreateRandomNumber();

public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();
	float moving = -0.5f;
	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }
	
	inline Camera* GetCamera() { return m_mainCamera; }
	void SetCamera(Camera* camera);
	void GameInit();	
};

