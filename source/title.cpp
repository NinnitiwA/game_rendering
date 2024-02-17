#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "workspace.h"
#include "input.h"
#include "postProcess.h"

#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "skydome.h"
#include "water.h"
#include "ground.h"
#include "grass.h"
#include "tree.h"
#include "fade.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "polygon.h"

void Title::Init()
{
	// 初期設定
	m_SceneName = TITLE_SCENE;
	GUI::drawGuiFlag = false;
	GUI::bloom = false;
	GUI::luminanceValue = 0.9f;

	// システムオブジェクト
	AddGameObject<Camera>(0)->SetFixedDistance(D3DXVECTOR3(-3.0f, 6.0f, -12.0f));
	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFadeOut();

	// ポストプロセス
	m_PostProcess = new PostProcess();
	m_PostProcess->Init();
	m_Polygon2D = new Polygon2D();
	m_Polygon2D->Init();

	// プレイヤー
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	player->SetRotation(D3DXVECTOR3(0.0f, 180.0f * D3DX_PI/ 180.0f, 0.0f));

	// 環境オブジェクト
	AddGameObject<Sky>(1);
	Water::CreateWave();
	AddGameObject<Water>(1)->CreateWater(true);
	AddGameObject<DirectionalLight>(1);
	AddGameObject<PointLight>(1);
	AddGameObject<SpotLight>(1);
	InitTitleStage();

	// UIオブジェクト
	UIObject* titleUI = AddGameObject<UIObject>(3);
	titleUI->Init(20.0f + (SCREEN_WIDTH / 2.0f), -60.0f + (SCREEN_HEIGHT / 2.0f), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	titleUI->SetUIType(NORMAL_TYPE, 0);
	UIObject* selectUI = AddGameObject<UIObject>(3);
	selectUI->Init(20.0f + (SCREEN_WIDTH / 2.0f), 30.0f + (SCREEN_HEIGHT / 2.0f), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	selectUI->SetUIType(BLINK_TYPE, 1);

	// オーディオ
    Resource::GetMainThemeAudio()->Play();

	getchar();
}

void Title::Update()
{
	Scene::Update();

	// ポストプロセス
	m_PostProcess->Update();

	// ゲームシーン推移
	if (Input::GetKeyPress('1'))
	{
		m_NextScene = true;
		m_Fade->SetFadeTexture(true);
		m_Fade->SetFadeIn();
		GUI::sceneEndFlag = false;
		Resource::GetMainThemeAudio()->Stop();
        Resource::GetJumpSEAudio()->Play();
	}
	if (m_Fade->GetFadeFinished() && m_NextScene)Manager::SetScene<Tutorial>();
	if (Input::GetKeyPress('2'))
	{
		m_NextScene = false;
		m_Fade->SetFadeTexture(false);
		m_Fade->SetFadeIn();
		GUI::sceneEndFlag = false;
		Resource::GetMainThemeAudio()->Stop();
        Resource::GetJumpSEAudio()->Play();
	}
	if (m_Fade->GetFadeFinished() && !m_NextScene)Manager::SetScene<WorkSpace>();
}

void Title::Draw()
{
	//********************************デファードレンダリング********************************

	Renderer::SetSamplerStateWRAP();
	GUI::BeginProfiling();

	// ディレクショナルライトのシャドウマップ生成-----------------------------------

	Camera* camera = GetGameObject<Camera>();

	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		Renderer::BeginShadowMap(i);

		// ビューポート設定
		if (i == 0)	Renderer::SetWideViewport();         // 2倍
		else if (i == 1) Renderer::SetDefaultViewport(); // 1倍
		else Renderer::SetVariableViewport(0);           // 0.5倍


		camera->DrawCascadeShadowMap(i);
		for (GameObject* gameObject : m_GameObject[1])
		{
			gameObject->DrawShadowMapping();
		}
	}



	GUI::EndProfiling(SHADOWMAP_DIVISION);
	Renderer::SetDefaultViewport();


	// 水面反射用テクスチャ生成-----------------------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginReflection();

	camera->DrawReflection();
	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->DrawReflection();
	}

	GUI::EndProfiling(REFLECTION_DIVISION);



	// デバッグ--------------------------------------------------------------------
	// カスケードシャドウ----------------------------------------------------------
	Renderer::BeginDebugCascadeShadowView();
	camera->DrawDebugCascadeShadow();

	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->DrawDebug();
	}

	// デバッグオブジェクト
	for (GameObject* gameObject : m_GameObject[4])
	{
		gameObject->DrawDebug();
	}


	// アニメーション--------------------------------------------------------------
	Renderer::BeginDebugAnimationView();
	camera->DrawDebugAnimation();

	// デバッグオブジェクト
	for (GameObject* gameObject : m_GameObject[5])
	{
		gameObject->DrawDebug();
	}



	// Z pre-pass-------------------------------------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginZPrePass();
	camera->Draw();

	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->DrawZPrePass();
	}


	GUI::EndProfiling(ZPREPASS_DIVISION);



	// G-Bufferに情報出力-----------------------------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginGBuffer();

	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->Draw();
	}


	Renderer::SetRasterizerStateCullBack();
	GUI::EndProfiling(GBUFFER_DIVISION);

	// ディレクショナルライト用ライティング-----------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginDirectionalLighting();
	GetGameObject<DirectionalLight>()->DrawLighting();
	//m_Polygon2D->DrawDirectionalLighting();


	// ローカルライト用ライティング-------------------------------------------------
	Renderer::BeginLocalLighting();

	PointLight* pointLight = GetGameObject<PointLight>();
	SpotLight* spotLight = GetGameObject<SpotLight>();
	pointLight->DrawLighting();
	spotLight->DrawLighting();

	GUI::EndProfiling(LIGHTING_DIVISION);


	// ポストプロセスパス
	GUI::BeginProfiling();
	// アンチエイリアシング--------------------------------------------------------
	Renderer::BeginAntiAliasing();
	m_PostProcess->DrawAntiAliasing();


	// 被写界深度-------------------------------------------------------------------
	Renderer::BeginDepthOfField();
	m_PostProcess->DrawDepthOfField();


	// フォグ-----------------------------------------------------------------------
	Renderer::BeginFog();
	m_PostProcess->DrawFog();


	// 輝度抽出---------------------------------------------------------------------
	Renderer::BeginLuminance();
	Renderer::SetVariableViewport(0);
	Renderer::SetSamplerStateCLAMP();
	m_PostProcess->DrawLuminance();


	// ブルーム-----------------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		Renderer::BeginBlooms(i);
		Renderer::SetVariableViewport(i);
		m_PostProcess->DrawBlooms(i);
	}


	// 最終ブルーム----------------------------------------------------------------
	Renderer::BeginBloom();
	Renderer::SetDefaultViewport();
	m_PostProcess->DrawBloom();
	Renderer::SetSamplerStateWRAP();

	GUI::EndProfiling(POSTPROCESS_DIVISION);


	// UI--------------------------------------------------------------------------
	Renderer::SetDepthEnable(false);
	Renderer::Begin();
	m_Polygon2D->Draw();
	for (GameObject* gameObject : m_GameObject[3])
	{
		gameObject->Draw();
	}


	// 画面出力--------------------------------------------------------------------
	Renderer::DrawBackBuffer();
	Renderer::SetDepthEnable(true);


	// GUI描画---------------------------------------------------------------------
	GUI::ImGuiRender();

	Renderer::End();
}


void Title::InitTitleStage()
{
	Scene* scene = Manager::GetScene();

	Ground::CreateGround(scene, 2, 1, D3DXVECTOR3(-20.0f, 3.0f, 0.0f));
	Grass::CreateGrass(scene, 100, 2, 1, D3DXVECTOR3(-20.0f, 3.0f, 0.0f), D3DXVECTOR4(3.0f, 3.0f, 3.0f, 3.0f), 0.0f);

	Ground::CreateGround(scene, 1, 1, D3DXVECTOR3(40.0f, 5.0f, 15.0f));
	Grass::CreateGrass(scene, 100, 1, 1, D3DXVECTOR3(40.0f, 5.0f, 15.0f), D3DXVECTOR4(3.0f, 3.0f, 3.0f, 3.0f), 0.0f);

	{
		Tree* tree = scene->AddGameObject<Tree>(1);
		tree->SetPosition(D3DXVECTOR3(22.0f, 5.0f, 33.0f));
	}
	{
		Tree* tree = scene->AddGameObject<Tree>(1);
		tree->SetPosition(D3DXVECTOR3(-8.0f, 3.0f, 14.0f));
	}
	{
		Tree* tree = scene->AddGameObject<Tree>(1);
		tree->SetPosition(D3DXVECTOR3(-15.0f, 3.0f, 7.0f));
	}

	// エネミー
	{
		Enemy* enemy = scene->AddGameObject<Enemy>(1);
		enemy->SetPosition(D3DXVECTOR3(26.0f, 6.0f, 25.0f));
		enemy->SetRotation(D3DXVECTOR3(0.0f, 15.6f, 0.0f));
	}
	{
		Enemy* enemy = scene->AddGameObject<Enemy>(1);
		enemy->SetPosition(D3DXVECTOR3(30.0f, 6.0f, 24.0f));
		enemy->SetRotation(D3DXVECTOR3(0.0f, 17.0f, 0.0f));
	}
}
