#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dataStorage.h"
#include "tutorial.h"
#include "title.h"
#include "game.h"
#include "postProcess.h"

#include "camera.h"
#include "player.h"
#include "enemy.h"
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
#include "arrow.h"
#include "eventManager.h"
#include "invisibleBox.h"
#include "playerUI.h"
#include "fade.h"
#include "random.h"

#include "debugObject.h"
#include "animationObject.h"
#include "debugInstancingObject.h"


void Tutorial::Init()
{
	// 初期設定
	m_SceneName = TUTORIAL_SCENE;
	GUI::drawGuiFlag = false;
	GUI::playerControllFlag = true;
	GUI::drawLocalLightFlag = false;

	// システムオブジェクト
	AddGameObject<Camera>(0);
	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFadeOut();
	EventManager::InitTutorialStage();

	// ポストプロセス
	m_PostProcess = new PostProcess();
	m_PostProcess->Init();
	m_Polygon2D = new Polygon2D();
	m_Polygon2D->Init();

	// プレイヤー
	Player* player = AddGameObject<Player>(1);
	AddGameObject<PlayerUI>(3);

	// ライト
	AddGameObject<DirectionalLight>(1);
	AddGameObject<PointLight>(1);
	AddGameObject<SpotLight>(1);

	// 環境オブジェクト
	AddGameObject<Sky>(1);

	// 地面と草原
	int groundListNum = DataStorage::GetTutorialDataStorage()->GroundGrassPos.size();
	for (int i = 0; i < groundListNum; i++)
	{
		Ground::CreateGround(this, 1, 1, DataStorage::GetTutorialDataStorage()->GroundGrassPos[i]);
		Grass::CreateGrass(this, 120, 1, 1, DataStorage::GetTutorialDataStorage()->GroundGrassPos[i], D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}

	// 岩
	int rockListNum = DataStorage::GetTutorialDataStorage()->RockPos.size();
	for (int i = 0; i < rockListNum; i++)
	{
		Rock* rock = AddGameObject<Rock>(1);
		rock->SetPosition(DataStorage::GetTutorialDataStorage()->RockPos[i]);
		if (i >= 2 && i < 11)
		{
			float randScale = Random(0, 100) / 50.0f - 0.5f;
			rock->SetScale(D3DXVECTOR3(4.5f + randScale, 4.5f + randScale, 4.5f + randScale));
		}
	}

	// 木
	int treeListNum = DataStorage::GetTutorialDataStorage()->TreePos.size();
	for (int i = 0; i < treeListNum; i++)
	{
		AddGameObject<Tree>(1)->SetPosition(DataStorage::GetTutorialDataStorage()->TreePos[i]);
	}

	// 矢印看板
	int arrowListNum = DataStorage::GetTutorialDataStorage()->ArrowPos.size();
	for (int i = 0; i < arrowListNum; i++)
	{
		Arrow* arrow = AddGameObject<Arrow>(1);
		arrow->SetPosition(DataStorage::GetTutorialDataStorage()->ArrowPos[i]);
		arrow->SetRotation(D3DXVECTOR3(0.0f, DataStorage::GetTutorialDataStorage()->ArrowRot[i].y * D3DX_PI / 180.0f, 0.0f));
	}

	// 水面
	Water::CreateWave();
	AddGameObject<Water>(1)->CreateWater(true);


	// エネミー
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(150.0f, 8.5f, 80.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(160.0f, 8.5f, 155.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(165.0f, 8.5f, 145.0f));


	// デバッグ
	for (int i = 0; i < 7; i++)
	{
		AddGameObject<DebugObject>(4);
	}
	AddGameObject<Field>(4);
	AddGameObject<AnimationObject>(5);
}

void Tutorial::Update()
{
	Scene::Update();

	// ポストプロセス
	m_PostProcess->Update();

	// シーン終了設定
	if (GUI::sceneEndFlag) m_Fade->SetFadeIn();

	// リザルトシーン推移
	if (m_Fade->GetFadeFinished())
	{
		Manager::SetScene<Game>();
		GUI::sceneEndFlag = false;
	}
}

void Tutorial::Draw()
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
	// アンチエイリアシング---------------------------------------------------------
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


	// ブルーム---------------------------------------------------------------------
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
