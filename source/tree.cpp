#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "tree.h"
#include "particle.h"
#include "model.h"
#include "random.h"

void Tree::Init()
{
	float random = Random(0, 360);
	m_Rotation = D3DXVECTOR3(0.0f, random * D3DX_PI / 180.0f, 0.0f);
	float scale = Random(20, 40);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * (scale / 20.0f);
	m_BaseScale = m_Scale;

	// 接地位置に変更
	m_Position.y -= m_Scale.y / 2.0f;

	// 木モデルの種類設定
	m_TreeType = Random(0, 2);
}

void Tree::Uninit()
{
	GameObject::Uninit();
}

void Tree::Update()
{
	if (!m_isSway) return;


	// 揺れの処理
	{
		float rad = 0.0f; // 揺れの角度
		if (m_SwayFrame % 2 == 0) rad = m_SwayFrame / 8.0f;
		else rad -= m_SwayFrame / 8.0f;

		m_Scale = m_BaseScale + D3DXVECTOR3( cosf(D3DX_PI * 0.5f * rad) * 0.15f, cosf(D3DX_PI * 0.5f * rad) * 0.15f, cosf(D3DX_PI * 0.5f * rad) * 0.15f);

		// フレームカウント
		m_SwayFrame++;
		// 揺れ終了
		if (m_SwayFrame > 10)
		{
			m_SwayFrame = 0;
			m_isSway = false;
			m_Scale = m_BaseScale;
		}
	}

	GameObject::Update();
}

void Tree::Draw()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);
	
	// テクスチャ設定
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// カラー設定
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	Renderer::SetPostProcessParameter(param);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Metalic = 0.02f;
	material.Roughness = 0.12f;
	Renderer::SetMaterial(material);

	// ポリゴン描写
	Resource::GetTreeModel(m_TreeType)->Draw(material);
}

void Tree::DrawShadowMapping()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetTreeModel(m_TreeType)->Draw();
}

void Tree::DrawZPrePass()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetTreeModel(m_TreeType)->Draw();
}

void Tree::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetTreeModel(m_TreeType)->Draw();
}
