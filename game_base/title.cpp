#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "title.h"
#include "workspace.h"
#include "input.h"
#include "postProcess.h"

#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "fade.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "polygon.h"

void Title::Init()
{
	// 初期設定
	m_SceneName = TITLE_SCENE;

	// システムオブジェクト
	AddGameObject<Camera>(0);
	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFadeOut();

	// ポストプロセス
	m_PostProcess = new PostProcess();
	m_PostProcess->Init();
	m_Polygon2D = new Polygon2D();
	m_Polygon2D->Init();


	// 環境オブジェクト

	// プレイヤー
	Player* player = AddGameObject<Player>(1);

	AddGameObject<DirectionalLight>(1);
	AddGameObject<PointLight>(1);
	AddGameObject<SpotLight>(1);


	// Audio
	//m_BGM = polygon2D->AddComponent<Audio>();
	//m_BGM->Load("asset\\audio\\bgm\\bgm.wav");
	//m_BGM->Play();
	//m_SE = polygon2D->AddComponent<Audio>();
	//m_SE->Load("asset\\audio\\se\\move.wav");

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
		m_Fade->SetFadeIn();
		GUI::sceneEndFlag = false;
		//m_SE->Play();
	}
	if (m_Fade->GetFadeFinished() && m_NextScene)Manager::SetScene<Game>();
	if (Input::GetKeyPress('2'))
	{
		m_NextScene = false;
		m_Fade->SetFadeIn();
		GUI::sceneEndFlag = false;
		//m_SE->Play();
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


	// アンチエイリアシング--------------------------------------------------------
	Renderer::BeginAntiAliasing();
	m_PostProcess->DrawAntiAliasing();

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
