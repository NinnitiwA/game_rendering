#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "resource.h"
#include "camera.h"
#include "debugInstancingObject.h"

#define INSTANCE_X 200
#define INSTANCE_Y 200
#define INSTANCE_NUM INSTANCE_X * INSTANCE_X
#define INSTANCE_SPACE 2.5f

void DebugInstancingObject::Init()
{
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * 0.6f;
	m_ThreadNum = 8;

	InstancingObject::InitInstancePositionData();

	// Instanced
	// StructuredBufferの作成--------------------------------------------------------------------------------------------------
	{
		// 座標データ
		{
			// DebugObject クラスの初期化関数内で
			D3D11_BUFFER_DESC instanceBufferDesc;
			ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
			instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			instanceBufferDesc.StructureByteStride = sizeof(INSTANCE_POSITION_DATA); // 構造体のサイズ
			instanceBufferDesc.ByteWidth = sizeof(INSTANCE_POSITION_DATA) * INSTANCE_NUM; // バッファ全体のサイズ
			instanceBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			instanceBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


			// インスタンスデータを初期化
			INSTANCE_POSITION_DATA* instanceDataArray = new INSTANCE_POSITION_DATA[INSTANCE_NUM];
			for (unsigned int y = 0; y < INSTANCE_Y; y++)
			{
				for (unsigned int x = 0; x < INSTANCE_X; x++)
				{
					D3DXVECTOR3 position(-(INSTANCE_SPACE * INSTANCE_Y / 2.0f) + INSTANCE_SPACE * x, 1.0f, -(INSTANCE_SPACE * INSTANCE_Y / 2.0f) + INSTANCE_SPACE * y);
					instanceDataArray[x + y * INSTANCE_Y].Position = position;
				}
			}

			D3D11_SUBRESOURCE_DATA initDataDesc = {};
			initDataDesc.pSysMem = instanceDataArray;

			Renderer::GetDevice()->CreateBuffer(&instanceBufferDesc, &initDataDesc, &m_InstancePositionBuffer);

			delete[] instanceDataArray;
		}

		// カリングデータ
		{
			// 書き込み用カリングバッファの作成
			// cullingBuffer
			D3D11_BUFFER_DESC cullingWriteBufferDesc = {};
			ZeroMemory(&cullingWriteBufferDesc, sizeof(cullingWriteBufferDesc));
			cullingWriteBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			cullingWriteBufferDesc.StructureByteStride = sizeof(INSTANCE_POSITION_DATA); // 構造体のサイズ
			cullingWriteBufferDesc.ByteWidth = sizeof(INSTANCE_POSITION_DATA) * INSTANCE_NUM; // バッファ全体のサイズ
			cullingWriteBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			cullingWriteBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;


			// インスタンスデータを初期化
			INSTANCE_POSITION_DATA* cullingDataArray = new INSTANCE_POSITION_DATA[INSTANCE_NUM];
			for (unsigned int y = 0; y < INSTANCE_Y; y++)
			{
				for (unsigned int x = 0; x < INSTANCE_X; x++)
				{
					D3DXVECTOR3 position(-(INSTANCE_SPACE * INSTANCE_Y / 2.0f) + INSTANCE_SPACE * x, 1.0f, -(INSTANCE_SPACE * INSTANCE_Y / 2.0f) + INSTANCE_SPACE * y);
					cullingDataArray[x + y * INSTANCE_Y].Position = position;
				}
			}

			D3D11_SUBRESOURCE_DATA cullingDataDesc = {};
			cullingDataDesc.pSysMem = cullingDataArray;
			cullingDataDesc.SysMemPitch = 0;
			cullingDataDesc.SysMemSlicePitch = 0;

			Renderer::GetDevice()->CreateBuffer(&cullingWriteBufferDesc, &cullingDataDesc, &m_CullingWriteBuffer);

			// カリング結果を格納するバッファの作成
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = INSTANCE_NUM; // インスタンスの数に合わせる

			Renderer::GetDevice()->CreateUnorderedAccessView(m_CullingWriteBuffer, &uavDesc, &m_CullingResultBufferUAV);



			// 読み込み用カリングバッファの作成
			// cullingBuffer
			D3D11_BUFFER_DESC cullingReadBufferDesc = {};
			ZeroMemory(&cullingReadBufferDesc, sizeof(cullingReadBufferDesc));
			cullingReadBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cullingReadBufferDesc.StructureByteStride = sizeof(INSTANCE_POSITION_DATA); // 構造体のサイズ
			cullingReadBufferDesc.ByteWidth = sizeof(INSTANCE_POSITION_DATA) * INSTANCE_NUM; // バッファ全体のサイズ
			cullingReadBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			cullingReadBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			cullingReadBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			Renderer::GetDevice()->CreateBuffer(&cullingReadBufferDesc, &cullingDataDesc, &m_CullingReadBuffer);


			delete[] cullingDataArray;
		}


		// インスタンス数データ
		{
			// 書き込み用インスタンス数バッファの作成
			// instanceNumBuffer
			D3D11_BUFFER_DESC instanceNumWriteBufferDesc = {};
			ZeroMemory(&instanceNumWriteBufferDesc, sizeof(instanceNumWriteBufferDesc));
			instanceNumWriteBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			instanceNumWriteBufferDesc.StructureByteStride = sizeof(INSTANCE_NUM_DATA); // 構造体のサイズ
			instanceNumWriteBufferDesc.ByteWidth = sizeof(INSTANCE_NUM_DATA); // バッファ全体のサイズ
			instanceNumWriteBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			instanceNumWriteBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			instanceNumWriteBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


			// インスタンスデータを初期化
			INSTANCE_NUM_DATA* instanceNumData = new INSTANCE_NUM_DATA;
			instanceNumData[0].InstanceNum = 0;


			D3D11_SUBRESOURCE_DATA instanceNumDataDesc = {};
			instanceNumDataDesc.pSysMem = instanceNumData;
			instanceNumDataDesc.SysMemPitch = 0;
			instanceNumDataDesc.SysMemSlicePitch = 0;

			Renderer::GetDevice()->CreateBuffer(&instanceNumWriteBufferDesc, &instanceNumDataDesc, &m_InstanceNumWriteBuffer);

			// 結果を格納するバッファの作成
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = 1; // インスタンスの数に合わせる

			Renderer::GetDevice()->CreateUnorderedAccessView(m_InstanceNumWriteBuffer, &uavDesc, &m_InstanceNumBufferUAV);



			// 読み込み用インスタンス数バッファの作成
			// instanceNumBuffer
			D3D11_BUFFER_DESC instanceNumReadBufferDesc = {};
			ZeroMemory(&instanceNumReadBufferDesc, sizeof(instanceNumReadBufferDesc));
			instanceNumReadBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			instanceNumReadBufferDesc.StructureByteStride = sizeof(INSTANCE_NUM_DATA); // 構造体のサイズ
			instanceNumReadBufferDesc.ByteWidth = sizeof(INSTANCE_NUM_DATA); // バッファ全体のサイズ
			instanceNumReadBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			instanceNumReadBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			instanceNumReadBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			Renderer::GetDevice()->CreateBuffer(&instanceNumReadBufferDesc, &instanceNumDataDesc, &m_InstanceNumReadBuffer);


			delete[] instanceNumData;
		}



		// StructuredBufferのバインディング--------------------------------------------------------------------------------------------------

		{
			// シェーダーリソースビューの作成
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN; // フォーマットは構造体のフォーマットに合わせる
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			srvDesc.BufferEx.FirstElement = 0;
			srvDesc.BufferEx.NumElements = INSTANCE_NUM;

			Renderer::GetDevice()->CreateShaderResourceView(m_InstancePositionBuffer, &srvDesc, &m_InstancePositionShaderResourceView);
		}

		{
			// cullingBufferシェーダーリソースビューの作成
			D3D11_SHADER_RESOURCE_VIEW_DESC cullingDesc = {};
			cullingDesc.Format = DXGI_FORMAT_UNKNOWN; // フォーマットは構造体のフォーマットに合わせる
			cullingDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			cullingDesc.BufferEx.FirstElement = 0;
			cullingDesc.BufferEx.NumElements = INSTANCE_NUM;

			Renderer::GetDevice()->CreateShaderResourceView(m_CullingReadBuffer, &cullingDesc, &m_CullingShaderResourceView);
		}

		{
			// シェーダーリソースビューの作成
			D3D11_SHADER_RESOURCE_VIEW_DESC instanceNumDesc = {};
			instanceNumDesc.Format = DXGI_FORMAT_UNKNOWN; // フォーマットは構造体のフォーマットに合わせる
			instanceNumDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			instanceNumDesc.BufferEx.FirstElement = 0;
			instanceNumDesc.BufferEx.NumElements = 1;

			Renderer::GetDevice()->CreateShaderResourceView(m_InstanceNumReadBuffer, &instanceNumDesc, &m_InstanceNumShaderResourceView);
		}
	}
}

void DebugInstancingObject::Uninit()
{
	InstancingObject::UninitInstanceData();
	GameObject::Uninit();
}


void DebugInstancingObject::Update()
{
	InstancingObject::Update();
	GameObject::Update();
}

void DebugInstancingObject::Draw()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetInstancedDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// シェーダーリソースビューのバインディング
	Renderer::GetDeviceContext()->VSSetShaderResources(0, 1, &m_CullingShaderResourceView); // 適切なスロットとシェーダーステージを指定

	// ポリゴン描写
	Resource::GetSphereModel()->DrawInstancedIndirect(m_InstanceNum, m_IndirectArgsBuffer);
}

void DebugInstancingObject::DrawZPrePass()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->CSSetShader(Resource::GetInstancedFrustumCullingCS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// 入力用SRVセット
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_InstancePositionShaderResourceView);
	// 座標出力用UAVセット
	UINT uavCount = INSTANCE_NUM;
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_CullingResultBufferUAV, &uavCount);
	// インスタンス数出力用UAVセット
	UINT insNumCount = 1;
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &m_InstanceNumBufferUAV, &insNumCount);

	// スレッド数の計算
	int dispatchThreadCountX = (INSTANCE_NUM + 255) / 256;

	// コンピュートシェーダーのディスパッチ
	Renderer::GetDeviceContext()->Dispatch(256, 1, 1);



	// バッファのコピー
	Renderer::GetDeviceContext()->CopyResource(m_CullingReadBuffer, m_CullingWriteBuffer);
	Renderer::GetDeviceContext()->CopyResource(m_InstanceNumReadBuffer, m_InstanceNumWriteBuffer);


	// カリング後のインスタンス数を取得
	{
		// バッファのマップ
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Renderer::GetDeviceContext()->Map(m_InstanceNumReadBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		// マップされたポインタを取得
		INSTANCE_NUM_DATA* pData = static_cast<INSTANCE_NUM_DATA*>(mappedResource.pData);
		// データを読み取る
		m_InstanceNum = pData->InstanceNum;
		pData->InstanceNum = 0;
		// バッファのアンマップ
		Renderer::GetDeviceContext()->Unmap(m_InstanceNumReadBuffer, 0);
	}
	// カリング後のインスタンス数を初期化
	{
		// バッファのマップ
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Renderer::GetDeviceContext()->Map(m_InstanceNumWriteBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);
		// マップされたポインタを取得
		INSTANCE_NUM_DATA* pData = static_cast<INSTANCE_NUM_DATA*>(mappedResource.pData);
		// バッファを0で初期化
		memset(pData, 0, sizeof(INSTANCE_NUM_DATA));
		// バッファのアンマップ
		Renderer::GetDeviceContext()->Unmap(m_InstanceNumWriteBuffer, 0);
	}




	// 入力用SRVセット
	Renderer::GetDeviceContext()->VSSetShaderResources(1, 1, &m_CullingShaderResourceView);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetInstancedVS(), NULL, 0);
	Renderer::GetDeviceContext()->CSSetShader(NULL, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	
	// ポリゴン描画
	Resource::GetSphereModel()->DrawInstancedIndirect(m_InstanceNum, m_IndirectArgsBuffer);
}

