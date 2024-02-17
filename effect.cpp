#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "effect.h"

void Effect::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; // DYNAMIC : 頂点バッファ書き換え可能
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // D3D11_CPU_ACCESS_WRITE : 頂点バッファ書き換え可能

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// シェーダ設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	Renderer::CreateVertexShader(&m_DepthVertexShader, &m_VertexLayout, "shader\\depthVS.cso");
	Renderer::CreatePixelShader(&m_DepthPixelShader, "shader\\depthPS.cso");
}

void Effect::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_DepthVertexShader->Release();
	m_DepthPixelShader->Release();
}

void Effect::Update()
{
	m_Frame++;

	if (m_Frame >= m_EffectInfo->m_DivideX * m_EffectInfo->m_DivideY) SetDestroy();
}

void Effect::Draw()
{
	// テクスチャ座標算出
	float x = m_Frame % m_EffectInfo->m_DivideX * (1.0f / m_EffectInfo->m_DivideX);
	float y = m_Frame / m_EffectInfo->m_DivideY * (1.0f / m_EffectInfo->m_DivideY);

	// 頂点データ
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / m_EffectInfo->m_DivideX), y);

		vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / m_EffectInfo->m_DivideY));

		vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / m_EffectInfo->m_DivideX), y + (1.0f / m_EffectInfo->m_DivideY));

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	if (Renderer::GetRenderState() == RENDER_STATE_GENERAL)
	{
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	}
	else if (Renderer::GetRenderState() == RENDER_STATE_DEPTH)
	{
		Renderer::GetDeviceContext()->VSSetShader(m_DepthVertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_DepthPixelShader, NULL, 0);
	}
	else return;

	// ブレンドステートATC設定
	Renderer::SetATCEnable(TRUE);

	// ビルボード設定
	// カメラのビューマトリックス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX view = camera->GetViewMatrix();
	// ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view); // 逆行列
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;
	// マトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描写
	Renderer::GetDeviceContext()->Draw(4, 0);

	// ブレンドステート設定
	Renderer::SetATCEnable(FALSE);
}

void Effect::SetEffect(D3DXVECTOR3 pos, EffectType effectType)
{
	// 座標設定
	SetPosition(pos);

	// エフェクトパラメータ設定
	static EffectInfo et;
	m_EffectInfo = &et;
	switch (effectType)
	{
	case EFFECT_EXPLOSION:
		strcpy(m_EffectInfo->m_TextureName, "asset/texture/volcano.png");
		m_EffectInfo->m_EffectType = EFFECT_EXPLOSION;
		m_EffectInfo->m_DivideX = 5;
		m_EffectInfo->m_DivideY = 5;
		break;
	case EFFECT_IMPULSE:
		strcpy(m_EffectInfo->m_TextureName, "asset/texture/impulse.png");
		m_EffectInfo->m_EffectType = EFFECT_IMPULSE;
		m_EffectInfo->m_DivideX = 10;
		m_EffectInfo->m_DivideY = 1;
		break;
	case EFFECT_NON:
		break;
	default:
		break;
	}

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		m_EffectInfo->m_TextureName,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}