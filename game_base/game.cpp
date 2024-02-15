#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dataStorage.h"
#include "game.h"
#include "title.h"
#include "postProcess.h"

#include "TheLandOfBeginningsArea.h"
#include "TheCrystalFountainheadArea.h"
#include "TheSkyIslandArea.h"
#include "camera.h"
#include "player.h"
#include "field.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "polygon.h"
#include "skydome.h"
#include "grass.h"
#include "rock.h"
#include "tree.h"
#include "water.h"
#include "ground.h"
#include "eventManager.h"
#include "invisibleBox.h"
#include "playerUI.h"
#include "fade.h"
#include "random.h"

#include "debugObject.h"
#include "animationObject.h"


void Game::Init()
{
	// 初期設定
	m_SceneName = GAME_SCENE;
	GUI::drawGuiFlag = false;
	GUI::playerControllFlag = true;
	GUI::drawLocalLightFlag = false;
	GUI::luminanceValue = 0.825f;

	// システムオブジェクト
	AddGameObject<Camera>(0);
	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFadeOut();

	// ポストプロセス
	m_PostProcess = new PostProcess();
	m_PostProcess->Init();
	m_Polygon2D = new Polygon2D();
	m_Polygon2D->Init();

	// プレイヤー
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	AddGameObject<PlayerUI>(3);

	// ライト
	AddGameObject<DirectionalLight>(1);
	AddGameObject<PointLight>(1);
	AddGameObject<SpotLight>(1);

	// 環境オブジェクト
	AddGameObject<Sky>(1);
	TheLandOfBeginnings::InitEnvironment();
	TheCrystalFountainhead::InitEnvironment();
	TheSkyIsland::InitEnvironment();
	Water::CreateWave();
	AddGameObject<Water>(1)->CreateWater(true);

	// エネミー
	TheLandOfBeginnings::InitEnemy();
	TheCrystalFountainhead::InitEnemy();
	TheSkyIsland::InitEnemy();

	// デバッグ
	for (int i = 0; i < 7; i++)
	{
		AddGameObject<DebugObject>(4);
	}
	AddGameObject<Field>(4);
	AddGameObject<AnimationObject>(5);
}

void Game::Update()
{
	Scene::Update();

	// ポストプロセス
	m_PostProcess->Update();

	// シーン終了設定
	if (GUI::sceneEndFlag) m_Fade->SetFadeIn();

	// リザルトシーン推移
	if (m_Fade->GetFadeFinished())
	{
		Manager::SetScene<Title>();
		GUI::sceneEndFlag = false;
	}
}

void Game::Draw()
{
	Camera* camera = GetGameObject<Camera>();


	//********************************デファードレンダリング********************************

	Renderer::SetSamplerStateWRAP();
	GUI::BeginProfiling();

	// ディレクショナルライトのシャドウマップ生成-----------------------------------


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
