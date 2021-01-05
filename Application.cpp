#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "RigidBody.h"
#include "cSound.h"
#include "cSoundMgr.h"
#include <stdlib.h> 
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>  
#include <string>  


Application *Application::m_application = nullptr;
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
CameraComp* cc = new CameraComp();
//cSound* theSound = cSound::pauseMusic();
const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
//Declaring Objects
Entity* b = new Entity();
Entity* spawn = new Entity();
Entity* bone = new Entity();
Entity* shield = new Entity();
Entity* shield2 = new Entity();
Entity* shield3 = new Entity();
Entity* shieldCollect = new Entity();
Entity* bone2 = new Entity();
Entity* bone3 = new Entity();
Entity* ground = new Entity();
Entity* ground1 = new Entity();
Entity* ground2 = new Entity();
Entity* ground3 = new Entity();
Entity* ground4 = new Entity();
Entity* ground5 = new Entity();
Entity* spider = new Entity();
Entity* spider2 = new Entity();
Entity* spider3 = new Entity();
Entity* deathBox = new Entity();
Entity* wallLeft1 = new Entity();
Entity* wallLeft2 = new Entity();
Entity* wallLeft3 = new Entity();
Entity* wallLeft4 = new Entity();
Entity* wallLeft5 = new Entity();
Entity* wallLeft6 = new Entity();
Entity* wallLeft7 = new Entity();
Entity* wallLeft8 = new Entity();
Entity* wallLeft9 = new Entity();
Entity* wallLeft10 = new Entity();
Entity* wallLeft11 = new Entity();
Entity* wallLeft12 = new Entity();
Entity* wallLeft13 = new Entity();
Entity* wallLeft14 = new Entity();
Entity* wallRight1 = new Entity();
Entity* wallRight2 = new Entity();
Entity* wallRight3 = new Entity();
Entity* wallRight4 = new Entity();
Entity* wallRight5 = new Entity();
Entity* wallRight6 = new Entity();
Entity* wallRight7 = new Entity();
Entity* wallRight8 = new Entity();
Entity* wallRight9 = new Entity();
Entity* wallRight10 = new Entity();
Entity* wallRight11 = new Entity();
Entity* wallRight12 = new Entity();
Entity* wallRight13 = new Entity();
Entity* wallRight14 = new Entity();




Application::Application()
{
}

void Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}
	//Text Error
	if (TTF_Init() < 0)
	{
		std::cout << "Error: " << TTF_GetError() << std::endl;
	}

	//setting openGL version to 4.2 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-Course-Work", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);

	//Display Socre - https://www.youtube.com/watch?v=lfz_X5YgpT8
	glDisable(GL_DEPTH_TEST);
	renderTarget = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Font* font = TTF_OpenFont("BlackPearl.ttf", 50);
	SDL_Color color = { 144, 77, 255, 255 };

	//Convert int to Const Char*
	string str = "Score = " + to_string(score) + " & lives = " + to_string(lives) + " A = Left, D = Right";
	char* cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	std::cout << cstr << endl;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, cstr, color);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
	SDL_Rect textRect;
	textRect.x = textRect.y = 0;

	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(textSurface);
	SDL_RenderCopy(renderTarget, text, NULL, &textRect);
	SDL_RenderPresent(renderTarget);

	/*renderTarget1 = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Font* font1 = TTF_OpenFont("BlackPearl.ttf", 40);
	SDL_Color color1 = { 144, 77, 255, 255 };
	//Convert int to Const Char*
	//string str = to_string(score);
	//char* cstr = new char[str.length() + 1];
	//strcpy(cstr, str.c_str());
	//std::cout << cstr << endl;

	SDL_Surface* textSurface1 = TTF_RenderText_Solid(font1, "Score = ", color1);
	SDL_Texture* text1 = SDL_CreateTextureFromSurface(renderTarget1, textSurface1);
	SDL_Rect textRect1;
	textRect1.x = textRect1.y = 50;

	SDL_QueryTexture(text1, NULL, NULL, &textRect1.w, &textRect1.h);
	SDL_FreeSurface(textSurface1);
	SDL_RenderCopy(renderTarget1, text1, NULL, &textRect1);
	SDL_RenderPresent(renderTarget1);*/
	
	SDL_CaptureMouse(SDL_TRUE);

	OpenGlInit();	
	GameInit();
}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	SDL_GL_SetSwapInterval(1);

	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glDisable(GL_CULL_FACE);
}

void Application::MousePos()
{
	
}

void Application::GameInit()
{
	

	//Init the sound manager------------------------------------------------------------------
	if (theSoundMgr->initMixer())
	{
		//load game sounds
		soundList = { "theme", "click", "woof", "whining", "bounce", "armor" };
		soundTypes = { soundType::music, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx };

		soundsToUse = { ASSET_AUDIO_PATH_Theme + "themeMusic.mp3",  ASSET_AUDIO_PATH_SFX + "ClickOn.wav", ASSET_AUDIO_PATH_SFX + "woof1.mp3", ASSET_AUDIO_PATH_SFX + "Whining.mp3", ASSET_AUDIO_PATH_SFX + "bounce.wav", ASSET_AUDIO_PATH_SFX + "Armor.wav" };

		for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
		{
			theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
		}
		//play the theme on a loop
		theSoundMgr->getSnd("theme")->play(-1);
	}

	//loading all resources-----------------------------------------------------------------
	Resources::GetInstance()->AddModel("Models/cube.obj");
	Resources::GetInstance()->AddModel("Models/cube2.obj");
	Resources::GetInstance()->AddModel("Models/Dachshund.obj");
	Resources::GetInstance()->AddModel("Models/dalek.obj");
	Resources::GetInstance()->AddModel("Models/bone.obj");
	Resources::GetInstance()->AddModel("Models/bridge.obj");
	Resources::GetInstance()->AddModel("Models/Wall.obj");
	Resources::GetInstance()->AddModel("Models/Shield.obj");
	Resources::GetInstance()->AddTexture("Images/Textures/Wood.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/fur.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/bone.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/floor.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/black.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/redBlack.jfif");
	Resources::GetInstance()->AddTexture("Images/Textures/wall.jpg");
	Resources::GetInstance()->AddTexture("Images/Textures/wall2.bmp");
	Resources::GetInstance()->AddTexture("Images/Textures/steel.jpg");

		/*Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "Shaders/Phong.frag", 
		ASSET_PATH + "Shaders/Phong.vert"), 
		"simple"*/

		Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "Shaders/basic_VERT.glsl",
		ASSET_PATH + "Shaders/basic_FRAG.glsl"),
		"simple"
	);
	
	//Object 1 ----------------------------------------------------------------------------
	/*Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(1.f, 0.01f, 1.f));*/

	//Object 2 Player--------------------------------------------------------------------
	m_entities.push_back(b);
	b->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Dachshund.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/black.jpg"))
	);
	MeshRenderer* n = b->GetComponent<MeshRenderer>();
	b->GetTransform()->SetPosition(glm::vec3(5.f, 20.f, 20.f));
	b->AddComponent<RigidBody>();
	b->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(2.f, 0.5f, 2.f)));
	//b->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	b->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	//Object 3--------------------------------------------------------------------------
	/*for (int i = 0; i < 10; i++)
	{
		Entity* c = new Entity();
		m_entities.push_back(c);
		c->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/dalek.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
		);
		MeshRenderer* o = c->GetComponent<MeshRenderer>();
		c->GetTransform()->SetPosition(glm::vec3(-5.f, 5.f * i, -20.f));
		c->AddComponent<RigidBody>();
		c->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		//c->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		c->GetTransform()->SetScale(glm::vec3(0.001f, 0.001f, 0.001f));
	}*/

	//Object Shield
	//1
	m_entities.push_back(shield);
	shield->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Shield.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/steel.jpg"))
		);
	MeshRenderer* m_shield = shield->GetComponent<MeshRenderer>();

	shield->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(0.f, 0.0f, 3.f));
	shield->AddComponent<RigidBody>();
	shield->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(2.f, 0.1f, 0.2f)));
	shield->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	shield->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	/*
	m_entities.push_back(shield2);
	shield2->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Shield.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/steel.jpg"))
	);
	MeshRenderer* m_shield2 = shield2->GetComponent<MeshRenderer>();

	shield2->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(-5.f, 0.0f, 0.f));
	shield2->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
	shield2->AddComponent<RigidBody>();
	shield2->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.8f, 0.1f, 0.2f)));
	shield2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	shield2->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//3
	m_entities.push_back(shield3);
	shield3->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Shield.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/steel.jpg"))
	);
	MeshRenderer* m_shield3 = shield3->GetComponent<MeshRenderer>();

	shield3->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(5.f, 0.0f, 0.f));
	shield3->GetTransform()->SetRotation(glm::quat(1.f, 0.f, 1.f, 0.f));
	shield3->AddComponent<RigidBody>();
	shield3->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.8f, 0.1f, 0.2f)));
	shield3->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	shield3->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));*/


	//Object power up
	m_entities.push_back(shieldCollect);
	shieldCollect->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Shield.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/steel.jpg"))
	);
	MeshRenderer* m_shieldCollect = shieldCollect->GetComponent<MeshRenderer>();

	shieldCollect->GetTransform()->SetPosition(glm::vec3(5.f, 10.0f, 30.f));
	shieldCollect->AddComponent<RigidBody>();
	shieldCollect->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.8f, 0.1f, 0.2f)));
	shieldCollect->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	shieldCollect->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	//Object 4 Bone---------------------------------------------------------------------
	//Bone 1
	m_entities.push_back(bone);
	bone->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bone.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/bone.jpg"))
	);
	MeshRenderer* m_bone = bone->GetComponent<MeshRenderer>();
	
	bone->GetTransform()->SetPosition(glm::vec3(-10.f, 10.f, 70.f));
	bone->AddComponent<RigidBody>();
	bone->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.01f, 1.0f, 0.01f)));
	bone->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	bone->GetTransform()->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	//Bone 2
	m_entities.push_back(bone2);
	bone2->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bone.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/bone.jpg"))
	);
	MeshRenderer* m_bone2 = bone2->GetComponent<MeshRenderer>();

	bone2->GetTransform()->SetPosition(glm::vec3(20.f, 10.f, 90.f));
	bone2->AddComponent<RigidBody>();
	bone2->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.01f, 1.0f, 0.01f)));
	bone2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	bone2->GetTransform()->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	//Bone 3
	m_entities.push_back(bone3);
	bone3->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bone.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/bone.jpg"))
	);
	MeshRenderer* m_bone3 = bone3->GetComponent<MeshRenderer>();

	bone3->GetTransform()->SetPosition(glm::vec3(0.f, 10.f, 50.f));
	bone3->AddComponent<RigidBody>();
	bone3->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(0.01f, 1.0f, 0.01f)));
	bone3->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	bone3->GetTransform()->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	

	//Object 5 Floor---------------------------------------------------------------------	
	//Floor 0
	m_entities.push_back(ground);
	ground->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground = ground->GetComponent<MeshRenderer>();
	ground->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 210.f));
	ground->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground->AddComponent<RigidBody>();
	ground->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));
	//Floor 1
	m_entities.push_back(ground1);
	ground1->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground1 = ground1->GetComponent<MeshRenderer>();
	ground1->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 160.f));
	ground1->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground1->AddComponent<RigidBody>();
	ground1->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground1->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground1->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));
	//Floor 2
	m_entities.push_back(ground2);
	ground2->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground2 = ground2->GetComponent<MeshRenderer>();
	ground2->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 110.f));
	ground2->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground2->AddComponent<RigidBody>();
	ground2->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground2->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));
	//Floor 3
	m_entities.push_back(ground3);
	ground3->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground3 = ground3->GetComponent<MeshRenderer>();
	ground3->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 60.f));
	ground3->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground3->AddComponent<RigidBody>();
	ground3->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground3->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground3->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));
	//Floor 4
	m_entities.push_back(ground4);
	ground4->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground4 = ground4->GetComponent<MeshRenderer>();
	ground4->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 10.f));
	ground4->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground4->AddComponent<RigidBody>();
	ground4->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground4->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground4->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));
	//Floor 5
	m_entities.push_back(ground5);
	ground5->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/bridge.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/Wood.jpg"))
	);
	MeshRenderer* m_ground5 = ground5->GetComponent<MeshRenderer>();
	ground5->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, -50.f));
	ground5->GetTransform()->SetRotation(glm::quat(1.0f, 0.f, 1.f, 0.f));
	ground5->AddComponent<RigidBody>();
	ground5->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(40.f, 10.0f, 200.f)));
	ground5->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	ground5->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.4f));

	//Object 6 Spider---------------------------------------------------------------------
	m_entities.push_back(spider);
	spider->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Images/Textures/redBlack.jfif"))
	);
	MeshRenderer* m_spider = spider->GetComponent<MeshRenderer>();
	spider->GetTransform()->SetPosition(glm::vec3(8.f, 20.f, 30.f));
	spider->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
	spider->AddComponent<RigidBody>();
	spider->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(2.f, 1.f, 2.f)));
	//spider->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	spider->GetTransform()->SetScale(glm::vec3(0.02f, 0.02f, 0.02f));

	//Object 7 Death Box-----------------------------------------------------------------
	m_entities.push_back(deathBox);
	deathBox->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, -90.f));
	deathBox->AddComponent<RigidBody>();
	deathBox->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(20.f, 100.f, 10.f)));
	deathBox->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	deathBox->GetTransform()->SetScale(glm::vec3(100.0f, 100.0f, 10.0f));

		
		//wall 1
		m_entities.push_back(wallLeft1);
		wallLeft1->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft1 = wallLeft1->GetComponent<MeshRenderer>();
		wallLeft1->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 30.f));
		wallLeft1->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft1->AddComponent<RigidBody>();
		wallLeft1->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft1->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft1->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));
	
		//wall 2
		m_entities.push_back(wallLeft2);
		wallLeft2->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallleft2 = wallLeft2->GetComponent<MeshRenderer>();
		wallLeft2->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, -30.f));
		wallLeft2->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft2->AddComponent<RigidBody>();
		wallLeft2->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallLeft2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft2->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 3
		m_entities.push_back(wallLeft3);
		wallLeft3->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallleft3 = wallLeft3->GetComponent<MeshRenderer>();
		wallLeft3->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, -10.f));
		wallLeft3->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft3->AddComponent<RigidBody>();
		wallLeft3->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallLeft3->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft3->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 4
		m_entities.push_back(wallLeft4);
		wallLeft4->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallleft4 = wallLeft4->GetComponent<MeshRenderer>();
		wallLeft4->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 10.f));
		wallLeft4->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft4->AddComponent<RigidBody>();
		wallLeft4->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft4->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft4->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 5
		m_entities.push_back(wallLeft5);
		wallLeft5->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallleft5 = wallLeft5->GetComponent<MeshRenderer>();
		wallLeft5->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 30.f));
		wallLeft5->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft5->AddComponent<RigidBody>();
		wallLeft5->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallLeft5->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft5->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 6
		m_entities.push_back(wallLeft6);
		wallLeft6->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft6 = wallLeft6->GetComponent<MeshRenderer>();
		wallLeft6->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 50.f));
		wallLeft6->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft6->AddComponent<RigidBody>();
		wallLeft6->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft6->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft6->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 7
		m_entities.push_back(wallLeft7);
		wallLeft7->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft7 = wallLeft7->GetComponent<MeshRenderer>();
		wallLeft7->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, -50.f));
		wallLeft7->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft7->AddComponent<RigidBody>();
		wallLeft7->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft7->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft7->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 8
		m_entities.push_back(wallLeft8);
		wallLeft8->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft8 = wallLeft8->GetComponent<MeshRenderer>();
		wallLeft8->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, -70.f));
		wallLeft8->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft8->AddComponent<RigidBody>();
		wallLeft8->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft8->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft8->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 9
		m_entities.push_back(wallLeft9);
		wallLeft9->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft9 = wallLeft9->GetComponent<MeshRenderer>();
		wallLeft9->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 50.f));
		wallLeft9->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft9->AddComponent<RigidBody>();
		wallLeft9->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft9->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft9->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 10
		m_entities.push_back(wallLeft10);
		wallLeft10->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft10 = wallLeft10->GetComponent<MeshRenderer>();
		wallLeft10->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 70.f));
		wallLeft10->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft10->AddComponent<RigidBody>();
		wallLeft10->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft10->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft10->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 11
		m_entities.push_back(wallLeft11);
		wallLeft11->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft11 = wallLeft11->GetComponent<MeshRenderer>();
		wallLeft11->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 90.f));
		wallLeft11->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft11->AddComponent<RigidBody>();
		wallLeft11->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft11->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft11->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 12
		m_entities.push_back(wallLeft12);
		wallLeft12->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft12 = wallLeft12->GetComponent<MeshRenderer>();
		wallLeft12->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 110.f));
		wallLeft12->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft12->AddComponent<RigidBody>();
		wallLeft12->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft12->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft12->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 13
		m_entities.push_back(wallLeft13);
		wallLeft13->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft13 = wallLeft13->GetComponent<MeshRenderer>();
		wallLeft13->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 130.f));
		wallLeft13->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft13->AddComponent<RigidBody>();
		wallLeft13->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft13->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft13->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 14
		m_entities.push_back(wallLeft14);
		wallLeft14->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallLeft14 = wallLeft14->GetComponent<MeshRenderer>();
		wallLeft14->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		wallLeft14->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallLeft14->AddComponent<RigidBody>();
		wallLeft14->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallLeft14->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallLeft14->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 1
		m_entities.push_back(wallRight1);
		wallRight1->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight1 = wallRight1->GetComponent<MeshRenderer>();
		wallRight1->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 30.f));
		wallRight1->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight1->AddComponent<RigidBody>();
		wallRight1->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight1->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight1->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 2
		m_entities.push_back(wallRight2);
		wallRight2->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight2 = wallRight2->GetComponent<MeshRenderer>();
		wallRight2->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, -30.f));
		wallRight2->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight2->AddComponent<RigidBody>();
		wallRight2->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallRight2->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight2->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 3
		m_entities.push_back(wallRight3);
		wallRight3->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight3 = wallRight3->GetComponent<MeshRenderer>();
		wallRight3->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, -10.f));
		wallRight3->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight3->AddComponent<RigidBody>();
		wallRight3->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallRight3->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight3->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 4
		m_entities.push_back(wallRight4);
		wallRight4->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight4 = wallRight4->GetComponent<MeshRenderer>();
		wallRight4->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 10.f));
		wallRight4->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight4->AddComponent<RigidBody>();
		wallRight4->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight4->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight4->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 5
		m_entities.push_back(wallRight5);
		wallRight5->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight5 = wallRight5->GetComponent<MeshRenderer>();
		wallRight5->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 30.f));
		wallRight5->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight5->AddComponent<RigidBody>();
		wallRight5->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 1.f)));
		wallRight5->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight5->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		

		//wall 6
		m_entities.push_back(wallRight6);
		wallRight6->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight6 = wallRight6->GetComponent<MeshRenderer>();
		wallRight6->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 50.f));
		wallRight6->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight6->AddComponent<RigidBody>();
		wallRight6->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight6->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight6->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 7
		m_entities.push_back(wallRight7);
		wallRight7->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight7 = wallRight7->GetComponent<MeshRenderer>();
		wallRight7->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, -50.f));
		wallRight7->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight7->AddComponent<RigidBody>();
		wallRight7->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight7->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight7->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 8
		m_entities.push_back(wallRight8);
		wallRight8->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight8 = wallRight8->GetComponent<MeshRenderer>();
		wallRight8->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, -70.f));
		wallRight8->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight8->AddComponent<RigidBody>();
		wallRight8->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight8->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight8->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 9
		m_entities.push_back(wallRight9);
		wallRight9->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight9 = wallRight9->GetComponent<MeshRenderer>();
		wallRight9->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 50.f));
		wallRight9->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight9->AddComponent<RigidBody>();
		wallRight9->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight9->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight9->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 10
		m_entities.push_back(wallRight10);
		wallRight10->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight10 = wallRight10->GetComponent<MeshRenderer>();
		wallRight10->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 70.f));
		wallRight10->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight10->AddComponent<RigidBody>();
		wallRight10->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight10->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight10->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 11
		m_entities.push_back(wallRight11);
		wallRight11->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight11 = wallRight11->GetComponent<MeshRenderer>();
		wallRight11->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 90.f));
		wallRight11->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight11->AddComponent<RigidBody>();
		wallRight11->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight11->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight11->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 12
		m_entities.push_back(wallRight12);
		wallRight12->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight12 = wallRight12->GetComponent<MeshRenderer>();
		wallRight12->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 110.f));
		wallRight12->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight12->AddComponent<RigidBody>();
		wallRight12->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight12->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight12->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 13
		m_entities.push_back(wallRight13);
		wallRight13->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight13 = wallRight13->GetComponent<MeshRenderer>();
		wallRight13->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 130.f));
		wallRight13->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight13->AddComponent<RigidBody>();
		wallRight13->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight13->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight13->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

		//wall 14
		m_entities.push_back(wallRight14);
		wallRight14->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("Models/Wall.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Images/Textures/wall2.bmp"))
		);
		MeshRenderer* m_wallRight14 = wallRight14->GetComponent<MeshRenderer>();
		wallRight14->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		wallRight14->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
		wallRight14->AddComponent<RigidBody>();
		wallRight14->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(30.f, 30.0f, 2.f)));
		wallRight14->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		wallRight14->GetTransform()->SetScale(glm::vec3(1.f, 1.0f, 1.f));

	Entity* c = new Entity();
	c = new Entity();
	m_entities.push_back(c);
	CameraComp* cc = new CameraComp();
	b->AddComponent(cc);
	cc->Start();

	Resources::GetInstance()->ReleaseUnusedResources();
}

void Application::Loop()
{
	

	m_appState = AppState::RUNNING;	
	//std::cout << playerX << std::endl;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	while (m_appState != AppState::QUITTING)
	{
		//Collision---------------------------------------------------------------------
		//Player and bone - Reset bone position and increase score
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, bone->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			std::cout << "Bone Col, score = " << score << std::endl;
			theSoundMgr->getSnd("woof")->play(0);
			CreateRandomNumber();
			bone->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));;
			score++;
		}
		//Player and bone2 - Reset bone position and increase score
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, bone2->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			std::cout << "Bone Col, score = " << score << std::endl;
			theSoundMgr->getSnd("woof")->play(0);
			CreateRandomNumber();
			bone2->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));
			score++;
		}
		//Player and bone3 - Reset bone position and increase score
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, bone3->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			std::cout << "Bone Col, score = " << score << std::endl;
			theSoundMgr->getSnd("woof")->play(0);
			CreateRandomNumber();
			bone3->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));
			score++;
		}
		//Bone and death box - Reset bone position
		if (Physics::GetInstance()->Collision3D(bone->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			CreateRandomNumber();
			bone->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));;
		}
		if (Physics::GetInstance()->Collision3D(bone2->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			CreateRandomNumber();
			bone2->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));;
		}
		if (Physics::GetInstance()->Collision3D(bone3->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			CreateRandomNumber();
			bone3->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));;
		}
		//Player and spider - Reset spider and lose a life
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, spider->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			std::cout << "Spider Col, lives = " << lives << std::endl;
			theSoundMgr->getSnd("whining")->play(0);
			spawnSomethingSpider();
			lives--;
		}
		//Spider and death box - Reset Spider
		if (Physics::GetInstance()->Collision3D(spider->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			spawnSomethingSpider();
		}

		//Spider and shield - play sound and apply force to the spider on the Y-axis
		if (Physics::GetInstance()->Collision3D(spider->GetComponent<RigidBody>()->Get(), 0, 0, shield->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			theSoundMgr->getSnd("bounce")->play(0);
			spider->GetComponent<RigidBody>()->Get()->applyCentralImpulse(btVector3(0.f, 20.f, 0.f));
		}

		//Player and power up - Reset the collectable position, player gains shield power up for a set time (is protected from spider that are in front of him)
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, shieldCollect->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			shieldCollect->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 200.f));;
			theSoundMgr->getSnd("armor")->play(0);
			armor = true;
			timer = 600;
			shield->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(0.f, 0.0f, 3.f));	
			shield2->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(1.f, 0.f, 0.f));
			shield3->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(-1.f, 0.f, 0.f));
		}

		//Death Box and power up - reset power up position
		if (Physics::GetInstance()->Collision3D(deathBox->GetComponent<RigidBody>()->Get(), 0, 0, shieldCollect->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			shieldCollect->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 200.f));;
		}

		//Ground and Death Box - Reset ground position
		if (Physics::GetInstance()->Collision3D(ground->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		if (Physics::GetInstance()->Collision3D(ground1->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground1->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		if (Physics::GetInstance()->Collision3D(ground2->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground2->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		if (Physics::GetInstance()->Collision3D(ground3->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground3->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		if (Physics::GetInstance()->Collision3D(ground4->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground4->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		if (Physics::GetInstance()->Collision3D(ground5->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			ground5->GetTransform()->SetPosition(glm::vec3(0.f, 0.f, 260.f));
		}
		//Left wall - Reset wall position
		if (Physics::GetInstance()->Collision3D(wallLeft1->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft1->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft2->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft2->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft3->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft3->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft4->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft4->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft5->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft5->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft6->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft6->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft7->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft7->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft8->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft8->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft9->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft9->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft10->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft10->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft11->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft11->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft12->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft12->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft13->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft13->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallLeft14->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallLeft14->GetTransform()->SetPosition(glm::vec3(20.f, -2.f, 150.f));
		}

		//Right wall - Reset wall position
		if (Physics::GetInstance()->Collision3D(wallRight1->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight1->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight2->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight2->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight3->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight3->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight4->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight4->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight5->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight5->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight6->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight6->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight7->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight7->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight8->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight8->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight9->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight9->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight10->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight10->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight11->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight11->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight12->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight12->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight13->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight13->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}
		if (Physics::GetInstance()->Collision3D(wallRight14->GetComponent<RigidBody>()->Get(), 0, 0, deathBox->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			wallRight14->GetTransform()->SetPosition(glm::vec3(-20.f, -2.f, 150.f));
		}

		// If player is colliding with the ground then the player is "grounded" and able to jump
		if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground1->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground2->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground3->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground4->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else if (Physics::GetInstance()->Collision3D(b->GetComponent<RigidBody>()->Get(), 0, 0, ground5->GetComponent<RigidBody>()->Get(), 1, 1) == true)
		{
			//std::cout << "Ground Col" << std::endl;
			grounded = true;
		}
		else
		{
			grounded = false;
		}




		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_a:
					//b->GetTransform()->AddPosition(glm::vec3(1.0f, 0.f, 0.f));
					//b->GetComponent<RigidBody>()->Get()->applyCentralImpulse(btVector3(1.f, 0.f, 0.f));
					break;
				case SDLK_d:
					//b->GetTransform()->AddPosition(glm::vec3(-1.0f, 0.f, 0.f));
					//b->GetComponent<RigidBody>()->Get()->applyCentralImpulse(btVector3(-1.f, 0.f, 0.f));
					break;
				case SDLK_s:
					b->GetTransform()->AddPosition(glm::vec3(0.0f, 0.f, -1.f));
					break;
				case SDLK_w:
					b->GetTransform()->AddPosition(glm::vec3(0.0f, 0.f, 1.f));
					theSoundMgr->getSnd("click")->play(0);
					break;
				case SDLK_q:
					m_mainCamera->SetProjPersp(45.f, (float)WINDOW_W / (float)WINDOW_H, 0.1f, 1000.f);
					theSoundMgr->getSnd("theme")->resumeMusic();
					break;
				case SDLK_e:
					m_mainCamera->SetProjOrtho(-500, (float)WINDOW_W, 0, (float)WINDOW_H, 0.1f, 1000.f);
					theSoundMgr->getSnd("theme")->pauseMusic();
					break;
				case SDLK_t:
					//spawnSomethingBone();
					//Score Text
					break;
				case SDLK_o:
					b->AddComponent(cc);
					cc->Start2();
					m_mainCamera->SetCamera2();
					break;
				case SDLK_p:
					b->AddComponent(cc);
					cc->Start();
					m_mainCamera->SetCamera1();
					break;
				case SDLK_i:
					if (grounded)
					{
						b->GetComponent<RigidBody>()->Get()->applyCentralImpulse(btVector3(0.f, 20.f, 0.f));
					}
					break;
				case SDLK_ESCAPE:
					m_appState = AppState::QUITTING;
					break;
				case SDLK_SPACE:
					gameActive = true;
					break;
				case SDLK_l:					
					if (musicOn)
					{
						theSoundMgr->getSnd("theme")->pauseMusic();
						musicOn = false;
					}
					else if (!musicOn)
					{
						theSoundMgr->getSnd("theme")->resumeMusic();
						musicOn = true;
					}
					break;
				}
				//record when the user releases a key
			case SDL_MOUSEMOTION:
				/*INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				SDL_SetRelativeMouseMode(SDL_TRUE);
				if (SDL_GetRelativeMouseMode() == true)
				{
					b->GetTransform()->RotateEulerAxis((m_worldDeltaTime * 1)* event.motion.xrel, b->GetTransform()->GetUp());
					b->GetTransform()->RotateEulerAxis((m_worldDeltaTime*1) * event.motion.yrel, b->GetTransform()->GetRight());
				}*/
				break;
			}
		}

		if (currentKeyStates[SDL_SCANCODE_D])
		{
			b->GetTransform()->AddPosition(glm::vec3(-0.3f, 0.f, 0.f));
		}
		if 	(currentKeyStates[SDL_SCANCODE_A])
		{
			b->GetTransform()->AddPosition(glm::vec3(0.3f, 0.f, 0.f));
		}

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		if (gameActive)
		{		
		shield->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(0.f, 0.0f, 3.f));
		shield2->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(-2.f, 0.0f, 0.f));
		shield3->GetTransform()->SetPosition(glm::vec3(b->GetTransform()->GetPosition()) + glm::vec3(2.f, 0.0f, 0.f));
		bone->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		bone2->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		bone3->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		spider->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground1->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground2->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground3->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground4->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		ground5->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft1->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft2->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft3->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft4->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft5->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft6->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft7->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft8->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft9->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft10->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft11->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft12->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft13->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallLeft14->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight1->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight2->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight3->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight4->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight5->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight6->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight7->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight8->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight9->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight10->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight11->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight12->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight13->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		wallRight14->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));

		shieldCollect->GetTransform()->AddRotation(glm::quat(1.0f, 0.f, -0.05f, 0.f));
		shieldCollect->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, moving));
		}		

		if (armor)
		{					
			timer--;
			if (timer < 0)
			{
				armor = false;
			}					
		}
		if (!armor)
		{
			shield->GetTransform()->SetPosition(glm::vec3(0.f, -200.f, 0.f));
			shield2->GetTransform()->SetPosition(glm::vec3(0.f, -200.f, 0.f));
			shield3->GetTransform()->SetPosition(glm::vec3(0.f, -200.f, 0.f));
		}

		if (lives <= 0)
		{
			gameOver = true;
		}

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities
		Update(deltaTime);
		Render();

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	//SDL_DestroyTexture(text);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	//IMG_Quit();
	SDL_Quit();
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}


Application::~Application()
{

}

Application * Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

//test

void Application::CreateRandomNumber()
{
	m_randomNumber = rand() % 20 + -10;
}

void Application::spawnSomethingBone()
{
	CreateRandomNumber();
	bone->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 50.f));
}
void Application::spawnSomethingSpider()
{
	CreateRandomNumber();
	spider->GetTransform()->SetPosition(glm::vec3(m_randomNumber, 10.f, 70.f));
	spider->GetTransform()->SetRotation(glm::quat(1.f, 0.f, -1.f, 0.f));
}