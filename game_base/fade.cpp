#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "fade.h"

bool Fade::m_FadeTexture = true;

void Fade::Init()
{
	UIObject::Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Fade::Uninit()
{
	UIObject::Uninit();
}

void Fade::Update()
{
	// フェイドイン
	if (m_Fade)
	{
		m_Alpha += 0.01f;
		if (m_Alpha >= 1.0f)
		{
			m_Alpha = 1.0f;
			m_FadeFinished = true;
		}
	}
	// フェイドアウト
	else
	{
		m_Alpha -= 0.01f;
		if (m_Alpha <= 0.0f)
		{
			m_Alpha = 0.0f;
		}
	}

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha);
	GameObject::Update();
}

void Fade::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// 頂点バッファ設定
	SetVertexBuffer();

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// テクスチャ設定
	if(m_FadeTexture) Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetFadeTexture());
	else Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetKenshouTexture());

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描写
	Renderer::GetDeviceContext()->Draw(4, 0);
}