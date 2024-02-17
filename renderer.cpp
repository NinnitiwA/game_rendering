#include "main.h"
#include "renderer.h"
#include <io.h>

D3D_FEATURE_LEVEL         Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*             Renderer::m_Device = NULL;
ID3D11DeviceContext*      Renderer::m_DeviceContext = NULL;
IDXGISwapChain*           Renderer::m_SwapChain = NULL;

ID3D11RenderTargetView*   Renderer::m_DefaultBackBufferRTV = NULL;
ID3D11DepthStencilView*   Renderer::m_DefaultDepthDSV = NULL;

ID3D11RenderTargetView*   Renderer::m_ReflectionRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_ReflectionSRV = NULL;
ID3D11DepthStencilView*   Renderer::m_ReflectionDSV = NULL;
ID3D11RenderTargetView*   Renderer::m_AlbedoColorRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_AlbedoColorSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_PositionRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_PositionSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_NormalRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_NormalSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_MaterialRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_MaterialSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_DirectionalLightingRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_DirectionalLightingSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_LocalLightingRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_LocalLightingSRV = NULL;
ID3D11DepthStencilView*   Renderer::m_DepthDSV = NULL;
ID3D11ShaderResourceView* Renderer::m_DepthStencilSRV = NULL;
ID3D11DepthStencilView*   Renderer::m_ShadowMapDSV[] = {};
ID3D11ShaderResourceView* Renderer::m_ShadowMapSRV[] = {};
ID3D11RenderTargetView*   Renderer::m_DepthOfFieldRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_DepthOfFieldSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_FogRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_FogSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_LuminanceRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_LuminanceSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_BloomsRTV[4]  = {NULL};
ID3D11ShaderResourceView* Renderer::m_BloomsSRV[4]  = {NULL};
ID3D11DepthStencilView*   Renderer::m_BloomsDSV[4]  = {NULL};
ID3D11RenderTargetView*   Renderer::m_BloomRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_BloomSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_AntiAliasingRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_AntiAliasingSRV = NULL;

ID3D11RenderTargetView*   Renderer::m_DebugCascadeShadowRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_DebugCascadeShadowSRV = NULL;
ID3D11RenderTargetView*   Renderer::m_DebugAnimationRTV = NULL;
ID3D11ShaderResourceView* Renderer::m_DebugAnimationSRV = NULL;

ID3D11RasterizerState*    Renderer::m_RasterizerStateFillWire  = NULL;;
ID3D11RasterizerState*    Renderer::m_RasterizerStateCullBack  = NULL;
ID3D11RasterizerState*    Renderer::m_RasterizerStateCullFront = NULL;

ID3D11SamplerState*       Renderer::m_SamplerStateWRAP  = NULL;
ID3D11SamplerState*       Renderer::m_SamplerStateCLAMP = NULL;

ID3D11BlendState*         Renderer::m_BlendStateDisable = NULL;
ID3D11BlendState*         Renderer::m_BlendStateEnable  = NULL;

ID3D11Buffer*			Renderer::m_WorldBuffer = NULL;
ID3D11Buffer*			Renderer::m_ViewBuffer = NULL;
ID3D11Buffer*			Renderer::m_ProjectionBuffer = NULL;
ID3D11Buffer*			Renderer::m_MaterialBuffer = NULL;
ID3D11Buffer*			Renderer::m_LightBuffer = NULL;
ID3D11Buffer*			Renderer::m_PointLightBuffer = NULL;
ID3D11Buffer*			Renderer::m_SpotLightBuffer = NULL;
ID3D11Buffer*			Renderer::m_CameraBuffer = NULL;
ID3D11Buffer*			Renderer::m_ParameterBuffer = NULL;
ID3D11Buffer*			Renderer::m_FogBuffer = NULL;
ID3D11Buffer*			Renderer::m_LightViewProjectionCropBuffer = NULL;
ID3D11Buffer*			Renderer::m_PlayerParameterBuffer = NULL;
ID3D11Buffer*           Renderer::m_GrassBuffer = NULL;
ID3D11Buffer*           Renderer::m_WavesBuffer = NULL;


ID3D11DepthStencilState* Renderer::m_DepthStateEnable = NULL;
ID3D11DepthStencilState* Renderer::m_DepthStateDisable = NULL;

bool g_HardWareGPU = false;

void Renderer::Init()
{
	HRESULT hr = S_OK;


	// スワップチェーン生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetWindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	if (!g_HardWareGPU)
	{
		// ファクトリー、アダプター設定
		IDXGIFactory* dxgiFactory;
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		IDXGIAdapter* highPerformanceAdapter = nullptr;
		UINT maxVRAM = 0;
		UINT adapterNum = 0;

		for (UINT i = 0; dxgiFactory->EnumAdapters(i, &highPerformanceAdapter) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC adapterDesc;
			highPerformanceAdapter->GetDesc(&adapterDesc);

			// VRAMのサイズでアダプターの性能を評価
			if (adapterDesc.DedicatedVideoMemory > maxVRAM) {
				maxVRAM = adapterDesc.DedicatedVideoMemory;
				adapterNum = i;
			}
		}

		dxgiFactory->EnumAdapters(adapterNum, &highPerformanceAdapter);
		DXGI_ADAPTER_DESC adapterDesc;
		highPerformanceAdapter->GetDesc(&adapterDesc);


		// デバイス、コンテキスト生成
		hr = D3D11CreateDevice(highPerformanceAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			0,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&m_Device,
			&m_FeatureLevel,
			&m_DeviceContext);


		// スワップチェーン生成
		highPerformanceAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
		dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain);
	}
	else
	{
		hr = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			&m_FeatureLevel,
			&m_DeviceContext);
	}


	
	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = sd.BufferDesc.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	m_Device->CreateRenderTargetView(pBackBuffer, &rtvd, &m_DefaultBackBufferRTV);
	pBackBuffer->Release();


	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	m_Device->CreateTexture2D(&td, NULL, &depthTexture);

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	m_Device->CreateDepthStencilView(depthTexture, &dsvd, &m_DefaultDepthDSV);
	depthTexture->Release();

	m_DeviceContext->OMSetRenderTargets(1, &m_DefaultBackBufferRTV, m_DefaultDepthDSV);



	{// カラーマップ
		//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // HDRIフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_AlbedoColorRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_AlbedoColorSRV);
		mapTexture->Release();
	}
	{// 座標マップ
	//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_PositionRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_PositionSRV);
		mapTexture->Release();
	}
	{// 法線マップ
		//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_NormalRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_NormalSRV);
		mapTexture->Release();
	}
	{// マテリアルマップ
	    //テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_MaterialRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_MaterialSRV);
		mapTexture->Release();
	}
	{// 反射
//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // HDRIフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_ReflectionRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_ReflectionSRV);
		mapTexture->Release();

		//テクスチャー作成
		ID3D11Texture2D* depthStencilBuffer = NULL;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = sd.BufferDesc.Width;
		depthBufferDesc.Height = sd.BufferDesc.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //ピクセルフォーマット
		depthBufferDesc.SampleDesc = sd.SampleDesc;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

		//デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_Device->CreateDepthStencilView(depthStencilBuffer, &dsvd, &m_ReflectionDSV);
	}
	{// 指向性ライト・スペキュラ
		//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_DirectionalLightingRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_DirectionalLightingSRV);
		mapTexture->Release();
	}
	{// ローカルライト
	//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_LocalLightingRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_LocalLightingSRV);
		mapTexture->Release();
	}
	{// 深度ステンシル
		//テクスチャー作成
		ID3D11Texture2D* depthStencilBuffer = NULL;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		
		depthBufferDesc.Width = sd.BufferDesc.Width;
		depthBufferDesc.Height = sd.BufferDesc.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //ピクセルフォーマット
		depthBufferDesc.SampleDesc = sd.SampleDesc;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

		//デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_Device->CreateDepthStencilView(depthStencilBuffer, &dsvd, &m_DepthDSV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(depthStencilBuffer, &srvd, &m_DepthStencilSRV);
		depthStencilBuffer->Release();
	}
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{// シャドウマッピング
		//テクスチャー作成
		ID3D11Texture2D* depthStencilBuffer = NULL;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		float size = 1.0f; // カスケードのテクスチャサイズ
		if (i == 0) size = 2.0f;
		else if (i == 1) size = 1.0f;
		else size = 0.5f;

		depthBufferDesc.Width = (UINT)(sd.BufferDesc.Width * size);
		depthBufferDesc.Height = (UINT)(sd.BufferDesc.Height * size);
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS; //ピクセルフォーマット
		depthBufferDesc.SampleDesc = sd.SampleDesc;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

		//デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_Device->CreateDepthStencilView(depthStencilBuffer, &dsvd, &m_ShadowMapDSV[i]);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(depthStencilBuffer, &srvd, &m_ShadowMapSRV[i]);
		depthStencilBuffer->Release();
	}
	{// 被写界深度マップ
	//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_DepthOfFieldRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_DepthOfFieldSRV);
		mapTexture->Release();
	}
	{// フォグマップ
	//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_FogRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_FogSRV);
		mapTexture->Release();
	}
	{// 輝度マップ
//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width / 2;
		mapTextureDesc.Height = sd.BufferDesc.Height / 2;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_LuminanceRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_LuminanceSRV);
		mapTexture->Release();
	}
	for (int i = 0; i < 4; i++)
	{// ブルームマップ
//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = (UINT)(sd.BufferDesc.Width / ((i + 1) * 2.0f));
		mapTextureDesc.Height = (UINT)(sd.BufferDesc.Height / ((i + 1) * 2.0f));
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_BloomsRTV[i]);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_BloomsSRV[i]);
		mapTexture->Release();

		//ステンシル用テクスチャー作成
		ID3D11Texture2D* depthTexture = NULL;
		ZeroMemory(&td, sizeof(td));
		td.Width = sd.BufferDesc.Width / (i + 2);
		td.Height = sd.BufferDesc.Height / (i + 2);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_Device->CreateTexture2D(&td, NULL, &depthTexture);

		//ステンシルターゲット作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Flags = 0;
		m_Device->CreateDepthStencilView(depthTexture, &dsvd, &m_BloomsDSV[i]);
	}
	{// ブルームマップ
	//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_BloomRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_BloomSRV);
		mapTexture->Release();
	}
	{// アンチエイリアシングマップ
//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_AntiAliasingRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_AntiAliasingSRV);
		mapTexture->Release();
	}
	{// デバッグカスケードシャドウマップ
		//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_DebugCascadeShadowRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_DebugCascadeShadowSRV);
		mapTexture->Release();
	}
	{// デバッグアニメーションマップ
		//テクスチャー作成
		ID3D11Texture2D* mapTexture = NULL;
		D3D11_TEXTURE2D_DESC mapTextureDesc;
		ZeroMemory(&mapTextureDesc, sizeof(mapTextureDesc));

		mapTextureDesc.Width = sd.BufferDesc.Width;
		mapTextureDesc.Height = sd.BufferDesc.Height;
		mapTextureDesc.MipLevels = 1;
		mapTextureDesc.ArraySize = 1;
		mapTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //ピクセルフォーマット
		mapTextureDesc.SampleDesc = sd.SampleDesc;
		mapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		mapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		mapTextureDesc.CPUAccessFlags = 0;
		mapTextureDesc.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&mapTextureDesc, NULL, &mapTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = mapTextureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(mapTexture, &rtvd, &m_DebugAnimationRTV);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = mapTextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(mapTexture, &srvd, &m_DebugAnimationSRV);
		mapTexture->Release();
	}



	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	//FillWire作成
	m_Device->CreateRasterizerState(&rd, &m_RasterizerStateFillWire);

	// CullBack作成
	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_SOLID;
	m_Device->CreateRasterizerState(&rd, &m_RasterizerStateCullBack);
	m_DeviceContext->RSSetState(m_RasterizerStateCullBack);
	// CullFront作成
	rd.CullMode = D3D11_CULL_FRONT;
	m_Device->CreateRasterizerState(&rd, &m_RasterizerStateCullFront);


	// ブレンドステート設定
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[1].BlendEnable = FALSE;
		blendDesc.RenderTarget[2].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_Device->CreateBlendState(&blendDesc, &m_BlendStateDisable);
		m_DeviceContext->OMSetBlendState(m_BlendStateDisable, blendFactor, 0xffffffff);
	}
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[1].BlendEnable = TRUE;
		blendDesc.RenderTarget[2].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		m_Device->CreateBlendState(&blendDesc, &m_BlendStateEnable);
	}



	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);



	// サンプラーステート設定
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_Device->CreateSamplerState(&samplerDesc, &m_SamplerStateWRAP);
		m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerStateWRAP);
	}
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_Device->CreateSamplerState(&samplerDesc, &m_SamplerStateCLAMP);
	}


	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);
	m_DeviceContext->GSSetConstantBuffers(0, 1, &m_WorldBuffer);
	m_DeviceContext->PSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
	m_DeviceContext->GSSetConstantBuffers(1, 1, &m_ViewBuffer);
	m_DeviceContext->PSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);
	m_DeviceContext->GSSetConstantBuffers(2, 1, &m_ProjectionBuffer);
	m_DeviceContext->PSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);


	hBufferDesc.ByteWidth = sizeof(CAMERA);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->VSSetConstantBuffers(5, 1, &m_CameraBuffer);
	m_DeviceContext->CSSetConstantBuffers(5, 1, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);


	hBufferDesc.ByteWidth = sizeof(POSTPROCESSPARAMETER);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_ParameterBuffer);
	m_DeviceContext->VSSetConstantBuffers(6, 1, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);


	hBufferDesc.ByteWidth = sizeof(POINTLIGHTS);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_PointLightBuffer);
	m_DeviceContext->VSSetConstantBuffers(7, 1, &m_PointLightBuffer);
	m_DeviceContext->PSSetConstantBuffers(7, 1, &m_PointLightBuffer);


	hBufferDesc.ByteWidth = sizeof(SPOTLIGHT);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_SpotLightBuffer);
	m_DeviceContext->VSSetConstantBuffers(8, 1, &m_SpotLightBuffer);
	m_DeviceContext->PSSetConstantBuffers(8, 1, &m_SpotLightBuffer);


	hBufferDesc.ByteWidth = sizeof(FOG);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_FogBuffer);
	m_DeviceContext->VSSetConstantBuffers(9, 1, &m_FogBuffer);
	m_DeviceContext->PSSetConstantBuffers(9, 1, &m_FogBuffer);


	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX) * 3;

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_LightViewProjectionCropBuffer);
	m_DeviceContext->VSSetConstantBuffers(10, 1, &m_LightViewProjectionCropBuffer);
	m_DeviceContext->GSSetConstantBuffers(10, 1, &m_LightViewProjectionCropBuffer);
	m_DeviceContext->PSSetConstantBuffers(10, 1, &m_LightViewProjectionCropBuffer);


	hBufferDesc.ByteWidth = sizeof(PLAYERPARAMETER);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_PlayerParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(11, 1, &m_PlayerParameterBuffer);


	hBufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_GrassBuffer);
	m_DeviceContext->VSSetConstantBuffers(12, 1, &m_GrassBuffer);
	m_DeviceContext->GSSetConstantBuffers(12, 1, &m_GrassBuffer);


	hBufferDesc.ByteWidth = sizeof(WAVES);

	m_Device->CreateBuffer(&hBufferDesc, NULL, &m_WavesBuffer);
	m_DeviceContext->VSSetConstantBuffers(13, 1, &m_WavesBuffer);
	m_DeviceContext->PSSetConstantBuffers(13, 1, &m_WavesBuffer);



	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Roughness = 0.5f;
	material.Metalic = 0.5f;
	SetMaterial(material);



	// ImGui初期化
	GUI::ImGuiInit(m_Device, m_DeviceContext);
}


void Renderer::Uninit()
{
	// オブジェクト解放
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_PointLightBuffer->Release();
	m_SpotLightBuffer->Release();
	m_CameraBuffer->Release();
	m_MaterialBuffer->Release();
	m_ParameterBuffer->Release();
	m_FogBuffer->Release();
	m_LightViewProjectionCropBuffer->Release();
	m_PlayerParameterBuffer->Release();

	m_DeviceContext->ClearState();

	m_DefaultBackBufferRTV->Release();
	m_DefaultDepthDSV->Release();
	m_ReflectionRTV->Release();
	m_ReflectionSRV->Release();
	m_ReflectionDSV->Release();
	m_PositionRTV->Release();
	m_PositionSRV->Release();
	m_AlbedoColorRTV->Release();
	m_AlbedoColorSRV->Release();
	m_NormalRTV->Release();
	m_NormalSRV->Release();
	m_MaterialRTV->Release();
	m_MaterialSRV->Release();
	m_DirectionalLightingRTV->Release();
	m_DirectionalLightingSRV->Release();
	m_LocalLightingRTV->Release();
	m_LocalLightingSRV->Release();
	m_DepthDSV->Release();
	m_DepthStencilSRV->Release();
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		m_ShadowMapSRV[i]->Release();
		m_ShadowMapDSV[i]->Release();
	}
	m_DepthOfFieldRTV->Release();
	m_DepthOfFieldSRV->Release();
	m_BloomRTV->Release();
	m_BloomSRV->Release();
	for (int i = 0; i < 4; i++)
	{
		m_BloomsRTV[i]->Release();
		m_BloomsSRV[i]->Release();
		m_BloomsDSV[i]->Release();
	}
	m_AntiAliasingRTV->Release();
	m_AntiAliasingSRV->Release();

	m_DebugCascadeShadowRTV->Release();
	m_DebugCascadeShadowSRV->Release();
	m_DebugAnimationRTV->Release();
	m_DebugAnimationSRV->Release();

	m_BlendStateDisable->Release();
	m_BlendStateEnable->Release();

	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();


	// ImGui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Renderer::BeginReflection()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_ReflectionRTV, m_ReflectionDSV);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_ReflectionRTV, ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_ReflectionDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginZPrePass()
{
	// ブレンドステート設定
	SetBlendStateDisable();

	// 深度ステンシルビューをセット
	m_DeviceContext->OMSetRenderTargets(0, nullptr, m_DepthDSV);
	// 深度バッファクリア
	m_DeviceContext->ClearDepthStencilView(m_DepthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::BeginGBuffer()
{
	//カラーマップと法線マップをレンダリングターゲットに設定
	ID3D11RenderTargetView* renderTargets[] = {
	m_AlbedoColorRTV,
	m_PositionRTV,
	m_NormalRTV,
	m_MaterialRTV,
	};

	//マルチレンダリングターゲットをセット
	m_DeviceContext->OMSetRenderTargets(4, renderTargets, m_DepthDSV);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
	// バックバッファクリア （デバッグ用に赤色でクリア）
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_AlbedoColorRTV, ClearColor);
	m_DeviceContext->ClearRenderTargetView(m_PositionRTV, ClearColor);
	m_DeviceContext->ClearRenderTargetView(m_NormalRTV, ClearColor);
	m_DeviceContext->ClearRenderTargetView(m_MaterialRTV, ClearColor);
}


void Renderer::BeginShadowMap(int num)
{
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowMapDSV[num]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	m_DeviceContext->ClearDepthStencilView(m_ShadowMapDSV[num], D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginDirectionalLighting()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_DirectionalLightingRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_DirectionalLightingRTV, ClearColor);
}

void Renderer::BeginLocalLighting()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_LocalLightingRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_LocalLightingRTV, ClearColor);
}

void Renderer::BeginDepthOfField()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_DepthOfFieldRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_DepthOfFieldRTV, ClearColor);
}

void Renderer::BeginFog()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_FogRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_FogRTV, ClearColor);
}

void Renderer::BeginLuminance()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_LuminanceRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_LuminanceRTV, ClearColor);
}

void Renderer::BeginBlooms(int num)
{
	m_DeviceContext->OMSetRenderTargets(1, &m_BloomsRTV[num], m_BloomsDSV[num]);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_BloomsRTV[num], ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_BloomsDSV[num], D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginBloom()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_BloomRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_BloomRTV, ClearColor);
}

void Renderer::BeginAntiAliasing()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_AntiAliasingRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_AntiAliasingRTV, ClearColor);
}

void Renderer::BeginDebugCascadeShadowView()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_DebugCascadeShadowRTV, NULL);
	D3DXCOLOR ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_DebugCascadeShadowRTV, ClearColor);
}

void Renderer::BeginDebugAnimationView()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_DebugAnimationRTV, NULL);
	D3DXCOLOR ClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_DebugAnimationRTV, ClearColor);
}

void Renderer::Begin()
{
	// ブレンドステート設定
	SetBlendStateEnable();

	//デフォルトレンダリングターゲットをセット
	m_DeviceContext->OMSetRenderTargets(1, &m_DefaultBackBufferRTV, m_DefaultDepthDSV);

	// バックバッファクリア
	D3DXCOLOR ClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_DefaultBackBufferRTV, ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DefaultDepthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	m_SwapChain->Present( 1, 0 );
}



void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_DeviceContext->UpdateSubresource( m_ProjectionBuffer, 0, NULL, &projection, 0, 0 );

}


void Renderer::SetWorldMatrix( D3DXMATRIX *WorldMatrix )
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix( D3DXMATRIX *ViewMatrix )
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix( D3DXMATRIX *ProjectionMatrix )
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}


void Renderer::SetMaterial( MATERIAL Material )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	D3DXMatrixTranspose(&Light.ViewMatrix, &Light.ViewMatrix);
	D3DXMatrixTranspose(&Light.ProjectionMatrix, &Light.ProjectionMatrix);

	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetPointLights(POINTLIGHTS Light)
{
	m_DeviceContext->UpdateSubresource(m_PointLightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetSpotLight(SPOTLIGHT Light)
{
	m_DeviceContext->UpdateSubresource(m_SpotLightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetLightViewProjectionCropMatrix(D3DXMATRIX LightViewProjectionCrop[])
{
	D3DXMATRIX lightViewProjectionCrop[3];
	D3DXMatrixTranspose(&lightViewProjectionCrop[0], &LightViewProjectionCrop[0]);
	D3DXMatrixTranspose(&lightViewProjectionCrop[1], &LightViewProjectionCrop[1]);
	D3DXMatrixTranspose(&lightViewProjectionCrop[2], &LightViewProjectionCrop[2]);
	m_DeviceContext->UpdateSubresource(m_LightViewProjectionCropBuffer, 0, NULL, &lightViewProjectionCrop, 0, 0);
}

void Renderer::SetCamera(CAMERA Camera)
{
	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &Camera, 0, 0);
}

void Renderer::SetPostProcessParameter(POSTPROCESSPARAMETER param)
{
	m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetFog(FOG fog)
{
	m_DeviceContext->UpdateSubresource(m_FogBuffer, 0, NULL, &fog, 0, 0);
}

void Renderer::SetPlayerParameter(PLAYERPARAMETER param)
{
	m_DeviceContext->UpdateSubresource(m_PlayerParameterBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetGrass(D3DXVECTOR4 param)
{
	m_DeviceContext->UpdateSubresource(m_GrassBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetWaves(WAVES waves)
{
	m_DeviceContext->UpdateSubresource(m_WavesBuffer, 0, NULL, &waves, 0, 0);
}

void Renderer::SetSamplerStateWRAP()
{
	m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerStateWRAP);
}

void Renderer::SetSamplerStateCLAMP()
{
	m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerStateCLAMP);
}

void Renderer::SetBlendStateDisable()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendStateDisable, blendFactor, 0xffffffff);
}

void Renderer::SetBlendStateEnable()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendStateEnable, blendFactor, 0xffffffff);
}

void Renderer::SetDefaultViewport()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetWideViewport()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH * 2.0f;
	vp.Height = (FLOAT)SCREEN_HEIGHT * 2.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetVariableViewport(int ratio)
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH / ((ratio + 1) * 2.0f);
	vp.Height = (FLOAT)SCREEN_HEIGHT / ((ratio + 1) * 2.0f);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

ID3D11ShaderResourceView * Renderer::GetTextureByNumber(int num)
{
	switch (num)
	{
	case 0:
		return Renderer::GetAlbedoColorTexture();
		break;
	case 1:
		return Renderer::GetPositionTexture();
		break;
	case 2:
		return Renderer::GetNormalTexture();
		break;
	case 3:
		return Renderer::GetShadowMapTexture(0);
		break;
	case 4:
		return Renderer::GetMaterialTexture();
		break;
	case 5:
		return Renderer::GetDirectionalLightingTexture();
		break;
	case 6:
		return Renderer::GetLocalLightingTexture();
		break;
	}

	return nullptr;
}


D3DXMATRIX Renderer::GetWorldMatrix(D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation, D3DXVECTOR3 Position)
{
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
	world = scale * rot * trans;

	return world;
}



// 頂点シェーダ生成
void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	    { "SV_InstanceID", 0, DXGI_FORMAT_R32_UINT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // インスタンスIDの設定
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



// ピクセルシェーダ生成
void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::CreateGeometryShader(ID3D11GeometryShader** GeometryShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateGeometryShader(buffer, fsize, NULL, GeometryShader);

	delete[] buffer;
}



// コンピュートシェーダ生成
void Renderer::CreateComputeShader(ID3D11ComputeShader ** ComputeShader, const char * FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateComputeShader(buffer, fsize, NULL, ComputeShader);

	delete[] buffer;
}

