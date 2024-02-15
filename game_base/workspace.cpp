#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "workspace.h"
#include "title.h"
#include "postProcess.h"

#include "camera.h"
#include "player.h"
#include "field.h"
#include "pbrObject.h"
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
#include "fade.h"
#include "playerUI.h"

#include "debugObject.h"
#include "animationObject.h"
#include "debugInstancingObject.h"


void WorkSpace::Init()
{
	// �����ݒ�
	m_SceneName = WORFSPACE_SCENE;
	GUI::drawGuiFlag = true;
	GUI::playerControllFlag = true;
	GUI::drawLocalLightFlag = false;
	GUI::luminanceValue = 0.975f;

	// �V�X�e���I�u�W�F�N�g
	AddGameObject<Camera>(0);
	m_Fade = AddGameObject<Fade>(3);
	m_Fade->SetFadeOut();

	// �|�X�g�v���Z�X
	m_PostProcess = new PostProcess();
	m_PostProcess->Init();
	m_Polygon2D = new Polygon2D();
	m_Polygon2D->Init();

	// �v���C���[
	Player* player = AddGameObject<Player>(1);
	AddGameObject<PlayerUI>(3);

	// ���I�u�W�F�N�g
	AddGameObject<DirectionalLight>(1);
	AddGameObject<PointLight>(1);
	AddGameObject<SpotLight>(1);
	AddGameObject<Sky>(1);
	AddGameObject<Rock>(1)->SetPosition(D3DXVECTOR3(5.0f, 1.0f, 50.0f));
	AddGameObject<Tree>(1)->SetPosition(D3DXVECTOR3(-5.0f, 1.0f, 50.0f));
	Grass::CreateGrass(this, 300, 1, 1, D3DXVECTOR3(-30.0f, 0.0f, 60.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), 0.0f);
	Ground::CreateGround(this, 1, 1, D3DXVECTOR3(30.0f, 2.0f, 60.0f));

	int num = 10;
	for (int y = 0; y < num; y++)
	{
		for (int x = 0; x < num; x++)
		{
			AddGameObject<PBRObject>(1)->Init(D3DXVECTOR3(-22.5f + 4.5f * x, 4.0f, -22.5f + 4.5f * y), 0.1f * y + 0.001f, 0.1f * x + 0.001f);
		}
	}


	// �f�o�b�O
	for (int i = 0; i < 7; i++)
	{
		AddGameObject<DebugObject>(4);
	}
	AddGameObject<Field>(1);
	AddGameObject<AnimationObject>(5);
	//AddGameObject<DebugInstancingObject>(1);
}

void WorkSpace::Update()
{
	Scene::Update();

	// �|�X�g�v���Z�X
	m_PostProcess->Update();

	// �V�[���I���ݒ�
	if (GUI::sceneEndFlag) m_Fade->SetFadeIn();

	// ���U���g�V�[������
	if (m_Fade->GetFadeFinished())
	{
		Manager::SetScene<Title>();
		GUI::sceneEndFlag = false;
	}
}

void WorkSpace::Draw()
{
	Camera* camera = GetGameObject<Camera>();


	//********************************�f�t�@�[�h�����_�����O********************************

	Renderer::SetSamplerStateWRAP();
	GUI::BeginProfiling();

	// �f�B���N�V���i�����C�g�̃V���h�E�}�b�v����-----------------------------------


	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		Renderer::BeginShadowMap(i);

		// �r���[�|�[�g�ݒ�
		if (i == 0)	Renderer::SetWideViewport();         // 2�{
		else if (i == 1) Renderer::SetDefaultViewport(); // 1�{
		else Renderer::SetVariableViewport(0);           // 0.5�{


		camera->DrawCascadeShadowMap(i);
		for (GameObject* gameObject : m_GameObject[1])
		{
			gameObject->DrawShadowMapping();
		}
	}



	GUI::EndProfiling(SHADOWMAP_DIVISION);
	Renderer::SetDefaultViewport();


	// ���ʔ��˗p�e�N�X�`������-----------------------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginReflection();

	camera->DrawReflection();
	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->DrawReflection();
	}

	GUI::EndProfiling(REFLECTION_DIVISION);


	// �f�o�b�O--------------------------------------------------------------------
	// �J�X�P�[�h�V���h�E----------------------------------------------------------
	Renderer::BeginDebugCascadeShadowView();
	camera->DrawDebugCascadeShadow();

	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->DrawDebug();
	}

	// �f�o�b�O�I�u�W�F�N�g
	for (GameObject* gameObject : m_GameObject[4])
	{
		gameObject->DrawDebug();
	}


	// �A�j���[�V����--------------------------------------------------------------
	Renderer::BeginDebugAnimationView();
	camera->DrawDebugAnimation();

	// �f�o�b�O�I�u�W�F�N�g
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



	// G-Buffer�ɏ��o��-----------------------------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginGBuffer();

	for (GameObject* gameObject : m_GameObject[1])
	{
		gameObject->Draw();
	}

	Renderer::SetRasterizerStateCullBack();
	GUI::EndProfiling(GBUFFER_DIVISION);


	// �f�B���N�V���i�����C�g�p���C�e�B���O-----------------------------------------
	GUI::BeginProfiling();
	Renderer::BeginDirectionalLighting();
	GetGameObject<DirectionalLight>()->DrawLighting();


	// ���[�J�����C�g�p���C�e�B���O-------------------------------------------------
	Renderer::BeginLocalLighting();

	PointLight* pointLight = GetGameObject<PointLight>();
	SpotLight* spotLight = GetGameObject<SpotLight>();
	pointLight->DrawLighting();
	spotLight->DrawLighting();

	GUI::EndProfiling(LIGHTING_DIVISION);

	// �|�X�g�v���Z�X�p�X
	GUI::BeginProfiling();
	// ��ʊE�[�x-------------------------------------------------------------------
	Renderer::BeginDepthOfField();
	m_PostProcess->DrawDepthOfField();


	// �t�H�O-----------------------------------------------------------------------
	Renderer::BeginFog();
	m_PostProcess->DrawFog();


	// �P�x���o---------------------------------------------------------------------
	Renderer::BeginLuminance();
	Renderer::SetVariableViewport(0);
	Renderer::SetSamplerStateCLAMP();
	m_PostProcess->DrawLuminance();


	// �u���[��-----------------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		Renderer::BeginBlooms(i);
		Renderer::SetVariableViewport(i);
		m_PostProcess->DrawBlooms(i);
	}


	// �ŏI�u���[��----------------------------------------------------------------
	Renderer::BeginBloom();
	Renderer::SetDefaultViewport();
	m_PostProcess->DrawBloom();
	Renderer::SetSamplerStateWRAP();


	// �A���`�G�C���A�V���O--------------------------------------------------------
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


	// ��ʏo��--------------------------------------------------------------------
	Renderer::DrawBackBuffer();
	Renderer::SetDepthEnable(true);


	// GUI�`��---------------------------------------------------------------------
	GUI::ImGuiRender();

	Renderer::End();
}
