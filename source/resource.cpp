#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"

Model* Resource::m_SphereModel = NULL;
Model* Resource::m_TorusModel = NULL;
Model* Resource::m_PlayerModel = NULL;
Model* Resource::m_EnemyNormalModel = NULL;
Model* Resource::m_EnemyMagicModel = NULL;
Model* Resource::m_EnemyStoneModel = NULL;
Model* Resource::m_SkydomeModel = NULL;
Model* Resource::m_EnemyParticleModel = NULL;
Model* Resource::m_RockParticleModel = NULL;
Model* Resource::m_PlayerParticleModel = NULL;
Model* Resource::m_BulletModel = NULL;
Model* Resource::m_RockModel = NULL;
Model* Resource::m_TreeModel[3] = { NULL };
Model* Resource::m_GroundModel = NULL;
Model* Resource::m_SnakeHeadModel = NULL;
Model* Resource::m_SnakeBodyModel = NULL;
Model* Resource::m_ArrowModel = NULL;

ID3D11ShaderResourceView* Resource::m_FieldTexture          = NULL;
ID3D11ShaderResourceView* Resource::m_EnvironmentMapTexture = NULL;
ID3D11ShaderResourceView* Resource::m_RedGradationTexture   = NULL;
ID3D11ShaderResourceView* Resource::m_GridTexture           = NULL;
ID3D11ShaderResourceView* Resource::m_FadeTexture           = NULL;
ID3D11ShaderResourceView* Resource::m_GrassTexture          = NULL;
ID3D11ShaderResourceView* Resource::m_JumpTutorialTexture         = NULL;
ID3D11ShaderResourceView* Resource::m_DashTutorialTexture         = NULL;
ID3D11ShaderResourceView* Resource::m_PressAttackTutorialTexture  = NULL;
ID3D11ShaderResourceView* Resource::m_ChargeAttackTutorialTexture = NULL;
ID3D11ShaderResourceView* Resource::m_TitleTexture          = NULL;
ID3D11ShaderResourceView* Resource::m_TitleSelectTexture    = NULL;
ID3D11ShaderResourceView* Resource::m_EndTexture            = NULL;
ID3D11ShaderResourceView* Resource::m_DeathTexture          = NULL;
ID3D11ShaderResourceView* Resource::m_KenshouTexture        = NULL;
ID3D11ShaderResourceView* Resource::m_TLOBTitleTexture      = NULL;
ID3D11ShaderResourceView* Resource::m_TCFTitleTexture       = NULL;
ID3D11ShaderResourceView* Resource::m_TSITitleTexture       = NULL;

ID3D11InputLayout*	 Resource::m_VertexLayout          = NULL;
ID3D11InputLayout*   Resource::m_GrassVertexLayout     = NULL;
ID3D11VertexShader*	 Resource::m_ColorVS               = NULL;
ID3D11PixelShader*	 Resource::m_ColorPS               = NULL;
ID3D11VertexShader*	 Resource::m_UnlitTextureVS        = NULL;
ID3D11PixelShader*	 Resource::m_UnlitTexturePS        = NULL;
ID3D11VertexShader*	 Resource::m_DeferredGBufferVS     = NULL;
ID3D11PixelShader*	 Resource::m_DeferredGBufferPS     = NULL;
ID3D11PixelShader*	 Resource::m_DeferredGBufferPlayerPS        = NULL;
ID3D11PixelShader*	 Resource::m_DeferredGBufferColoredObjectPS = NULL;
ID3D11VertexShader*	 Resource::m_LightObjectGBufferVS  = NULL;
ID3D11PixelShader*	 Resource::m_LightObjectGBufferPS  = NULL;
ID3D11VertexShader*  Resource::m_GrassVS               = NULL;
ID3D11GeometryShader* Resource::m_GrassGS              = NULL;
ID3D11PixelShader*   Resource::m_GrassPS               = NULL;
ID3D11VertexShader*	 Resource::m_DirectionalLightingVS = NULL;
ID3D11PixelShader*	 Resource::m_DirectionalLightingPS = NULL;
ID3D11VertexShader*	 Resource::m_LocalLightingVS       = NULL;
ID3D11PixelShader*	 Resource::m_LocalLightingPS       = NULL;
ID3D11VertexShader*	 Resource::m_DepthOfFieldVS        = NULL;
ID3D11PixelShader*	 Resource::m_DepthOfFieldPS        = NULL;
ID3D11VertexShader*	 Resource::m_FogVS                 = NULL;
ID3D11PixelShader*	 Resource::m_FogPS                 = NULL;
ID3D11VertexShader*	 Resource::m_LuminanceVS           = NULL;
ID3D11PixelShader*	 Resource::m_LuminancePS           = NULL;
ID3D11VertexShader*	 Resource::m_GaussianBlurVS        = NULL;
ID3D11PixelShader*	 Resource::m_GaussianBlurPS        = NULL;
ID3D11VertexShader*	 Resource::m_BloomVS               = NULL;
ID3D11PixelShader*	 Resource::m_BloomPS               = NULL;
ID3D11VertexShader*	 Resource::m_AntiAliasingFXAAVS    = NULL;
ID3D11PixelShader*	 Resource::m_AntiAliasingFXAAPS    = NULL;
ID3D11VertexShader*	 Resource::m_ShadowDebugVS         = NULL;
ID3D11PixelShader*	 Resource::m_ShadowDebugPS         = NULL;
ID3D11VertexShader*	 Resource::m_InstancedVS           = NULL;
ID3D11VertexShader*	 Resource::m_InstancedDeferredGBufferVS = NULL;
ID3D11ComputeShader* Resource::m_InstancedFrustumCullingCS  = NULL;
ID3D11VertexShader*  Resource::m_ReflectionVS = NULL;
ID3D11PixelShader*   Resource::m_ReflectionPS = NULL;
ID3D11VertexShader*  Resource::m_GaugeVS = NULL;
ID3D11PixelShader*   Resource::m_GaugePS = NULL;

Audio* Resource::m_MainThemeBGM  = NULL;
Audio* Resource::m_SubThemeBGM   = NULL;
Audio* Resource::m_WalkSE        = NULL;
Audio* Resource::m_JumpSE        = NULL;
Audio* Resource::m_DodgeSE       = NULL;
Audio* Resource::m_HealSE        = NULL;
Audio* Resource::m_BreakSE       = NULL;
Audio* Resource::m_PressSE       = NULL;
Audio* Resource::m_ChargeSE      = NULL;
Audio* Resource::m_ChargeHeavySE = NULL;
Audio* Resource::m_ChargeLightSE = NULL;
Audio* Resource::m_EnemyAttackSE = NULL;
Audio* Resource::m_EnemyMagicSE  = NULL;


void Resource::Init()
{
	// モデル読み込み
	m_SphereModel = new Model();
	m_SphereModel->Load("asset\\model\\sphere.obj");
	m_TorusModel = new Model();
	m_TorusModel->Load("asset\\model\\torus.obj");
	m_PlayerModel = new Model();
	m_PlayerModel->Load("asset\\model\\player\\player.obj");
	m_EnemyNormalModel = new Model();
	m_EnemyNormalModel->Load("asset\\model\\enemy\\normal\\enemy.obj");
	m_EnemyMagicModel = new Model();
	m_EnemyMagicModel->Load("asset\\model\\enemy\\magic\\enemyMagic.obj");
	m_EnemyStoneModel = new Model();
	m_EnemyStoneModel->Load("asset\\model\\enemy\\stone\\enemyStone.obj");
	m_SkydomeModel = new Model();
	m_SkydomeModel->Load("asset\\model\\environment\\skydome\\skydome.obj");
	m_EnemyParticleModel = new Model();
	m_EnemyParticleModel->Load("asset\\model\\particle\\enemyParticle.obj", false);
	m_RockParticleModel = new Model();
	m_RockParticleModel->Load("asset\\model\\particle\\rockParticle.obj", false);
	m_PlayerParticleModel = new Model();
	m_PlayerParticleModel->Load("asset\\model\\particle\\particle.obj", false);
	m_BulletModel = new Model();
	m_BulletModel->Load("asset\\model\\bullet\\bullet.obj", false);
	m_RockModel = new Model();
	m_RockModel->Load("asset\\model\\environment\\rock\\rock.obj", false);
	m_TreeModel[0] = new Model();
	m_TreeModel[0]->Load("asset\\model\\environment\\tree\\tree.obj", false);
	m_TreeModel[1] = new Model();
	m_TreeModel[1]->Load("asset\\model\\environment\\tree\\tree02.obj", false);
	m_TreeModel[2] = new Model();
	m_TreeModel[2]->Load("asset\\model\\environment\\tree\\tree03.obj", false);
	m_GroundModel = new Model();
	m_GroundModel->Load("asset\\model\\environment\\ground\\field.obj");
	m_SnakeHeadModel = new Model();
	m_SnakeHeadModel->Load("asset\\model\\enemy\\snakeHead\\snake_head.obj");
	m_SnakeBodyModel = new Model();
	m_SnakeBodyModel->Load("asset\\model\\enemy\\snakeBody\\snake_body.obj");
	m_ArrowModel = new Model();
	m_ArrowModel->Load("asset\\model\\arrow\\arrow.obj", false);

	// テクスチャ読み込み
	Resource::LoadTexture("asset/texture/field004.jpg", &m_FieldTexture);
	Resource::LoadTexture("asset/texture/envmap.png", &m_EnvironmentMapTexture);
	Resource::LoadTexture("asset/texture/redGradation.png", &m_RedGradationTexture);
	Resource::LoadTexture("asset/texture/grid.jpg", &m_GridTexture);
	Resource::LoadTexture("asset/texture/fade.png", &m_FadeTexture);
	Resource::LoadTexture("asset/texture/baseGrass.png", &m_GrassTexture);
	Resource::LoadTexture("asset/texture/tutorial/tutorialJump.png", &m_JumpTutorialTexture);
	Resource::LoadTexture("asset/texture/tutorial/tutorialDash.png", &m_DashTutorialTexture);
	Resource::LoadTexture("asset/texture/tutorial/tutorialPressAttack.png", &m_PressAttackTutorialTexture);
	Resource::LoadTexture("asset/texture/tutorial/tutorialChargeAttack.png", &m_ChargeAttackTutorialTexture);
	Resource::LoadTexture("asset/texture/titleBoard/title.png", &m_TitleTexture);
	Resource::LoadTexture("asset/texture/titleBoard/selectMode.png", &m_TitleSelectTexture);
	Resource::LoadTexture("asset/texture/titleBoard/end.png", &m_EndTexture);
	Resource::LoadTexture("asset/texture/titleBoard/death.png", &m_DeathTexture);
	Resource::LoadTexture("asset/texture/titleBoard/kenshou.png", &m_KenshouTexture);
	Resource::LoadTexture("asset/texture/titleBoard/TLOB_title.png", &m_TLOBTitleTexture);
	Resource::LoadTexture("asset/texture/titleBoard/TCF_title.png", &m_TCFTitleTexture);
	Resource::LoadTexture("asset/texture/titleBoard/TSI_title.png", &m_TSITitleTexture);

	// シェーダ読み込み
	Renderer::CreateVertexShader(&m_ColorVS, &m_VertexLayout, "shader\\colorVS.cso");
	Renderer::CreatePixelShader(&m_ColorPS, "shader\\colorPS.cso");
	Renderer::CreateVertexShader(&m_UnlitTextureVS, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_UnlitTexturePS, "shader\\unlitTexturePS.cso");
	Renderer::CreateVertexShader(&m_DeferredGBufferVS, &m_VertexLayout, "shader\\deferredGBufferVS.cso");
	Renderer::CreatePixelShader(&m_DeferredGBufferPS, "shader\\deferredGBufferPS.cso");
	Renderer::CreatePixelShader(&m_DeferredGBufferPlayerPS, "shader\\deferredGBufferPlayerPS.cso");
	Renderer::CreatePixelShader(&m_DeferredGBufferColoredObjectPS, "shader\\deferredGBufferColoredObjectPS.cso");
	Renderer::CreateVertexShader(&m_LightObjectGBufferVS, &m_VertexLayout, "shader\\PLGBufferVS.cso");
	Renderer::CreatePixelShader(&m_LightObjectGBufferPS, "shader\\PLGBufferPS.cso");
	Renderer::CreateVertexShader(&m_GrassVS, &m_GrassVertexLayout, "shader\\grassVS.cso");
	Renderer::CreateGeometryShader(&m_GrassGS, "shader\\grassGS.cso");
	Renderer::CreatePixelShader(&m_GrassPS, "shader\\grassPS.cso");
	Renderer::CreateVertexShader(&m_DirectionalLightingVS, &m_VertexLayout, "shader\\directionalLightingVS.cso");
	Renderer::CreatePixelShader(&m_DirectionalLightingPS, "shader\\directionalLightingPS.cso");
	Renderer::CreateVertexShader(&m_LocalLightingVS, &m_VertexLayout, "shader\\localLightingVS.cso");
	Renderer::CreatePixelShader(&m_LocalLightingPS, "shader\\localLightingPS.cso");
	Renderer::CreateVertexShader(&m_DepthOfFieldVS, &m_VertexLayout, "shader\\depthOfFieldVS.cso");
	Renderer::CreatePixelShader(&m_DepthOfFieldPS, "shader\\depthOfFieldPS.cso");
	Renderer::CreateVertexShader(&m_FogVS, &m_VertexLayout, "shader\\fogVS.cso");
	Renderer::CreatePixelShader(&m_FogPS, "shader\\fogPS.cso");
	Renderer::CreateVertexShader(&m_LuminanceVS, &m_VertexLayout, "shader\\luminanceVS.cso");
	Renderer::CreatePixelShader(&m_LuminancePS, "shader\\luminancePS.cso");
	Renderer::CreateVertexShader(&m_GaussianBlurVS, &m_VertexLayout, "shader\\gaussianBlurVS.cso");
	Renderer::CreatePixelShader(&m_GaussianBlurPS, "shader\\gaussianBlurPS.cso");
	Renderer::CreateVertexShader(&m_BloomVS, &m_VertexLayout, "shader\\bloomVS.cso");
	Renderer::CreatePixelShader(&m_BloomPS, "shader\\bloomPS.cso");
	Renderer::CreateVertexShader(&m_AntiAliasingFXAAVS, &m_VertexLayout, "shader\\antiAliasingFXAAVS.cso");
	Renderer::CreatePixelShader(&m_AntiAliasingFXAAPS, "shader\\antiAliasingFXAAPS.cso");
	Renderer::CreateVertexShader(&m_ShadowDebugVS, &m_VertexLayout, "shader\\shadowDebugVS.cso");
	Renderer::CreatePixelShader(&m_ShadowDebugPS, "shader\\shadowDebugPS.cso");
	Renderer::CreateVertexShader(&m_InstancedVS, &m_VertexLayout, "shader\\InstancedVS.cso");
	Renderer::CreateVertexShader(&m_InstancedDeferredGBufferVS, &m_VertexLayout, "shader\\InstancedDeferredGBufferVS.cso");
	Renderer::CreateComputeShader(&m_InstancedFrustumCullingCS, "shader\\InstancedFrustumCullingCS.cso");
	Renderer::CreateVertexShader(&m_ReflectionVS, &m_VertexLayout, "shader\\reflectionVS.cso");
	Renderer::CreatePixelShader(&m_ReflectionPS, "shader\\reflectionPS.cso");
	Renderer::CreateVertexShader(&m_GaugeVS, &m_VertexLayout, "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(&m_GaugePS, "shader\\gaugePS.cso");

	// オーディオ読み込み
	Audio::InitMaster();
	m_MainThemeBGM = new Audio;
	m_MainThemeBGM->Load("asset\\audio\\bgm\\bgm.wav");
	m_SubThemeBGM = new Audio;
	m_SubThemeBGM->Load("asset\\audio\\bgm\\game_bgm.wav");
	m_WalkSE = new Audio;
	m_WalkSE->Load("asset\\audio\\se\\move.wav");
	m_JumpSE = new Audio;
	m_JumpSE->Load("asset\\audio\\se\\jump.wav");
	m_DodgeSE = new Audio;
	m_DodgeSE->Load("asset\\audio\\se\\dodge.wav");
	m_HealSE = new Audio;
	m_HealSE->Load("asset\\audio\\se\\heal.wav");
	m_BreakSE = new Audio;
	m_BreakSE->Load("asset\\audio\\se\\rock_break.wav");
	m_PressSE = new Audio;
	m_PressSE->Load("asset\\audio\\se\\player_press.wav");
	m_ChargeSE = new Audio;
	m_ChargeSE->Load("asset\\audio\\se\\player_charge.wav");
	m_ChargeHeavySE = new Audio;
	m_ChargeHeavySE->Load("asset\\audio\\se\\player_charge_heavy.wav");
	m_ChargeLightSE = new Audio;
	m_ChargeLightSE->Load("asset\\audio\\se\\player_charge_light.wav");
	m_EnemyAttackSE = new Audio;
	m_EnemyAttackSE->Load("asset\\audio\\se\\enemy_attack.wav");
	m_EnemyMagicSE = new Audio;
	m_EnemyMagicSE->Load("asset\\audio\\se\\enemy_magic.wav");
}

void Resource::Uninit()
{
	m_SphereModel->Unload();
	delete m_SphereModel;
	m_TorusModel->Unload();
	delete m_TorusModel;
	m_PlayerModel->Unload();
	delete m_PlayerModel;
	m_EnemyNormalModel->Unload();
	delete m_EnemyNormalModel;
	m_EnemyMagicModel->Unload();
	delete m_EnemyMagicModel;
	m_EnemyStoneModel->Unload();
	delete m_EnemyStoneModel;
	m_SkydomeModel->Unload();
	delete m_SkydomeModel;
	m_EnemyParticleModel->Unload();
	delete m_EnemyParticleModel;
	m_RockParticleModel->Unload();
	delete m_RockParticleModel;
	m_PlayerParticleModel->Unload();
	delete m_PlayerParticleModel;
	m_BulletModel->Unload();
	delete m_BulletModel;
	m_RockModel->Unload();
	delete m_RockModel;
	m_TreeModel[0]->Unload();
	delete m_TreeModel[0];
	m_TreeModel[1]->Unload();
	delete m_TreeModel[1];
	m_TreeModel[2]->Unload();
	delete m_TreeModel[2];
	m_GroundModel->Unload();
	delete m_GroundModel;
	m_SnakeHeadModel->Unload();
	delete m_SnakeHeadModel;
	m_SnakeBodyModel->Unload();
	delete m_SnakeBodyModel;
	m_ArrowModel->Unload();
	delete m_ArrowModel;

	m_FieldTexture->Release();
	m_EnvironmentMapTexture->Release();
	m_RedGradationTexture->Release();
	m_GridTexture->Release();
	m_FadeTexture->Release();
	m_GrassTexture->Release();
	m_JumpTutorialTexture->Release();
	m_DashTutorialTexture->Release();
	m_ChargeAttackTutorialTexture->Release();
	m_PressAttackTutorialTexture->Release();
	m_TitleSelectTexture->Release();
	m_TitleTexture->Release();
	m_EndTexture->Release();
	m_DeathTexture->Release();
	m_KenshouTexture->Release();
	m_TLOBTitleTexture->Release();
	m_TCFTitleTexture->Release();
	m_TSITitleTexture->Release();

	m_VertexLayout->Release();
	m_GrassVertexLayout->Release();
	m_ColorVS->Release();
	m_ColorPS->Release();
	m_UnlitTextureVS->Release();
	m_UnlitTexturePS->Release();
	m_DeferredGBufferVS->Release();
	m_DeferredGBufferPS->Release();
	m_DeferredGBufferPlayerPS->Release();
	m_DeferredGBufferColoredObjectPS->Release();
	m_LightObjectGBufferVS->Release();
	m_LightObjectGBufferPS->Release();
	m_GrassVS->Release();
	m_GrassGS->Release();
	m_GrassPS->Release();
	m_DirectionalLightingVS->Release();
	m_DirectionalLightingPS->Release();
	m_LocalLightingVS->Release();
	m_LocalLightingPS->Release();
	m_DepthOfFieldVS->Release();
	m_DepthOfFieldPS->Release();
	m_FogVS->Release();
	m_FogPS->Release();
	m_LuminanceVS->Release();
	m_LuminancePS->Release();
	m_GaussianBlurVS->Release();
	m_GaussianBlurPS->Release();
	m_BloomVS->Release();
	m_BloomPS->Release();
	m_AntiAliasingFXAAVS->Release();
	m_AntiAliasingFXAAPS->Release();
	m_ShadowDebugVS->Release();
	m_ShadowDebugPS->Release();
	m_InstancedVS->Release();
	m_InstancedDeferredGBufferVS->Release();
	m_InstancedFrustumCullingCS->Release();
	m_ReflectionVS->Release();
	m_ReflectionPS->Release();
	m_GaugeVS->Release();
	m_GaugePS->Release();

	m_MainThemeBGM->Uninit();
	m_SubThemeBGM->Uninit();
	m_WalkSE->Uninit();
	m_JumpSE->Uninit();
	m_DodgeSE->Uninit();
	m_HealSE->Uninit();
	m_BreakSE->Uninit();
	m_PressSE->Uninit();
	m_ChargeSE->Uninit();
	m_ChargeHeavySE->Uninit();
	m_ChargeLightSE->Uninit();
	m_EnemyAttackSE->Uninit();
	m_EnemyMagicSE->Uninit();

	Audio::UninitMaster();
}



