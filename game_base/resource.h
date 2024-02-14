#pragma once
#include "model.h"

// リソース管理用クラス
class Resource
{
private:
    // 3Dモデル
    static Model* m_SphereModel;
	static Model* m_TorusModel;
	static Model* m_PlayerModel;
	static Model* m_EnemyNormalModel;
	static Model* m_EnemyMagicModel;
	static Model* m_EnemyStoneModel;
	static Model* m_SkydomeModel;
	static Model* m_EnemyParticleModel;
	static Model* m_RockParticleModel;
	static Model* m_PlayerParticleModel;
	static Model* m_BulletModel;
	static Model* m_RockModel;
	static Model* m_TreeModel[3];
	static Model* m_GroundModel;
	static Model* m_SnakeHeadModel;
	static Model* m_SnakeBodyModel;

	// テクスチャ
	static ID3D11ShaderResourceView* m_FieldTexture;
	static ID3D11ShaderResourceView* m_EnvironmentMapTexture;
	static ID3D11ShaderResourceView* m_RedGradationTexture;
	static ID3D11ShaderResourceView* m_GridTexture;
	static ID3D11ShaderResourceView* m_FadeTexture;
	static ID3D11ShaderResourceView* m_GrassTexture;
	static ID3D11ShaderResourceView* m_JumpTutorialTexture;
	static ID3D11ShaderResourceView* m_DashTutorialTexture;
	static ID3D11ShaderResourceView* m_PressAttackTutorialTexture;
	static ID3D11ShaderResourceView* m_ChargeAttackTutorialTexture;
	static ID3D11ShaderResourceView* m_EndTexture;
	static ID3D11ShaderResourceView* m_DeathTexture;
	static ID3D11ShaderResourceView* m_KenshouTexture;
	static ID3D11ShaderResourceView* m_TLOBTitleTexture;
	static ID3D11ShaderResourceView* m_TCFTitleTexture;
	static ID3D11ShaderResourceView* m_TSITitleTexture;

	// シェーダ
	static ID3D11InputLayout*	m_VertexLayout;
	static ID3D11InputLayout*   m_GrassVertexLayout;
	static ID3D11VertexShader*	m_ColorVS;
	static ID3D11PixelShader*	m_ColorPS;
	static ID3D11VertexShader*	m_UnlitTextureVS;
	static ID3D11PixelShader*	m_UnlitTexturePS;
	static ID3D11VertexShader*	m_ShadowMappingVS;
	static ID3D11PixelShader*	m_ShadowMappingPS;
	static ID3D11VertexShader*	m_DeferredGBufferVS;
	static ID3D11PixelShader*	m_DeferredGBufferPS;
	static ID3D11PixelShader*	m_DeferredGBufferPlayerPS;
	static ID3D11PixelShader*	m_DeferredGBufferColoredObjectPS;
	static ID3D11VertexShader*   m_GrassVS;
	static ID3D11GeometryShader* m_GrassGS;
	static ID3D11PixelShader*    m_GrassPS;
	static ID3D11VertexShader*	m_LightObjectGBufferVS;
	static ID3D11PixelShader*	m_LightObjectGBufferPS;
	static ID3D11VertexShader*	m_DirectionalLightingVS;
	static ID3D11PixelShader*	m_DirectionalLightingPS;
	static ID3D11VertexShader*	m_LocalLightingVS;
	static ID3D11PixelShader*	m_LocalLightingPS;
	static ID3D11VertexShader*	m_DepthOfFieldVS;
	static ID3D11PixelShader*	m_DepthOfFieldPS;
	static ID3D11VertexShader*	m_FogVS;
	static ID3D11PixelShader*	m_FogPS;
	static ID3D11VertexShader*	m_LuminanceVS;
	static ID3D11PixelShader*	m_LuminancePS;
	static ID3D11VertexShader*	m_GaussianBlurVS;
	static ID3D11PixelShader*	m_GaussianBlurPS;
	static ID3D11VertexShader*	m_BloomVS;
	static ID3D11PixelShader*	m_BloomPS;
	static ID3D11VertexShader*	m_AntiAliasingFXAAVS;
	static ID3D11PixelShader*	m_AntiAliasingFXAAPS;
	static ID3D11VertexShader*	m_ShadowDebugVS;
	static ID3D11PixelShader*	m_ShadowDebugPS;
	static ID3D11VertexShader*	m_InstancedVS;
	static ID3D11VertexShader*	m_InstancedDeferredGBufferVS;
	static ID3D11ComputeShader*	m_InstancedFrustumCullingCS;
	static ID3D11VertexShader*  m_ReflectionVS;
	static ID3D11PixelShader*   m_ReflectionPS;
	static ID3D11VertexShader*  m_GaugeVS;
	static ID3D11PixelShader*   m_GaugePS;



public:
	static void Init();
	static void Uninit();


	// リソース取得用関数
	static Model* GetSphereModel() { return m_SphereModel; }
	static Model* GetTorusModel() { return m_TorusModel; }
	static Model* GetPlayerModel() { return m_PlayerModel; }
	static Model* GetEnemyNormalModel() { return m_EnemyNormalModel; }
	static Model* GetEnemyMagicModel() { return m_EnemyMagicModel; }
	static Model* GetEnemyStoneModel() { return m_EnemyStoneModel; }
	static Model* GetSkydomeModel() { return m_SkydomeModel; }
	static Model* GetEnemyParticleModel() { return m_EnemyParticleModel; }
	static Model* GetRockParticleModel() { return m_RockParticleModel; }
	static Model* GetPlayerParticleModel() { return m_PlayerParticleModel; }
	static Model* GetBulletModel() { return m_BulletModel; }
	static Model* GetRockModel() { return m_RockModel; }
	static Model* GetTreeModel(int i) { return m_TreeModel[i]; }
	static Model* GetGroundModel() { return m_GroundModel; }
	static Model* GetSnakeHeadModel() { return m_SnakeHeadModel; }
	static Model* GetSnakeBodyModel() { return m_SnakeBodyModel; }

	static ID3D11ShaderResourceView* const* GetFieldTexture() { return &m_FieldTexture; }
	static ID3D11ShaderResourceView* const* GetEnvironmentMapTexture() { return &m_EnvironmentMapTexture; }
	static ID3D11ShaderResourceView* const* GetRedGradationTexture() { return &m_RedGradationTexture; }
	static ID3D11ShaderResourceView* const* GetGridTexture() { return &m_GridTexture; }
	static ID3D11ShaderResourceView* const* GetFadeTexture() { return &m_FadeTexture; }
	static ID3D11ShaderResourceView* const* GetGrassTexture() { return &m_GrassTexture; }
	static ID3D11ShaderResourceView* const* GetJumpTutorialTexture() { return &m_JumpTutorialTexture; }
	static ID3D11ShaderResourceView* const* GetDashTutorialTexture() { return &m_DashTutorialTexture; }
	static ID3D11ShaderResourceView* const* GetPressAttackTutorialTexture() { return &m_PressAttackTutorialTexture; }
	static ID3D11ShaderResourceView* const* GetChargeAttackTutorialTexture() { return &m_ChargeAttackTutorialTexture; }
	static ID3D11ShaderResourceView* const* GetEndTexture() { return &m_EndTexture; }
	static ID3D11ShaderResourceView* const* GetDeathTexture() { return &m_DeathTexture; }
	static ID3D11ShaderResourceView* const* GetKenshouTexture() { return &m_KenshouTexture; }
	static ID3D11ShaderResourceView* const* GetTLOBTitleTexture() { return &m_TLOBTitleTexture; }
	static ID3D11ShaderResourceView* const* GetTCFTitleTexture() { return &m_TCFTitleTexture; }
	static ID3D11ShaderResourceView* const* GetTSITitleTexture() { return &m_TSITitleTexture; }

	static ID3D11InputLayout* GetVertexLayout() { return m_VertexLayout; }
	static ID3D11InputLayout* GetGrassVertexLayout() { return m_GrassVertexLayout; }
	static ID3D11VertexShader* GetColorVS() { return m_ColorVS; }
	static ID3D11PixelShader* GetColorPS() { return m_ColorPS; }
	static ID3D11VertexShader* GetUnlitTextureVS() { return m_UnlitTextureVS; }
	static ID3D11PixelShader* GetUnlitTexturePS() { return m_UnlitTexturePS; }
	static ID3D11VertexShader* GetShadowMappingVS() { return m_ShadowMappingVS; }
	static ID3D11PixelShader* GetShadowMappingPS() { return m_ShadowMappingPS; }
	static ID3D11VertexShader* GetDeferredGBufferVS() { return m_DeferredGBufferVS; }
	static ID3D11PixelShader* GetDeferredGBufferPS() { return m_DeferredGBufferPS; }
	static ID3D11PixelShader* GetDeferredGBufferPlayerPS() { return m_DeferredGBufferPlayerPS; }
	static ID3D11PixelShader* GetDeferredGBufferColoredObjectPS() { return m_DeferredGBufferColoredObjectPS; }
	static ID3D11VertexShader* GetLightObjectGBufferVS() { return m_LightObjectGBufferVS; }
	static ID3D11PixelShader* GetLightObjectGBufferPS() { return m_LightObjectGBufferPS; }
	static ID3D11VertexShader* GetGrassVS() { return m_GrassVS; }
	static ID3D11GeometryShader* GetGrassGS() { return m_GrassGS; }
	static ID3D11PixelShader* GetGrassPS() { return m_GrassPS; }
	static ID3D11VertexShader* GetDirectionalLightingVS() { return m_DirectionalLightingVS; }
	static ID3D11PixelShader* GetDirectionalLightingPS() { return m_DirectionalLightingPS; }
	static ID3D11VertexShader* GetLocalLightingVS() { return m_LocalLightingVS; }
	static ID3D11PixelShader* GetLocalLightingPS() { return m_LocalLightingPS; }
	static ID3D11VertexShader* GetDepthOfFieldVS() { return m_DepthOfFieldVS; }
	static ID3D11PixelShader* GetDepthOfFieldPS() { return m_DepthOfFieldPS; }
	static ID3D11VertexShader* GetFogVS() { return m_FogVS; }
	static ID3D11PixelShader* GetFogPS() { return m_FogPS; }
	static ID3D11VertexShader* GetLuminanceVS() { return m_LuminanceVS; }
	static ID3D11PixelShader* GetLuminancePS() { return m_LuminancePS; }
	static ID3D11VertexShader* GetGaussianBlurVS() { return m_GaussianBlurVS; }
	static ID3D11PixelShader* GetGaussianBlurPS() { return m_GaussianBlurPS; }
	static ID3D11VertexShader* GetBloomVS() { return m_BloomVS; }
	static ID3D11PixelShader* GetBloomPS() { return m_BloomPS; }
	static ID3D11VertexShader* GetAntiAliasingFXAAVS() { return m_AntiAliasingFXAAVS; }
	static ID3D11PixelShader* GetAntiAliasingFXAAPS() { return m_AntiAliasingFXAAPS; }
	static ID3D11VertexShader* GetShadowDebugVS() { return m_ShadowDebugVS; }
	static ID3D11PixelShader*  GetShadowDebugPS() { return m_ShadowDebugPS; }
	static ID3D11VertexShader* GetInstancedVS() { return m_InstancedVS; }
	static ID3D11VertexShader* GetInstancedDeferredGBufferVS() { return m_InstancedDeferredGBufferVS; }
	static ID3D11ComputeShader* GetInstancedFrustumCullingCS() { return m_InstancedFrustumCullingCS; }
	static ID3D11VertexShader* GetReflectionVS() { return m_ReflectionVS; }
	static ID3D11PixelShader*  GetReflectionPS() { return m_ReflectionPS; }
	static ID3D11VertexShader* GetGaugeVS() { return m_GaugeVS; }
	static ID3D11PixelShader*  GetGaugePS() { return m_GaugePS; }


	// テクスチャ読み込み用
	static void LoadTexture(const char FileName[256], ID3D11ShaderResourceView** Texture)
	{
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			FileName,
			NULL,
			NULL,
			Texture,
			NULL);
		assert(Texture);
	}
};