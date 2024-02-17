#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "water.h"
#include "random.h"


void Water::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Water::Uninit()
{
	GameObject::Uninit();
}


void Water::Update()
{
	if (!m_isIndependence)
	{
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();

		m_Position = player->GetPosition();
		m_Position.z += 20.0f;
		m_Position.y = 0.0f;
	}
}


void Water::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	Renderer::GetDeviceContext()->VSSetShader(Resource::GetReflectionVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetReflectionPS(), NULL, 0);


	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metalic = 0.02;
	material.Roughness = 0.3;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	ID3D11ShaderResourceView* reflectionTexture = Renderer::GetReflectionTexture();
	//レンダリングテクスチャを1番にセット
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &reflectionTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(m_numIndex, 0, 0);

}

void Water::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	Renderer::GetDeviceContext()->VSSetShader(Resource::GetReflectionVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetReflectionPS(), NULL, 0);


	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metalic = 0.02;
	material.Roughness = 0.3;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	ID3D11ShaderResourceView* reflectionTexture = Renderer::GetReflectionTexture();
	//レンダリングテクスチャを1番にセット
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &reflectionTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(m_numIndex, 0, 0);
}

void Water::CreateWave(void)
{
	// 波の情報を生成し定数バッファに格納
	WAVES waves;
	for (int i = 0; i < 20; i++)
	{
		WAVE& wave = waves.Wave[i];
		float randomRad = (float)(Random(0, 360) * D3DX_PI * 2 * 0.15f);
		wave.dir.x = sinf(randomRad);
		wave.dir.y = cosf(randomRad);
		wave.amplitude = 0.03f + powf(2.0f, (float)Random(0, 1) * 2.0f) * 0.045f;
		wave.waveLength = 1.0f + powf(2.f, 1.f + (float)Random(0, 2)) * 1.5f;
	}
	Renderer::SetWaves(waves);
}

void Water::CreateWater(bool Flag)
{
	// 識別子設定
	m_isIndependence = Flag;

	// ポリゴン生成
	int yoko = m_Yoko;       // 横のポリゴンの分割数
	int tate = m_Tate;       // 縦のポリゴンの分割数
	float takasa = m_Takasa; // ランダムで作成する高さの最大値
	float sizeX = m_SizeX;   // ポリゴンの横幅
	float sizeZ = m_SizeZ;   // ポリゴンの縦幅

	// 必要な頂点数を計算する
	m_numVertex = (yoko + 1) * (tate + 1);
	m_numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * m_numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int y = 0; y < (tate + 1); y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				int i = y * (yoko + 1) + x;

				float height = ((float)rand() / RAND_MAX) * takasa;
				// 頂点座標の設定
				pVtx[i].Position = D3DXVECTOR3(-(yoko / 2.0f) * sizeX + x * sizeX, 0.0f, (tate / 2.0f) * sizeZ - y * sizeZ);
				// UV値の設定
				pVtx[i].TexCoord = D3DXVECTOR2(x, y);

				D3DXVECTOR3 vUP(0.0f, 0.0f, 1.0f); // アップベクトル
				// 法線の設定
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 接ベクトルの設定
				D3DXVec3Cross(&pVtx[i].Tangent, &pVtx[i].Normal, &vUP);

				// 従法線の設定
				D3DXVec3Cross(&pVtx[i].Binormal, &pVtx[i].Normal, &pVtx[i].Tangent);

				// 頂点カラーの設定
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		int idx = 0;

		for (int y = 0; y < tate; y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				pIdx[idx] = (yoko + 1) + x + (yoko + 1) * y;
				idx++;
				pIdx[idx] = 0 + x + (yoko + 1) * y;
				idx++;
			}

			if (y < (tate - 1))
			{
				pIdx[idx] = yoko + (yoko + 1) * y;
				idx++;
				pIdx[idx] = (yoko + 1) * 2 + (yoko + 1) * y;
				idx++;
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);
	}
}

void Water::CreateWater(D3DXVECTOR3 Pos, int Yoko, int Tate, float Takasa, float SizeX, float SizeZ, bool Flag)
{
	// パラメータ設定
	m_Position = Pos;

	m_Yoko = Yoko;
	m_Tate = Tate;
	m_Takasa = Takasa;
	m_SizeX = SizeX;
	m_SizeZ = SizeZ;

	m_isIndependence = Flag;


	// ポリゴン生成
	int yoko = m_Yoko;       // 横のポリゴンの分割数
	int tate = m_Tate;       // 縦のポリゴンの分割数
	float takasa = m_Takasa; // ランダムで作成する高さの最大値
	float sizeX = m_SizeX;   // ポリゴンの横幅
	float sizeZ = m_SizeZ;   // ポリゴンの縦幅

	// 必要な頂点数を計算する
	m_numVertex = (yoko + 1) * (tate + 1);
	m_numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * m_numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int y = 0; y < (tate + 1); y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				int i = y * (yoko + 1) + x;

				float height = ((float)rand() / RAND_MAX) * takasa;
				// 頂点座標の設定
				pVtx[i].Position = D3DXVECTOR3(-(yoko / 2.0f) * sizeX + x * sizeX, 0.0f, (tate / 2.0f) * sizeZ - y * sizeZ);
				// UV値の設定
				pVtx[i].TexCoord = D3DXVECTOR2(x, y);

				D3DXVECTOR3 vUP(0.0f, 0.0f, 1.0f);
				// 法線の設定
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 接ベクトルの設定
				D3DXVec3Cross(&pVtx[i].Tangent, &pVtx[i].Normal, &vUP);

				// 従法線の設定
				D3DXVec3Cross(&pVtx[i].Binormal, &pVtx[i].Normal, &pVtx[i].Tangent);

				// 頂点カラーの設定
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		int idx = 0;

		for (int y = 0; y < tate; y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				pIdx[idx] = (yoko + 1) + x + (yoko + 1) * y;
				idx++;
				pIdx[idx] = 0 + x + (yoko + 1) * y;
				idx++;
			}

			if (y < (tate - 1))
			{
				pIdx[idx] = yoko + (yoko + 1) * y;
				idx++;
				pIdx[idx] = (yoko + 1) * 2 + (yoko + 1) * y;
				idx++;
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);
	}
}
