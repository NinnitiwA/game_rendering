#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "resource.h"
#include "dataManager.h"

Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Resource::Init();

	// JSONからデータの取得
	SaveData();
	LoadData();

	m_Scene = new Title;
	m_Scene->Init();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Resource::Uninit();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	// Update処理時間計測
	GUI::BeginProfiling();

	Input::Update();

	if (m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}

	m_Scene->Update();


	// Update処理時間計測
	GUI::EndProfiling(UPDATE_DIVISION);
}

void Manager::Draw()
{
	m_Scene->Draw();

}
