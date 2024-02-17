#pragma once
#include "gui.h"

#define SHADOWMAP_NUM 3 // シャドウマップの枚数


// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
	D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};


// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float       Shininess;
	float		Metalic;
	float       Roughness;
	BOOL		TextureEnable;
};


// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LIGHT
{
	LIGHT_TYPE  LightType;
	LIGHT_TYPE	Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient; // a : cascadeShadow
	D3DXMATRIX  ViewMatrix;
	D3DXMATRIX  ProjectionMatrix;
};

struct CAMERA
{
	D3DXVECTOR4 Position;
	D3DXVECTOR4	FrustumPosition[4];
	D3DXCOLOR   Time;
	D3DXMATRIX  ReflectionMatrix;
};

struct POINTLIGHT
{
	D3DXVECTOR4	Position;
	D3DXVECTOR4	Attenuation;
	D3DXCOLOR   Color;
};

struct POINTLIGHTS
{
	POINTLIGHT PointLight[300];
	int        PointLightNum;
	int        Dummy[3];
};

struct SPOTLIGHT
{
	D3DXVECTOR4	Position;  // w : range
	D3DXVECTOR4	Direction; // w : radius
	D3DXVECTOR4	Attenuation;
	D3DXCOLOR   Color;
};

struct POSTPROCESSPARAMETER
{
	D3DXVECTOR4 color; // PBRObjectColor
	D3DXVECTOR4 level; // x : AA基準値
	D3DXVECTOR4	depth; // x : Back, y : Front, z : level
	int texWidth;
	int texHeight;
	int Dummy[2];
};

struct FOG
{
	D3DXVECTOR4 HeightColor; 
	D3DXVECTOR4 HeightLevel; // x : 基準高度, y : 減衰距離
	D3DXVECTOR4 DepthColor;  
	D3DXVECTOR4 DepthLevel;  // x : 基準距離, y : 減衰距離
};

struct PLAYERPARAMETER
{
	D3DXVECTOR4 Status; 
	D3DXVECTOR4 Material;
};

// 波用構造体
struct WAVE
{
	D3DXVECTOR2 dir;
	float amplitude;
	float waveLength;
};

// 合成波生成用構造体
struct WAVES
{
	WAVE Wave[20];
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;


class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;

	// バックバッファ
	static ID3D11RenderTargetView*   m_DefaultBackBufferRTV;
	static ID3D11DepthStencilView*   m_DefaultDepthDSV;

	// Reflection
	static ID3D11RenderTargetView*   m_ReflectionRTV;
	static ID3D11ShaderResourceView* m_ReflectionSRV;
	static ID3D11DepthStencilView*   m_ReflectionDSV;

	// GBuffer
	static ID3D11RenderTargetView*   m_AlbedoColorRTV;
	static ID3D11ShaderResourceView* m_AlbedoColorSRV;
	static ID3D11RenderTargetView*   m_PositionRTV;
	static ID3D11ShaderResourceView* m_PositionSRV;
	static ID3D11RenderTargetView*   m_NormalRTV;
	static ID3D11ShaderResourceView* m_NormalSRV;
	static ID3D11RenderTargetView*   m_MaterialRTV;
	static ID3D11ShaderResourceView* m_MaterialSRV;

	// Lighting
	static ID3D11RenderTargetView*   m_DirectionalLightingRTV;
	static ID3D11ShaderResourceView* m_DirectionalLightingSRV;
	static ID3D11RenderTargetView*   m_LocalLightingRTV;
	static ID3D11ShaderResourceView* m_LocalLightingSRV;

	// ShadoMap
	static ID3D11DepthStencilView*   m_ShadowMapDSV[SHADOWMAP_NUM];
	static ID3D11ShaderResourceView* m_ShadowMapSRV[SHADOWMAP_NUM];
	static ID3D11DepthStencilView*   m_DepthDSV; // Z pre-pass
	static ID3D11ShaderResourceView* m_DepthStencilSRV;

	// PostProcess
	static ID3D11RenderTargetView*   m_DepthOfFieldRTV;
	static ID3D11ShaderResourceView* m_DepthOfFieldSRV;

	static ID3D11RenderTargetView*   m_FogRTV;
	static ID3D11ShaderResourceView* m_FogSRV;

	static ID3D11RenderTargetView*   m_LuminanceRTV;
	static ID3D11ShaderResourceView* m_LuminanceSRV;

	static ID3D11RenderTargetView*   m_BloomsRTV[4];
	static ID3D11ShaderResourceView* m_BloomsSRV[4];
	static ID3D11DepthStencilView*   m_BloomsDSV[4];
	static ID3D11RenderTargetView*   m_BloomRTV;
	static ID3D11ShaderResourceView* m_BloomSRV;

	static ID3D11RenderTargetView*   m_AntiAliasingRTV;
	static ID3D11ShaderResourceView* m_AntiAliasingSRV;

	// Debug
	static ID3D11RenderTargetView*   m_DebugCascadeShadowRTV;
	static ID3D11ShaderResourceView* m_DebugCascadeShadowSRV;

	static ID3D11RenderTargetView*   m_DebugAnimationRTV;
	static ID3D11ShaderResourceView* m_DebugAnimationSRV;


	static ID3D11RasterizerState*    m_RasterizerStateFillWire;
	static ID3D11RasterizerState*    m_RasterizerStateCullBack;
	static ID3D11RasterizerState*    m_RasterizerStateCullFront;

	static ID3D11SamplerState*       m_SamplerStateWRAP;
	static ID3D11SamplerState*       m_SamplerStateCLAMP;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_PointLightBuffer;
	static ID3D11Buffer*			m_SpotLightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;
	static ID3D11Buffer*			m_FogBuffer;
	static ID3D11Buffer*			m_LightViewProjectionCropBuffer;
	static ID3D11Buffer*			m_PlayerParameterBuffer;
	static ID3D11Buffer*            m_GrassBuffer;
	static ID3D11Buffer*            m_WavesBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*        m_BlendStateDisable;
	static ID3D11BlendState*        m_BlendStateEnable;

public:
	static void Init();
	static void Uninit();
	static void BeginReflection();
	static void BeginZPrePass();
	static void BeginGBuffer();
	static void BeginShadowMap(int num);
	static void BeginDirectionalLighting();
	static void BeginLocalLighting();
	static void BeginDepthOfField();
	static void BeginFog();
	static void BeginLuminance();
	static void BeginBlooms(int num);
	static void BeginBloom();
	static void BeginAntiAliasing();
	static void BeginDebugCascadeShadowView();
	static void BeginDebugAnimationView();
	static void Begin();
	static void End();


	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX * WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX * ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX * ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetPointLights(POINTLIGHTS Light);
	static void SetSpotLight(SPOTLIGHT Light);
	static void SetLightViewProjectionCropMatrix(D3DXMATRIX LightViewProjectionCrop[]);
	static void SetCamera(CAMERA Camera);
	static void SetPostProcessParameter(POSTPROCESSPARAMETER param);
	static void SetFog(FOG fog);
	static void SetPlayerParameter(PLAYERPARAMETER param);
	static void SetGrass(D3DXVECTOR4 param);
	static void SetWaves(WAVES waves);


	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }

	static void SetRasterizerStateFillWire() { m_DeviceContext->RSSetState(m_RasterizerStateFillWire); }
	static void SetRasterizerStateCullBack(){ m_DeviceContext->RSSetState(m_RasterizerStateCullBack); }
	static void SetRasterizerStateCullFront() { m_DeviceContext->RSSetState(m_RasterizerStateCullFront); }

	static void SetSamplerStateWRAP();
	static void SetSamplerStateCLAMP();

	static void SetBlendStateDisable();
	static void SetBlendStateEnable();

	static void SetDefaultViewport();
	static void SetWideViewport();
	static void SetVariableViewport(int ratio);

	static ID3D11ShaderResourceView* GetReflectionTexture() { return m_ReflectionSRV; }
	static ID3D11ShaderResourceView* GetAlbedoColorTexture() { return m_AlbedoColorSRV; }
	static ID3D11ShaderResourceView* GetPositionTexture() { return m_PositionSRV; }
	static ID3D11ShaderResourceView* GetNormalTexture() { return m_NormalSRV; }
	static ID3D11ShaderResourceView* GetShadowMapTexture(int num) { return m_ShadowMapSRV[num]; }
	static ID3D11ShaderResourceView* GetDepthTexture() { return m_DepthStencilSRV; }
	static ID3D11ShaderResourceView* GetMaterialTexture() { return m_MaterialSRV; }
	static ID3D11ShaderResourceView* GetDirectionalLightingTexture() { return m_DirectionalLightingSRV; }
	static ID3D11ShaderResourceView* GetLocalLightingTexture() { return m_LocalLightingSRV; }
	static ID3D11ShaderResourceView* GetDepthOfFieldTexture() { return m_DepthOfFieldSRV; }
	static ID3D11ShaderResourceView* GetFogTexture() { return m_FogSRV; }
	static ID3D11ShaderResourceView* GetLuminanceTexture() { return m_LuminanceSRV; }
	static ID3D11ShaderResourceView* GetBloomsTexture(int num) { return m_BloomsSRV[num]; }
	static ID3D11ShaderResourceView* GetBloomTexture() { return m_BloomSRV; }
	static ID3D11ShaderResourceView* GetAntiAliasingTexture() { return m_AntiAliasingSRV; }
	static ID3D11ShaderResourceView* GetDebugCascadeShadowTexture() { return m_DebugCascadeShadowSRV; }
	static ID3D11ShaderResourceView* GetDebugAnimationTexture() { return m_DebugAnimationSRV; }

	static ID3D11ShaderResourceView* GetTextureByNumber(int num);

	static void DrawBackBuffer( void )
	{
		// レンダーターゲットビュー生成、設定
		ID3D11Texture2D* pBackBuffer = NULL;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_DefaultBackBufferRTV);
		pBackBuffer->Release();
	}

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
	static void CreateGeometryShader(ID3D11GeometryShader** GeometryShader, const char* FileName);
	static void CreateComputeShader(ID3D11ComputeShader** ComputeShader, const char* FileName);

	static D3DXMATRIX GetWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position);




};
